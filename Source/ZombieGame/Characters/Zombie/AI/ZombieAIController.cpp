// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ZombieGame/Characters/Zombie/FireZombieBoss.h"
#include "ZombieGame/Characters/Grandad/Grandad.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "ZombieGame/GameMode/ZombieGameMode.h"
#include "ZombieGame/Characters/FrontDoor/FrontDoor.h"
#include "ZombieGame/Widgets/ShopItem.h"

AZombieAIController::AZombieAIController()
{
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}

void AZombieAIController::BeginPlay()
{
    Super::BeginPlay();

    // binds OnDoorSpawn delegate to update door location on spawning
    if (AZombieGameMode *GameMode = Cast<AZombieGameMode>(UGameplayStatics::GetGameMode(this)))
    {
        GameMode->OnDoorSpawn.AddDynamic(this, &AZombieAIController::HandleDoorSpawned);
        GameMode->OnDoorOpen.AddDynamic(this, &AZombieAIController::HandleDoorOpen);
    }

    // Setting the grandad blackboard value needs a split second before being called, otherwise crashes
    FTimerHandle DelayHandle;
    GetWorldTimerManager().SetTimer(DelayHandle, this, &AZombieAIController::InitializeGrandadBlackboardValue, 0.5f, false);

    if (AIPerceptionComponent != nullptr)
    {
        AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AZombieAIController::EnemyDetected);
    }

    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);
    }
}

// Bound on begin play if there's an AIPerceptionComponent
void AZombieAIController::EnemyDetected(AActor *Actor, FAIStimulus Stimulus)
{
    ACharacter *Player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter *ZombieGameCharacter = Cast<AZombieGameCharacter>(Player);

    if (Actor == Player)
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            Target = ZombieGameCharacter;
            GetBlackboardComponent()->ClearValue("LastKnownPlayerLocation");
            GetBlackboardComponent()->SetValueAsObject(TEXT("PlayerCharacter"), Player);
        }
        else if (Stimulus.WasSuccessfullySensed() == false)
        {
            GetBlackboardComponent()->ClearValue("PlayerCharacter");
            GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), Stimulus.StimulusLocation);
            AFrontDoor *FrontDoor = AFrontDoor::GetInstance();
            if (FrontDoor && FrontDoor->bDoorOpen == false) // if door is closed
            {
                Target = FrontDoor;
            }
            else
            {
                Target = AGrandad::GetInstance();
            }
        }
    }
}

void AZombieAIController::HandleDoorSpawned()
{
    AFrontDoor *FrontDoor = AFrontDoor::GetInstance();
    if (FrontDoor->bIsSpawned)
    {
        GetBlackboardComponent()->SetValueAsVector(TEXT("DoorLocation"), FrontDoor->GetActorLocation());
        Target = FrontDoor;
    }
    else
    {
        GetBlackboardComponent()->ClearValue(TEXT("DoorLocation"));
        Target = AGrandad::GetInstance();
    }
}

void AZombieAIController::HandleDoorOpen()
{
    AFrontDoor *FrontDoor = AFrontDoor::GetInstance();
    if (FrontDoor && FrontDoor->bDoorOpen == false) // if door is closed
    {
        GetBlackboardComponent()->ClearValue(TEXT("DoorLocation"));
        Target = AGrandad::GetInstance();
    }
    else if (FrontDoor && FrontDoor->bDoorOpen == true) // if door is open
    {
        GetBlackboardComponent()->SetValueAsVector(TEXT("DoorLocation"), FrontDoor->GetActorLocation());
        Target = FrontDoor;
    }
}

void AZombieAIController::InitializeGrandadBlackboardValue()
{
    // Sets target as grandad, sets corresponding blackboard key.
    if (AGrandad *Grandad = AGrandad::GetInstance())
    {
        GetBlackboardComponent()->SetValueAsVector(TEXT("GrandadLocation"), Grandad->GetActorLocation());
        Target = Grandad;
    }
}


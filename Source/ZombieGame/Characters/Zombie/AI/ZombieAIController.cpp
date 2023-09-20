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

    if (AIPerceptionComponent != nullptr)
    {
        AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AZombieAIController::EnemyDetected);
    }

    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);

        APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    }
}

// Bound on begin play if there's an AIPerceptionComponent
void AZombieAIController::EnemyDetected(AActor *Actor, FAIStimulus Stimulus)
{
    AZombieGameCharacter *Player = Cast<AZombieGameCharacter>(Actor);
    if (Player == nullptr)
        return;

    AZombie *ZombieCharacter = Cast<AZombie>(GetPawn());
}

void AZombieAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    // Casting to different PlayerCharactrer types
    APawn *PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);
    ACharacter *Player = Cast<ACharacter>(PlayerPawn);
    AZombieGameCharacter *ZombieGameCharacter = Cast<AZombieGameCharacter>(PlayerPawn);

    // Gets Zombie and Fire Zombie
    AZombie *ZombieCharacter = Cast<AZombie>(GetPawn());
    AFireZombieBoss *FireZombieCharacter = Cast<AFireZombieBoss>(GetPawn());

    // Gets grandad instance
    AGrandad *Grandad = AGrandad::GetInstance();
    GetBlackboardComponent()->SetValueAsVector(TEXT("GrandadLocation"), Grandad->GetActorLocation());
    Target = Grandad;

    // Get front door instance
    AFrontDoor* FrontDoor = AFrontDoor::GetInstance();
    if (FrontDoor && FrontDoor->bIsSpawned)
    {
        GetBlackboardComponent()->SetValueAsVector(TEXT("DoorLocation"), FrontDoor->GetActorLocation());
        Target = FrontDoor;
    }
    if (FrontDoor && FrontDoor->bDoorOpen == true)
    {
        GetBlackboardComponent()->ClearValue(TEXT("DoorLocation"));
        Target = Grandad;
    }

    if (ZombieCharacter->GetIsZombieDead() == false)
    {
        UE_LOG(LogTemp, Log, TEXT("Name of actor: %s"), *ZombieCharacter->GetName());
        if (ZombieCharacter->GetName().StartsWith("BP_FireZombie")) // if it's a fire boss zombie
        {
            UCharacterMovementComponent *FireZombieMovement = Cast<UCharacterMovementComponent>(FireZombieCharacter->GetMovementComponent());
            if (ZombieCharacter->GetZombieHitCheck() == true)
            {
                FireZombieMovement->MaxWalkSpeed = 600;
                GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), Player->GetActorLocation());
                GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), Player->GetActorLocation());
                ZombieCharacter->SetZombieHitCheck(false);
            }
            else if (AIPerceptionComponent->HasAnyCurrentStimulus(*Player))
            {
                UE_LOG(LogTemp, Log, TEXT("Is fire zombie"));
                FireZombieMovement->MaxWalkSpeed = 600;
                GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), Player->GetActorLocation());
                GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), Player->GetActorLocation());
            }
            else
            {
                ZombieCharacter->SetZombieHitCheck(false);
                GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
            }
        }
        else // if it's a normal zombie
        {
            UCharacterMovementComponent *ZombieMovement = Cast<UCharacterMovementComponent>(ZombieCharacter->GetMovementComponent());
            if (ZombieCharacter->GetZombieHitCheck() == true)
            {
                if (ZombieCharacter->GetIsCrawling() == false)
                {
                    ZombieMovement->MaxWalkSpeed = 300;
                }
                else
                {
                    ZombieMovement->MaxWalkSpeed = 60;
                }
                GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), Player->GetActorLocation());
                GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), Player->GetActorLocation());
                ZombieCharacter->SetZombieHitCheck(false);
                Target = ZombieGameCharacter;
            }

            else if (AIPerceptionComponent->HasAnyCurrentStimulus(*Player))
            {
                UE_LOG(LogTemp, Log, TEXT("Is normal zombie"));
                if (ZombieCharacter->GetIsCrawling() == false)
                {
                    ZombieMovement->MaxWalkSpeed = 300;
                }
                else
                {
                    ZombieMovement->MaxWalkSpeed = 60;
                }
                GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), Player->GetActorLocation());
                GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), Player->GetActorLocation());
                Target = ZombieGameCharacter;
            }
            else
            {
                ZombieCharacter->SetZombieHitCheck(false);
                GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
                Target = FrontDoor;
            }
        }
    }
    else
    {
        this->UnPossess(); // Unpossess to stop the AI
        this->Destroy();   // Destroy the controller
    }
}

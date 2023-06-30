// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieAIController.h"
#include "Kismet/GameplayStatics.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "Zombie.h"
#include "ZombieGameCharacter.h"



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

        APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0); 

        // GetBlackboardComponent()->SetValueAsVector(TEXT("StartLocation"), GetPawn()->GetActorLocation()); 
    }

    
}

void AZombieAIController::Tick(float DeltaSeconds) 
{
    Super::Tick(DeltaSeconds);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    AZombie* ZombieCharacter = Cast<AZombie>(GetPawn());

    ACharacter* Player =  Cast<ACharacter>(PlayerPawn);

    AZombieGameCharacter* ZombieGameCharacter = Cast<AZombieGameCharacter>(PlayerPawn);

    GetBlackboardComponent()->SetValueAsVector(TEXT("HouseLocation"), HouseLocation);
    
    if (ZombieCharacter->IsDead == false)
    {
        UCharacterMovementComponent* ZombieMovement = Cast<UCharacterMovementComponent>(ZombieCharacter->GetMovementComponent());

        if (ZombieCharacter->ZombieHitCheck == true)
        {
            ZombieMovement->MaxWalkSpeed=300;
            GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), Player->GetActorLocation());
            GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), Player->GetActorLocation());
            ZombieCharacter->ZombieHitCheck = false;
        }

        else if (AIPerceptionComponent->HasAnyCurrentStimulus(*Player))
        {
            ZombieMovement->MaxWalkSpeed=300;
            GetBlackboardComponent()->SetValueAsVector(TEXT("PlayerLocation"), Player->GetActorLocation());
            GetBlackboardComponent()->SetValueAsVector(TEXT("LastKnownPlayerLocation"), Player->GetActorLocation());
            // ZombieCharacter->ZombieHitCheck = false;
        }
        else 
        {
            ZombieCharacter->ZombieHitCheck = false;
            GetBlackboardComponent()->ClearValue(TEXT("PlayerLocation"));
        }
    }
    else
    {
        this->UnPossess(); // Unpossess to stop the AI
        this->Destroy(); // Destroy the controller
    }
}

AZombieAIController::AZombieAIController() 
{
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}

void AZombieAIController::EnemyDetected(AActor* Actor, FAIStimulus Stimulus) 
{  
    AZombieGameCharacter* Player =  Cast<AZombieGameCharacter>(Actor);
    if ( Player == nullptr )	return;

    AZombie* ZombieCharacter = Cast<AZombie>(GetPawn());
}








// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieGame/Characters/Turret/TurretAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "ZombieGame/Characters/Zombie/Zombie.h"
#include "Kismet/KismetMathLibrary.h"
#include "ZombieGame/Characters/Turret/Turret.h"
#include "EnvironmentQuery/EnvQueryManager.h"

ATurretAIController::ATurretAIController()
{
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}

void ATurretAIController::BeginPlay()
{
    Super::BeginPlay();

    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);
    }

    if (AIPerceptionComponent != nullptr)
    {
        AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ATurretAIController::EnemyDetected);
        
    }
}

void ATurretAIController::EnemyDetected(AActor *Actor, FAIStimulus Stimulus)
{
    UE_LOG(LogTemp, Warning, TEXT("zombies detected"));

    if (AZombie *Zombie = Cast<AZombie>(Actor))
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            Zombies.Add(Zombie);
            // Zombie->OnZombieDestroyed.AddDynamic(this, &ATurretAIController::ZombieDeath);
        }
        else if (Stimulus.WasSuccessfullySensed() == false)
        {
            // Zombie->OnZombieDestroyed.AddDynamic(this, &ATurretAIController::ZombieDeath);
            Zombies.Remove(Zombie);
        }
        // Update CanSeeZombies based on the number of sensed zombies.
        GetBlackboardComponent()->SetValueAsBool(TEXT("CanSeeZombies"), Zombies.Num() > 0);
    }
}






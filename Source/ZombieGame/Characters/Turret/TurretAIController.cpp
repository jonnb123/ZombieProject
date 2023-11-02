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

    // FEnvQueryRequest HidingSpotQueryRequest = FEnvQueryRequest(FindZombieEQS, GetPawn());
    // HidingSpotQueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &ATurretAIController::ZombieQueryResult);

    AZombie *Zombie = Cast<AZombie>(Actor);
    if (Zombie)
    {
        if (Stimulus.WasSuccessfullySensed())
        {
            Zombies.Add(Zombie);
        }
        else if (Stimulus.WasSuccessfullySensed() == false)
        {
            Zombies.Remove(Zombie);
        }
        // Update CanSeeZombies based on the number of sensed zombies.
        GetBlackboardComponent()->SetValueAsBool(TEXT("CanSeeZombies"), Zombies.Num() > 0);
    }
}

void ATurretAIController::ZombieQueryResult(TSharedPtr<FEnvQueryResult> Result)
{
    if (Result->IsSuccessful())
    {
        // AActor* PreferredActor = Result->GetItemAsActor(0);

        // UE_LOG(LogTemp, Warning, TEXT("Preferred actor is %s:"), *PreferredActor->GetName());
        TArray<AActor *> AllActors;

        Result->GetAllAsActors(AllActors);

        for (AActor *Actor : AllActors)
        {
            AZombie *Zombie = Cast<AZombie>(Actor);
            if (Zombie)
            {
                Zombies.Add(Zombie);
                // Now you can work with each detected AZombie.
                UE_LOG(LogTemp, Warning, TEXT("Found Zombie: %s"), *Zombie->GetName());
            }
        }
    }
}

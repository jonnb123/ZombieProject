// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieGame/Characters/Dog/AI/DogAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieGame/Characters/Zombie/Zombie.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ZombieGame/GameMode/ZombieGameMode.h"


void ADogAIController::BeginPlay()
{
    Super::BeginPlay();

    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);
    }
}

void ADogAIController::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

}

ADogAIController::ADogAIController()
{
   
}



// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveSpeed.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"

UBTTask_MoveSpeed::UBTTask_MoveSpeed()
{
    NodeName = TEXT("Change character speed");
}

EBTNodeResult::Type UBTTask_MoveSpeed::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AAIController *AIController{OwnerComp.GetAIOwner()};
    const APawn *AIPawn{AIController->GetPawn()};

    if (AIPawn->GetName().StartsWith("BP_FireZombie")) // if it's a fire boss zombie
    {
        UCharacterMovementComponent *FireZombieMovement = Cast<UCharacterMovementComponent>(AIPawn->GetMovementComponent());

        UCharacterMovementComponent *ZombieMovement = Cast<UCharacterMovementComponent>(AIPawn->GetMovementComponent());

        ZombieMovement->MaxWalkSpeed = 600;

        return EBTNodeResult::Succeeded;
    }


    else // for the zombie and the dog
    {
        UCharacterMovementComponent *CharacterMovement = Cast<UCharacterMovementComponent>(AIPawn->GetMovementComponent());
        if (CharacterMovement->MaxWalkSpeed == 60)
        {
            CharacterMovement->MaxWalkSpeed = 300;
        }
        else
        {
            CharacterMovement->MaxWalkSpeed = 60;
        }
        return EBTNodeResult::Succeeded;
    }
}

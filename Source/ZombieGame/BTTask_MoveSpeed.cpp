// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_MoveSpeed.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Zombie.h"
#include "NavigationSystem.h"


UBTTask_MoveSpeed::UBTTask_MoveSpeed() 
{
    NodeName = TEXT("Change speed of zombie");
}

EBTNodeResult::Type UBTTask_MoveSpeed::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);


    AAIController* AIController {OwnerComp.GetAIOwner()};
    const APawn* AIPawn {AIController->GetPawn()};

    UCharacterMovementComponent* ZombieMovement = Cast<UCharacterMovementComponent>(AIPawn->GetMovementComponent());

    ZombieMovement->MaxWalkSpeed = 60;

    return EBTNodeResult::Succeeded;
}

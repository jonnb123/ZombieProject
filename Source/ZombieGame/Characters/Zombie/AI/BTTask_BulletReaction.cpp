// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_BulletReaction.h"
#include "Kismet/GameplayStatics.h"

UBTTask_BulletReaction::UBTTask_BulletReaction() 
{
    NodeName = TEXT("Bullet Reaction");
}

EBTNodeResult::Type UBTTask_BulletReaction::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) 
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    APawn* PlayerPawn = UGameplayStatics::GetPlayerPawn(GetWorld(), 0);

    return EBTNodeResult::Succeeded;
}

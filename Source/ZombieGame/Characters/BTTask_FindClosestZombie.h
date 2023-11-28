// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "AIController.h"
#include "BTTask_FindClosestZombie.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API UBTTask_FindClosestZombie : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

protected:
	UBTTask_FindClosestZombie();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;

	UBehaviorTreeComponent *CachedOwnerComp; // Member variable to store OwnerComp

	UPROPERTY(EditAnywhere, Category = "AI")
	UEnvQuery *FindZombieEQS;

	void ZombieQueryResult(TSharedPtr<FEnvQueryResult> Result);
	
	AAIController* AIController;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_MoveSpeed.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API UBTTask_MoveSpeed : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
	


protected:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;

	UBTTask_MoveSpeed();
};

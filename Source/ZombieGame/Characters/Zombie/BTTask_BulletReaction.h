// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_MoveTo.h"
#include "BTTask_BulletReaction.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API UBTTask_BulletReaction : public UBTTask_MoveTo
{
	GENERATED_BODY()

public: 
	UBTTask_BulletReaction();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
	



};

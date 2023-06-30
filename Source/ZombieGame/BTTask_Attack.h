// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_PlayAnimation.h"
#include "BTTask_Attack.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API UBTTask_Attack : public UBTTask_PlayAnimation
{
	GENERATED_BODY()
	
public:
	UBTTask_Attack();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere)
	float MeleeDamage = 10;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
};

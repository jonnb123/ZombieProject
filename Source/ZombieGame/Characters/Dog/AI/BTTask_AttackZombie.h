// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "Sound/SoundCue.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "BTTask_AttackZombie.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API UBTTask_AttackZombie : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

protected:
	UBTTask_AttackZombie();

	UBehaviorTreeComponent *CachedOwnerComp; // Member variable to store OwnerComp

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage *AttackMontage;

	UFUNCTION()
	void OnAttackEnd(UAnimMontage *Montage, bool bInterrupted);

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue *DogAttackSound;

	UPROPERTY(EditAnywhere)
	float BiteDamage = 20;

	AAIController* AIController;

	AActor* ZombieActor;
};

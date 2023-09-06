// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/Tasks/BTTask_PlayAnimation.h"
#include "Sound/SoundCue.h"
#include "AttackNotify.h"
#include "BTTask_AttackPlayer.generated.h"

/**
 *
 */


UCLASS()
class ZOMBIEGAME_API UBTTask_AttackPlayer : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory) override;

	UBTTask_AttackPlayer();

	UFUNCTION()
	void OnAttackEnd();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAttackNotify* AttackNotify;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage *AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage *CrawlAttackMontage;

	UPROPERTY(EditAnywhere)
	float MeleeDamage = 10;

	UPROPERTY(EditAnywhere)
	float FireMeleeDamage = 20;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue *ZombieAttackSound;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue *FireZombieAttackSound;
};

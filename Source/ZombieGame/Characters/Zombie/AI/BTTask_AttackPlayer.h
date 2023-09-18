// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/Tasks/BTTask_PlayAnimation.h"
#include "Sound/SoundCue.h"
#include "ZombieGame/Characters/FrontDoor/FrontDoor.h"
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

	// This breaks the zombie out of the attack node
	UFUNCTION()
	void OnAttackEnd(UAnimMontage *Montage, bool bInterrupted);

	UBehaviorTreeComponent *CachedOwnerComp; // Member variable to store OwnerComp

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage *AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	class UAnimMontage *CrawlAttackMontage;

	// Define a Blackboard key for the target actor
	UPROPERTY(EditAnywhere, Category = Blackboard)
	bool IsCharacter;

	// Define a Blackboard key for the target actor
	UPROPERTY(EditAnywhere, Category = Blackboard)
	bool IsGrandad;

	// Define a Blackboard key for the target actor
	UPROPERTY(EditAnywhere, Category = Blackboard)
	bool IsDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blackboard)
	AActor* Target;

	UPROPERTY(EditAnywhere)
	float MeleeDamage = 10;

	UPROPERTY(EditAnywhere)
	bool bAttackEnd = false;

	UPROPERTY(EditAnywhere)
	float FireMeleeDamage = 20;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue *ZombieAttackSound;

	UPROPERTY(EditAnywhere, Category = "Audio")
	USoundCue *FireZombieAttackSound;

	// UFUNCTION(BlueprintCallable)
	// void SetFrontDoorReference(AFrontDoor* InFrontDoor);

	// AFrontDoor* FrontDoorReference;

public:
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blackboard)
	// AActor* Target;

};

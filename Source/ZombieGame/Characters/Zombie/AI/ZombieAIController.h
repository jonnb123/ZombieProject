// Fill out your copyright notice in the Description page of Project Settings.
// novmeber
#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIController.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	AZombieAIController();

	UPROPERTY(VisibleAnywhere, Category="AI", meta=(AllowPrivateAccess="true"))
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

	UFUNCTION()
	void EnemyDetected(AActor* Actor, FAIStimulus Stimulus);

	

	UFUNCTION()
	void InitializeGrandadBlackboardValue();

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Blackboard)
	AActor* Target;

	UFUNCTION()
	void HandleDoorSpawned();

	UFUNCTION()
	void HandleDoorOpen();
	
};

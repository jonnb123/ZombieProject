// Fill out your copyright notice in the Description page of Project Settings.
// november/december
#pragma once

#include "CoreMinimal.h"
#include "Perception/AIPerceptionComponent.h"
#include "AIController.h"
// #include "Perception/PawnSensingComponent.h"
#include "ZombieAIController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API AZombieAIController : public AAIController
{
	GENERATED_BODY()

public:

	virtual void Tick(float DeltaSeconds) override;

	
	AZombieAIController();

	UFUNCTION()
	void EnemyDetected(AActor* Actor, FAIStimulus Stimulus);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="AI", meta=(AllowPrivateAccess="true"))
	UAIPerceptionComponent* AIPerceptionComponent;

	UPROPERTY(EditAnywhere)
	FVector HouseLocation;

private:
	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

};

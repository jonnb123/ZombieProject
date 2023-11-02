// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "EnvironmentQuery/EnvQuery.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "ZombieGame/Characters/Zombie/Zombie.h"
// #include "Perception/PawnSensingComponent.h"
#include "TurretAIController.generated.h"

/**
 *
 */
UCLASS()
class ZOMBIEGAME_API ATurretAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	ATurretAIController();

	UPROPERTY(EditAnywhere)
	class UBehaviorTree *AIBehavior;

	UPROPERTY(VisibleAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	UAIPerceptionComponent *AIPerceptionComponent;

	TArray<AZombie*> Zombies;

	UFUNCTION()
	void EnemyDetected(AActor* Actor, FAIStimulus Stimulus);

	UPROPERTY(EditAnywhere, Category = "AI")
    UEnvQuery *FindZombieEQS;

	void ZombieQueryResult(TSharedPtr<FEnvQueryResult> Result);

	// UPROPERTY(VisibleAnywhere, Category = "AI", meta = (AllowPrivateAccess = "true"))
	// UPawnSensingComponent *PawnSensingComponent;

	// UFUNCTION()
	// void CanSeeEnemy(APawn* SeenPawn);

private:
	
};

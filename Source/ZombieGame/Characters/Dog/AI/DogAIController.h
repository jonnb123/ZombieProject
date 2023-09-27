// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "ZombieGame/Characters/Zombie/Zombie.h"
#include "DogAIController.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API ADogAIController : public AAIController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	ADogAIController();

	UPROPERTY(EditAnywhere)
	class UBehaviorTree* AIBehavior;

};

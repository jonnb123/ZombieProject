// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuyableItem.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "Turret.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimSequence.h"
#include "PerkMachine.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API APerkMachine : public ABuyableItem
{
	GENERATED_BODY()

public:
	APerkMachine();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PerkMachineMesh;

	UFUNCTION(BlueprintCallable)
	void PlayConsumeAnimation();

	UFUNCTION(BlueprintCallable)
	void PlayConsumeAnimationSecondHalf();

	UFUNCTION(BlueprintCallable)
	virtual void UsePerk();

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
// #include "BuyableItem.h"
#include "ZombieGame/BuyableItem/BuyableItem.h"
#include "GameFramework/Actor.h"
// #include "Turret.h"
#include "ZombieGame/Characters/Turret/Turret.h"
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

	FTimerHandle PlayAfterDelayHandle;
	FTimerDelegate PlayAfterDelayDelegate;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PerkMachineMesh;

	UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

	UFUNCTION(BlueprintCallable)
	void PlayConsumeAnimation();

	UFUNCTION(BlueprintCallable)
	void PlayConsumeAnimationSecondHalf();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMesh* PerkBottle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    UAnimSequence* DrinkingAnimationSequence;

};

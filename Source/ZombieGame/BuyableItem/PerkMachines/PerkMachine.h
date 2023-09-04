// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieGame/BuyableItem/BuyableItem.h"
#include "PerkMachine.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API APerkMachine : public ABuyableItem
{
	GENERATED_BODY()

protected:
	APerkMachine();

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PerkMachineMesh;

	UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMesh* PerkBottle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation")
    UAnimSequence* DrinkingAnimationSequence;

	UFUNCTION(BlueprintCallable)
	void PlayConsumeAnimation();

	UFUNCTION(BlueprintCallable)
	void PlayConsumeAnimationSecondHalf();

	virtual void UseBuyableItem() override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PerkMachine.h"
#include "ZombieGame/Widgets/BuyableItemInterface.h"
#include "FullyAuto.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API AFullyAuto : public APerkMachine, public IBuyableItemInterface
{
	GENERATED_BODY()
public:
	virtual void HandleBuyItem() override;

	
private:
	void IncreaseFireRate();

	bool FireRateComplete = false;
};

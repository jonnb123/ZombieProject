// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PerkMachine.h"
#include "ZombieGame/Widgets/BuyableItemInterface.h"
#include "ExtendedMag.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API AExtendedMag : public APerkMachine, public IBuyableItemInterface
{
	GENERATED_BODY()

public:
	virtual void HandleBuyItem() override;

private:
	void ExtendMagazine();

	bool ExtendedMagComplete;
};

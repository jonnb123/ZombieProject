// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PerkMachine.h"
#include "ZombieGame/Widgets/BuyableItemInterface.h"
#include "HealthJuice.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API AHealthJuice : public APerkMachine, public IBuyableItemInterface
{
	GENERATED_BODY()

public:
	virtual void HandleBuyItem() override;

private:

	
};

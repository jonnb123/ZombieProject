// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieGameProjectile.h"
#include "ZombieGame/Widgets/BuyableItemInterface.h"
#include "PistolProjectile.generated.h"

/**
 *
 */
UCLASS()
class ZOMBIEGAME_API APistolProjectile : public AZombieGameProjectile, public IBuyableItemInterface
{
	GENERATED_BODY()

public:
	APistolProjectile();

	virtual void HandleBuyItem() override;

};

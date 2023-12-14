// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieGameProjectile.h"
#include "ZombieGame/Widgets/BuyableItemInterface.h"
#include "ShotgunProjectile.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API AShotgunProjectile : public AZombieGameProjectile, public IBuyableItemInterface
{
	GENERATED_BODY()

public:
	AShotgunProjectile();

	virtual void HandleBuyItem() override;
};

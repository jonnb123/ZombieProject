// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PerkMachine.h"
#include "FullyAuto.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API AFullyAuto : public APerkMachine
{
	GENERATED_BODY()

private:
	void UseBuyableItem() override;

	void IncreaseFireRate();

	bool FireRateComplete = false;
};

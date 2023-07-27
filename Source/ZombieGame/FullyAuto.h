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

public:
	void UseBuyableItem() override;

	UFUNCTION(BlueprintCallable)
	void IncreaseFireRate();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complete")
	bool FireRateComplete = false;
};

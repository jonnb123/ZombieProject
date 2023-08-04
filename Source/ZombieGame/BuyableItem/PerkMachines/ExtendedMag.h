// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PerkMachine.h"
#include "ExtendedMag.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API AExtendedMag : public APerkMachine
{
	GENERATED_BODY()
	
public:
	void UseBuyableItem() override;

	UFUNCTION(BlueprintCallable)
	void ExtendMagazine();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complete")
	bool ExtendedMagComplete;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PerkMachine.h"
#include "HealthJuice.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API AHealthJuice : public APerkMachine
{
	GENERATED_BODY()
	
public:
	void UseBuyableItem() override;
};

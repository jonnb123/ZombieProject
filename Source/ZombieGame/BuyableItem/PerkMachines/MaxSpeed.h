// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PerkMachine.h"
#include "MaxSpeed.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API AMaxSpeed : public APerkMachine
{
	GENERATED_BODY()
	
private:
	void UseBuyableItem() override;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuyableItem.h"
#include "BuyDoor.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API ABuyDoor : public ABuyableItem
{
	GENERATED_BODY()

public:
	virtual void BuyDoor();
	bool bDoorRotated = false;
};

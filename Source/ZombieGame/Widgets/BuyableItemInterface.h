// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "BuyableItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UBuyableItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ZOMBIEGAME_API IBuyableItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void HandleBuyItem() = 0;
};

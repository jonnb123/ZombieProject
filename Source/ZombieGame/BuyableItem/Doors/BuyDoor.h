// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieGame/BuyableItem/BuyableItem.h"
#include "ZombieGame/Widgets/BuyableItemInterface.h"
#include "BuyDoor.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API ABuyDoor : public ABuyableItem, public IBuyableItemInterface
{
	GENERATED_BODY()
public:
	virtual void HandleBuyItem() override;
private:
	ABuyDoor();

	
	bool bDoorRotated = false;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorMesh;
};

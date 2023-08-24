// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieGame/BuyableItem/BuyableItem.h"
#include "BuyDoor.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API ABuyDoor : public ABuyableItem
{
	GENERATED_BODY()

private:
	ABuyDoor();

	void UseBuyableItem() override;
	bool bDoorRotated = false;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorMesh;
};

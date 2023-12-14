// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "ZombieGame/Widgets/BuyableItemInterface.h"
#include "AssaultRifle.generated.h"

UCLASS()
class ZOMBIEGAME_API AAssaultRifle : public ABaseWeapon, public IBuyableItemInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AAssaultRifle();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void HandleBuyItem() override;
};

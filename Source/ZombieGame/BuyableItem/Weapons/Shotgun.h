// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseWeapon.h"
#include "ZombieGame/Widgets/BuyableItemInterface.h"
#include "Shotgun.generated.h"

UCLASS()
class ZOMBIEGAME_API AShotgun : public ABaseWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AShotgun();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
};

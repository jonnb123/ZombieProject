// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PerkMachine.h"
#include "AddTurret.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API AAddTurret : public APerkMachine
{
	GENERATED_BODY()

public:
	void UsePerk() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complete")
	bool TurretComplete = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SpawnRotation;

	// The class of the turret that will be spawned
    UPROPERTY(EditAnywhere)
    TSubclassOf<ATurret> TurretClass;
};

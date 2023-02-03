// Fill out your copyright notice in the Description page of Project Settings.
// added the perk system in February 2023
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractionInterface.h"
#include "Turret.h"
#include "BuyableItem.generated.h"

UCLASS()
class ZOMBIEGAME_API ABuyableItem : public AActor, public IInteractionInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuyableItem();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void HealthJuice();
	virtual void FullyAuto();
	virtual void MaxSpeed();
	virtual void ExtendedMag();


	// The name of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FString Name;

	// Price of the perk
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Price")
	FString ItemPrice;

	UFUNCTION(BlueprintImplementableEvent)
	void IncreaseFireRate();

	UFUNCTION(BlueprintImplementableEvent)
	void ExtendMagazine();


	UFUNCTION(BlueprintImplementableEvent)
	void PlayConsumeAnimation();

	UFUNCTION()
	void AddTurret();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complete")
	bool FireRateComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complete")
	bool ExtendedMagComplete;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Complete")
	bool TurretComplete = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector SpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator SpawnRotation;

	// The class of the turret that will be spawned
    UPROPERTY(EditAnywhere)
    TSubclassOf<ATurret> TurretClass;

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* _RootComponent;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* PerkMachineMesh;

    // // The location at which the turret will be spawned
    UPROPERTY(EditAnywhere, Category = "Turret")
    USceneComponent* TurretSpawnLocation;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseAmmo.generated.h"


UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	E_AssaultRifle 	 UMETA(DisplayName = "ASSAULT_RIFLE"),
	E_Pistol		 UMETA(DisplayName = "PISTOL"),
	E_Shotgun  		 UMETA(DisplayName = "SHOTGUN")
};


UCLASS()
class ZOMBIEGAME_API ABaseAmmo : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseAmmo();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// The total amount of ammo in this pickup.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int AmmoAmount;

	// The type of ammo in this pickup
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EAmmoType AmmoType;

	

};

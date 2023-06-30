// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseWeapon.generated.h"


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	E_Pistol		 UMETA(DisplayName = "PISTOL"),
	E_AssaultRifle 	 UMETA(DisplayName = "ASSAULT_RIFLE"),
	E_Shotgun  		 UMETA(DisplayName = "SHOTGUN")
};


UCLASS()
class ZOMBIEGAME_API ABaseWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int CurrentAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int TotalAmmo;		

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int MaxClipSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool Overlapping;

	// The type of weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType;

	// The name of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FString Name;

	// The name of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FString ItemPrice;

	// has the weapon been obtained by the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool IsObtained;

	// the indented index of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int Index;

	// the indented index of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRate;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

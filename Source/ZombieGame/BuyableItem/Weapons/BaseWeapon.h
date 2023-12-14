// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieGame/BuyableItem/BuyableItem.h"
#include "ZombieGame/BuyableItem/Weapons/Projectiles/ZombieGameProjectile.h"
#include "ZombieGame/Widgets/BuyableItemInterface.h"
#include "BaseWeapon.generated.h"


UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	E_Pistol		 UMETA(DisplayName = "PISTOL"),
	E_AssaultRifle 	 UMETA(DisplayName = "ASSAULT_RIFLE"),
	E_Shotgun  		 UMETA(DisplayName = "SHOTGUN"),

	E_Size			 UMETA(DisplayName = "Size")
};

UCLASS()
class ZOMBIEGAME_API ABaseWeapon : public ABuyableItem, public IBuyableItemInterface
{
	GENERATED_BODY()
	
public:	
	// Needed in perkmachines, zombiegamecharacter, mainwidget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int CurrentWeaponAmmo;

	// Needed in zombiegamecharacter, mainwidget
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int TotalWeaponAmmo;

	// needed in zombiegamecharacter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int MaxWeaponAmmo;			

	// needed in zombiegamecharacter and ExtendedMag
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int MaxWeaponClipSize;

	// Needed in ZombieGameCharacter and FullyAuto
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float WeaponFireRate;

	// needed in ZombieGameCharacter
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AZombieGameProjectile> ProjectileClass;

	// Needed in ZombieGameCharacter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType;

	// Needed in ZombieGameCharacter
	UPROPERTY(EditAnywhere, Category = "Weapons")
    USkeletalMesh* WeaponMesh;

	// Needed in ZombieGameCharacter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName WeaponSocketName;

	// Needed in ZombieGameCharacter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool IsObtained;

	// Needed in ZombieGameCharacter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* WeaponFireMontage;

	// Needed in ZombieGameCharacter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* WeaponReloadMontage;
	
	virtual void HandleBuyItem() override;
protected:
	void UseBuyableItem() override;

};

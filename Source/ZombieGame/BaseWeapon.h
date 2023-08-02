// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Animation/AnimMontage.h"
#include "Engine/SkeletalMesh.h"
#include "Components/BoxComponent.h"
#include "BuyableItem.h"
#include "ZombieGameProjectile.h"
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
// class ZOMBIEGAME_API ABaseWeapon : public AActor
class ZOMBIEGAME_API ABaseWeapon : public ABuyableItem
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseWeapon();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int CurrentAmmo;

	// array iterates on the value of the Enum for weapons: EWeaponType
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<int> AmmoArray;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int TotalAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int MaximumAmmo;			

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int MaxClipSize;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool Overlapping;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AZombieGameProjectile> ProjectileClass;

	// The type of weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType;

	// fire animation for weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* WeaponFireMontage;

	// reload animation for weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Animation")
	UAnimMontage* WeaponReloadMontage;

	// mesh of weapon
	UPROPERTY(EditAnywhere, Category = "Weapons")
    USkeletalMesh* WeaponMesh;

	// The name of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName SocketName;

	// Name of socket to fire projectile from 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FName FireSocketName;

	// has the weapon been obtained by the player
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	bool IsObtained;

	// the indented index of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int Index;

	// the indented index of the weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float FireRate;

	ABaseWeapon* OverlappingWeapon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	UAnimSequence* WeaponPickupAnimation; // Declare the UPROPERTY for the animation asset

	// UFUNCTION(BlueprintCallable)
	void UseBuyableItem() override;

	// UPROPERTY(EditAnywhere, Category = "Damage")
	// float BodyDamage = 10;

	// UPROPERTY(EditAnywhere, Category = "Damage")
	// float HeadDamage = 50;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

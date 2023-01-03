// Copyright Epic Games, Inc. All Rights Reserved.
// made changes to gamemode in january
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "InteractionInterface.h"
#include "BaseWeapon.h"
#include "BaseAmmo.h"
#include "ZombieGameCharacter.generated.h"

class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;


// Declaration of the delegate that will be called when the Primary Action is triggered
// It is declared as dynamic so it can be accessed also in Blueprints
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);


UCLASS(config=Game)
class AZombieGameCharacter : public ACharacter
{
	GENERATED_BODY()

	// /** Pawn mesh: 1st person view (arms; seen only by self) */
	// UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	// USkeletalMeshComponent* Mesh1P;

	/** First person camera */ 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

public:
	AZombieGameCharacter();

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

	// /** Delegate to whom anyone can subscribe to receive this event */
	// UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnUseItem;

	// Determines if the character is currently sprinting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool IsAiming;

	// maximum range
	UPROPERTY(EditAnywhere)
	float MaxBulletRange = 2000;

	UPROPERTY(EditAnywhere)
	float MaxInteractRange = 150;

	UPROPERTY(EditAnywhere)
	float BodyDamage = 10;

	UPROPERTY(EditAnywhere)
	float HeadDamage = 50;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 100;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// int CurrentAmmo = 9;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// int TotalAmmo = 10;		

	// UPROPERTY(EditAnywhere, BlueprintReadWrite)
	// int MaxClipSize = 9;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int PistolAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int AssaultRifleAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int ShotgunAmmo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsReloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool IsShooting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Points = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool FastWalking = false;

	UPROPERTY(EditAnywhere)
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UNiagaraSystem* HeadshotFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UNiagaraSystem* BodyShotFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	class UMaterialInterface* BulletHole;

	// primary weapons 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<ABaseWeapon*> Weapons;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	// ABaseWeapon* Weapon;

	// index of the weapon the player is currently using
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int WeaponIndex;

	// Switch the player's current primary weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwitchToNextPrimaryWeapon();

	// switches mesh of weapon currently being used
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent, Category = "Weapon")
	void SwitchWeaponMesh(int _index);

	UFUNCTION(BlueprintImplementableEvent)
	void Death();

	UFUNCTION(BlueprintImplementableEvent)
	void HealthRegenTimer();

	UFUNCTION(BlueprintImplementableEvent)
	void BloodOverlay();

	UFUNCTION(BlueprintImplementableEvent)
	void HideBlood();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayFireAnimations();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayReloadAnimations();
	
	// UFUNCTION(BlueprintCallable)
	// void AddAmmoCrate();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAmmoSound();

	// UFUNCTION(BlueprintImplementableEvent)
	// void PlayConsumeAnimation();

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* OutOfAmmoSound;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "HeadShot")
	// class UMaterialInstance* HeadShotDamage;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "BodyShot")
	// class UMaterialInstance* BodyShotDamage;

	// add ammmo to the player's correct ammo type
	UFUNCTION(BlueprintCallable)
	void AddAmmo(EAmmoType _AmmoType, int _AmmoAmount);

	// max ammo 
	UFUNCTION(BlueprintCallable)
	void MaxAmmo();

	// The firing timer handle
	FTimerHandle FireTimerHandle;


	FHitResult Hit;

	EPhysicalSurface TempSurface;

protected:
	
	// /** Fires a projectile. */
	// void OnPrimaryAction();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	// Zoom in
	void ZoomIn();

	// zoom out
	void ZoomOut();

	// fire
	void Fire();

	// starts firing the weapon
	void StartFiring();

	// stops firing the weapon
	void StopFiring();
	
	// Reloads the current weapon on R button press
	void ManualReload();

	// Reloads the current weapon
	void ReloadWeapon(EWeaponType _WeaponType);

	//calculate the ammo in the weapon's clip and on the player
	int CalculateAmmo(int _AmmoAmount);

	// void Interact();

	void Interacting();

	
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


private:
	IInteractionInterface* Interface = nullptr;

};


// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
// #include "BaseWeapon.h"
#include "ZombieGame/BuyableItem/Weapons/BaseWeapon.h"
// #include "BaseAmmo.h"
#include "ZombieGame/BuyableItem/Weapons/Ammo/BaseAmmo.h"
#include "Animation/AnimSequence.h"
#include "Engine/SkeletalMesh.h"
// #include "MainWidget.h"
#include "ZombieGame/Widgets/MainWidget.h"
// #include "PerkMachine.h"
#include "ZombieGame/BuyableItem/PerkMachines/PerkMachine.h"
// #include "BuyableItem.h"
#include "ZombieGame/BuyableItem/BuyableItem.h"
#include "ZombieGameCharacter.generated.h"

class USkeletalMesh;
class UCameraComponent;
class USoundBase;
class UNiagaraSystem;
class UMaterialInterface;

UCLASS(config = Game)
class AZombieGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AZombieGameCharacter();

	// Getters for meshes and cameras to be accessed in any class
	USkeletalMeshComponent *GetMesh1P() const { return Mesh1P; }
	USkeletalMeshComponent *GetGunMesh() const { return GunMesh; }
	UCameraComponent *GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	UCameraComponent *GetAnimationCameraComponent() const { return AnimationCameraComponent; }

	// Get a reference to the buyable item being overlapped
	UPROPERTY(EditAnywhere, Category = "References")
	ABuyableItem *OverlappingBuyableItem;

	// gets main widget reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TSubclassOf<UUserWidget> WidgetClass;
	UMainWidget *MainWidgetInstance;

	// Booleans

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool IsAiming = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDead = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsReloading = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool IsShooting = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasMaxSpeed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool IsSwappingWeapon = false;

	// Values

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health = 100;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Points = 1000;

	// Weapons

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
	EWeaponType EquippedWeaponCharacter;

	EWeaponType CurrentWeaponID;
	// index of the weapon the player is currently using
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int CurrentWeaponIndex = static_cast<int>(CurrentWeaponID);

	// primary weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<ABaseWeapon *> Weapons;

	// array iterates on the value of the Enum for weapons: EWeaponType
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<int> AmmoArray;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase *OutOfAmmoSound;

	// Functions

	// The virtual here shows that I intend to override the function, it overrides the virtual function from the base class, i.e. APawn
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;

	// Switch the player's current primary weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwitchToNextPrimaryWeapon();

	// called after a timer during SwitchToNextPrimaryWeapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void WeaponSwapAfterDelay();
	
	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION(BlueprintCallable)
	void RegenerateHealth();

	UFUNCTION(BlueprintImplementableEvent)
	void BloodOverlay();

	UFUNCTION(BlueprintImplementableEvent)
	void HideBlood();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAmmoSound();

	UFUNCTION(BlueprintCallable)
	void MaxAmmo();

	void ManualReload();

	void ReloadWeapon(EWeaponType _WeaponType);

	void CalculateAmmo();

	void OnInteractingPressed();

	// Timers 

	// The time for this is the fire-rate of the weapon, used in the weapon classes
	FTimerHandle FireTimerHandle; // Not used with delegate

	// timer is used locally
	FTimerHandle ReloadTimerHandle;

	const float WeaponSwapDelay = 0.7;
	FTimerHandle WeaponSwapTimerHandle; // Not used with delegate

	FTimerDelegate HealthRegenTimerDelegate;
	FTimerHandle HealthRegenTimerHandle;
	const float HealthRegenDuration = 5.0;

protected:
	virtual void BeginPlay();

	// Input functions
	virtual void SetupPlayerInputComponent(UInputComponent *InputComponent) override;
	void MoveForward(float Val);
	void MoveRight(float Val);
	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);
	void ZoomIn();
	void ZoomOut();
	void Fire();
	void StartFiring();
	void StopFiring();

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float TurnRateGamepad = 45.f; 

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	USkeletalMeshComponent *Mesh1P;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	USkeletalMeshComponent *GunMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent *FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent *AnimationCameraComponent;
	
};

// // meshes of weapons
// 	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
// 	TArray<USkeletalMesh *> WeaponMeshes;

// EPhysicalSurface TempSurface;
// FHitResult Hit;
//  UPROPERTY(EditAnywhere, Category = "Impact Effect")
// 	UParticleSystem* ImpactEffect;

// 	UPROPERTY(EditAnywhere, Category = "Impact Effect")
// 	UParticleSystem* FireImpactEffect;
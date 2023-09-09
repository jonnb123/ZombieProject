// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ZombieGame/BuyableItem/Weapons/BaseWeapon.h"
#include "ZombieGame/Widgets/MainWidget.h"
#include "ZombieGameCharacter.generated.h"

class USkeletalMesh;
class UCameraComponent;
class USoundBase;
class UNiagaraSystem;
class UMaterialInterface;

// careful changing the ordering, as abp relies on the int of the enum
UENUM(BlueprintType)
enum class ECharacterState : uint8
{
	Idle,
	SwappingWeapon,
	Aiming,
	Firing,
	Reloading,
	AimFiring
};

// add a change state function

UCLASS(config = Game)
class AZombieGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AZombieGameCharacter();

	// Getters for meshes and cameras
	USkeletalMeshComponent *GetMesh1P() const { return Mesh1P; }
	USkeletalMeshComponent *GetGunMesh() const { return GunMesh; }
	UCameraComponent *GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	UCameraComponent *GetAnimationCameraComponent() const { return AnimationCameraComponent; }

	// Character Health get/set
	float GetCharacterHealth() const { return Health; }
	void SetCharacterHealth(float NewValue) { Health = NewValue; }

	// Points get/set
	int GetPoints() const { return Points; }
	void SetPoints(float NewValue) { Points = NewValue; }

	// Setter for HasMaxSpeed variable
	void SetHasMaxSpeed(bool NewValue) { HasMaxSpeed = NewValue; }

	// Setter for MaxHealth
	void SetCharacterMaxHealth(float NewValue) { MaxHealth = NewValue; }

	// Switch the player's current primary weapon
	void SwitchToNextPrimaryWeapon(); // used in baseweapon

	// Used in Grandad
	void HandleCharacterDeath();

	UFUNCTION(BlueprintCallable)
	void MaxAmmo(); // used in bp_baseammo

	// Get a reference to the buyable item being overlapped
	UPROPERTY(EditAnywhere, Category = "References")
	ABuyableItem *OverlappingBuyableItem; // needs to be accessed in other classes

	// gets main widget reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TSubclassOf<UUserWidget> WidgetClass;
	UMainWidget *MainWidgetInstance; // needs to be accessed in other classes

	// index of the weapon the player is currently using
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int CurrentWeaponIndex = static_cast<int>(CurrentWeaponID); // used in other classes

	// primary weapons
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<ABaseWeapon *> Weapons; // used in other classes

protected:
	virtual void BeginPlay();

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Mesh)
	USkeletalMeshComponent *Mesh1P;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Mesh)
	USkeletalMeshComponent *GunMesh;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Camera)
	float TurnRateGamepad = 45.f;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent *FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent *AnimationCameraComponent;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
	USoundBase *OutOfAmmoSound; // left in protected as the sound is set in blueprint

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType EquippedWeaponCharacter; // left in protected as it's used in the abp

	// This variable tracks the players current state
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "State")
	ECharacterState CurrentState = ECharacterState::Idle;

	UFUNCTION(BlueprintCallable, Category = "State")
    bool ChangeCharacterState(ECharacterState NewState);

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boolean)
	// bool IsReloading = false; // left in protected as it's used in the abp

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boolean)
	// bool IsShooting = false; // left in protected as it's used in the abp

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boolean)
	// bool IsAiming = false; // left in protected as it's used in the abp

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boolean)
	// bool IsSwappingWeapon = false; // left in protected as it's used in the abp

	// variable needs to be changed in maxspeed class use a setter
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Boolean)
	bool HasMaxSpeed = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Health")
	float Health = 100;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Points)
	int Points = 1000;

private:
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

	// starts timer, if completed will call
	void StartReload();

	void RefillAmmo(); // This is called after the reloadmontage is over inside StartReload

	void OnInteractingPressed();

	void WeaponSwapAfterDelay();

	

	void RegenerateHealth();

	// The virtual here shows that I intend to override the function, it overrides the virtual function from the base class, i.e. APawn
	virtual float TakeDamage(float const DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;

	// Timers

	// The time for this is the fire-rate of the weapon, used in the weapon classes
	FTimerHandle FireTimerHandle; // Not used with delegate

	// reload timer is used locally
	FTimerHandle ReloadTimerHandle;

	// weapon swap delay timer
	const float WeaponSwapDelay = 0.7;
	FTimerHandle WeaponSwapTimerHandle; // Not used with delegate

	// health regen timer
	FTimerDelegate HealthRegenTimerDelegate;
	FTimerHandle HealthRegenTimerHandle;
	const float HealthRegenDuration = 5.0;

	// // variables
	TArray<int> AmmoArray; // Array of ammo for weapons

	EWeaponType CurrentWeaponID;

	bool IsDead = false;
};

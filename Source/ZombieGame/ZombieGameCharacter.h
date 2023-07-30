// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "BaseWeapon.h"
#include "BaseAmmo.h"
#include "Animation/AnimSequence.h"
#include "Engine/SkeletalMesh.h"
#include "MainWidget.h"
#include "PerkMachine.h"
#include "BuyableItem.h"
#include "ZombieGameCharacter.generated.h"

class USkeletalMesh;
class UCameraComponent;
class USoundBase;
class UNiagaraSystem;
class UMaterialInterface;


UCLASS(config=Game)
class AZombieGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AZombieGameCharacter();

	// The virtual here shows that I intend to override the function, it overrides the virtual function from the base class, i.e. APawn
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Mesh)
	USkeletalMeshComponent* GunMesh;

	/** First person camera */ 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	UCameraComponent* AnimationCameraComponent;

	UPROPERTY(EditAnywhere)
	ABuyableItem* OverlappingBuyableItem;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

	// Determines if the character is currently sprinting
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool IsAiming;

	// maximum range
	UPROPERTY(EditAnywhere)
	float MaxBulletRange = 2000;

	UPROPERTY(EditAnywhere)
	float MaxInteractRange = 150;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 100;

	UPROPERTY(EditDefaultsOnly)
	float MaxHealth = 100;

	// array iterates on the value of the Enum for weapons: EWeaponType
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<int> AmmoArray;

	EWeaponType CurrentWeaponID;
	// index of the weapon the player is currently using
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int CurrentWeaponIndex = static_cast<int>(CurrentWeaponID);

	// primary weapons 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TArray<ABaseWeapon*> Weapons;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsReloading;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "State")
	bool IsShooting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Points = 1000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool FastWalking = false;

	UPROPERTY(EditAnywhere, Category = "Impact Effect")
	UParticleSystem* ImpactEffect;

	UPROPERTY(EditAnywhere, Category = "Impact Effect")
	UParticleSystem* FireImpactEffect;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	UNiagaraSystem* HeadshotFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	UNiagaraSystem* BodyShotFX;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "FX")
	UMaterialInterface* BulletHole;

	// Switch the player's current primary weapon
	UFUNCTION(BlueprintCallable, Category = "Weapon")
	void SwitchToNextPrimaryWeapon();

	// meshes of weapons
	UPROPERTY(EditDefaultsOnly, Category = "Weapons")
    TArray<USkeletalMesh*> WeaponMeshes;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapons")
    EWeaponType EquippedWeaponCharacter;

	UFUNCTION(BlueprintCallable)
	void Death();

	UFUNCTION(BlueprintCallable)
	void RegenerateHealth();

	FTimerDelegate HealthRegenTimerDelegate;
	FTimerHandle HealthRegenTimerHandle;
	float HealthRegenDuration = 5.0; 

	UFUNCTION(BlueprintImplementableEvent)
	void BloodOverlay();

	UFUNCTION(BlueprintImplementableEvent)
	void HideBlood();

	UFUNCTION(BlueprintImplementableEvent)
	void PlayAmmoSound();

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category=Gameplay)
	USoundBase* OutOfAmmoSound;

	// max ammo 
	UFUNCTION(BlueprintCallable)
	void MaxAmmo();

	// The firing timer handle
	FTimerHandle FireTimerHandle;
	FHitResult Hit;

	EPhysicalSurface TempSurface;

	// Determines if the character is currently swapping weapon
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	bool IsSwappingWeapon = false;

	void WeaponSwapAfterDelay();

	FTimerHandle WeaponSwapTimerHandle;

	// gets main widget reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUserWidget> WidgetClass;
	UMainWidget* MainWidgetInstance;

protected:
	virtual void BeginPlay();

	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	void MoveForward(float Val);
	void MoveRight(float Val);

	void TurnAtRate(float Rate);
	void LookUpAtRate(float Rate);

	void ZoomIn();
	void ZoomOut();

	void Fire();
	void StartFiring();
	void StopFiring();
	
	void ManualReload();
	void ReloadWeapon(EWeaponType _WeaponType);

	void CalculateAmmo();

	FTimerHandle ReloadTimerHandle;

	void OnInteractingPressed();
	

public:
	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }

	USkeletalMeshComponent* GetGunMesh() const { return GunMesh; }

	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }


private:
	// IInteractionInterface* Interface = nullptr;

};


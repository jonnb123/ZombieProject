// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieGameCharacter.h"
#include "ZombieGameProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include "Kismet/GameplayStatics.h"
#include "PhysicalMaterials/PhysicalMaterial.h"
#include "Zombie.h"
#include "Engine/SkeletalMesh.h"
#include "NiagaraFunctionLibrary.h"
#include "Misc/CString.h"

//////////////////////////////////////////////////////////////////////////
// AZombieGameCharacter

AZombieGameCharacter::AZombieGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Create a CameraComponent
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// animation camera component
	AnimationCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("AnimationCamera"));
	AnimationCameraComponent->SetupAttachment(GetCapsuleComponent());
	AnimationCameraComponent->SetRelativeLocation(FVector((-78.296213,-0.000000,67.275858))); // Position the camera

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun Mesh"));
	GunMesh->SetOnlyOwnerSee(true);
	GunMesh->SetupAttachment(Mesh1P);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	// Attach GunMeshNEW to Mesh1P using a specific socket name
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	GunMesh->AttachToComponent(Mesh1P, AttachmentRules, TEXT("PistolSocket"));

	IsAiming = false;

	// CurrentWeaponIndex = 0;

	IsShooting = false;

	// Initialize the WeaponAmmoArray with default values
	AmmoArray.Init(0, static_cast<int>(EWeaponType::E_Size));
}

void AZombieGameCharacter::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();
	MainWidgetInstance = CreateWidget<UMainWidget>(GetWorld(), WidgetClass);
	MainWidgetInstance->AddToViewport();

	UE_LOG(LogTemp, Warning, TEXT("Current Weapon index: %d"), CurrentWeaponIndex);
}

float AZombieGameCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) // this is called in BTTask_Attack.cpp
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); // takes the damage values from the zombie in BTTask_Attack and plugs them into the base implementation of TakeDamage
	if (IsDead == false)
	{
		// removing health regen and blood overlay for test
		if (Health <= 90)
		{
			// HealthRegenTimer();
			HealthRegenTimerDelegate.BindUObject(this, &AZombieGameCharacter::RegenerateHealth);
			// this basically plays the ReloadCalcAndPlayAnimations once the animation is complete.
			GetWorldTimerManager().SetTimer(HealthRegenTimerHandle, HealthRegenTimerDelegate, HealthRegenDuration, true);
		}
		if (Health <= 50)
		{
			// BloodOverlay();
			if (MainWidgetInstance)
			{
				MainWidgetInstance->ShowBloodOverlay();
			}
		}
		else
		{
			// HideBlood();
			if (MainWidgetInstance)
			{
				MainWidgetInstance->ShowBloodOverlay();
			}
		}
		DamageToApply = FMath::Min(Health, DamageToApply);
		Health -= DamageToApply; // deducts damage from health
		// if alive
		if (Health > 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);
			return DamageToApply;
		}
		// when health is below 0
		UE_LOG(LogTemp, Warning, TEXT("Health left %f"), Health);
		Death();
		this->SetActorEnableCollision(false);
		return DamageToApply;
	}
	return DamageToApply; // DamageToApply just needs to be in function, not sure why
}

void AZombieGameCharacter::Death()
{
	IsDead = true;
	MainWidgetInstance->ShowDeathWindow();
	APlayerController *PlayerController = Cast<APlayerController>(GetController());
	GetCharacterMovement()->StopMovementImmediately();
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->bShowMouseCursor = true;
}

void AZombieGameCharacter::SwitchToNextPrimaryWeapon()
{
	GetWorldTimerManager().ClearTimer(ReloadTimerHandle); // if the user switches weapon whilst reloading, the reload animation will not carry over to the next weapon.
	IsReloading = false;
	bool Success = false;

	for (int i = 0; i < Weapons.Num(); i++)
	{
		if (i > CurrentWeaponIndex)
		{
			if (Weapons[i]->IsObtained)
			{
				Success = true;
				CurrentWeaponIndex = i;
				UE_LOG(LogTemp, Warning, TEXT("Current Weapon index: %d"), CurrentWeaponIndex);
				GunMesh->SetSkeletalMesh(Weapons[CurrentWeaponIndex]->WeaponMesh);

				// Attach GunMesh to the new socket
				GunMesh->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, Weapons[CurrentWeaponIndex]->SocketName);

				// This is referenced in the abp to change weapon
				EquippedWeaponCharacter = Weapons[CurrentWeaponIndex]->WeaponType;
			}
			break; // breaks out of the for loop
		}
	}

	if (!Success) // for the pistol
	{
		CurrentWeaponIndex = 0;
		UE_LOG(LogTemp, Warning, TEXT("Current Weapon index: %d"), CurrentWeaponIndex);

		// SwitchWeaponMesh(CurrentWeaponIndex);
		GunMesh->SetSkeletalMesh(Weapons[CurrentWeaponIndex]->WeaponMesh);

		GunMesh->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, Weapons[CurrentWeaponIndex]->SocketName);

		EquippedWeaponCharacter = Weapons[CurrentWeaponIndex]->WeaponType;
	}
}

void AZombieGameCharacter::MaxAmmo()
{
	for (int i = 0; i < Weapons.Num(); i++)
	{
		Weapons[i]->TotalAmmo = Weapons[i]->MaximumAmmo;
	}
}

//////////////////////////////////////////////////////////////////////////// Input

void AZombieGameCharacter::OnInteractingPressed()
{
	PerkMachineInteract();
	WeaponPickupInteract();
}

void AZombieGameCharacter::WeaponPickupInteract()
{
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	
	if (OverlappingWeapon)
	{
		if (OverlappingWeapon->Overlapping == true && OverlappingWeapon->IsObtained == false && Points >= FCString::Atoi(*OverlappingWeapon->ItemPrice))
		{
			Points = Points - FCString::Atoi(*OverlappingWeapon->ItemPrice);
			Mesh1P->PlayAnimation(WeaponPickupAnimation, false);
			// Set a timer for 1.5 seconds and specify the function to be called after the delay
			GetWorldTimerManager().SetTimer(WeaponPickupTimerHandle, this, &AZombieGameCharacter::WeaponPickupAfterDelay, 1.5f, false);
		}
	}
}

void AZombieGameCharacter::WeaponPickupAfterDelay()
{
	Mesh1P->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	OverlappingWeapon->IsObtained = true;
	Weapons.AddUnique(OverlappingWeapon);
	if (Weapons.Num()-1 != -1) // if the newly added weapons index isn't -1
	{
		SwitchToNextPrimaryWeapon();
	}
}

void AZombieGameCharacter::PerkMachineInteract()
{
	if (OverlappingPerkMachine)
	{
		OverlappingPerkMachine->UsePerk();
	}
}

void AZombieGameCharacter::SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind Interact events
	PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AZombieGameCharacter::OnInteractingPressed);

	// Bind fire event
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &AZombieGameCharacter::StartFiring);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &AZombieGameCharacter::StopFiring);

	// Bind fire event
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AZombieGameCharacter::ManualReload);

	// Aiming
	PlayerInputComponent->BindAction("Aim", IE_Pressed, this, &AZombieGameCharacter::ZoomIn);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &AZombieGameCharacter::ZoomOut);

	// Bind weapon switch events
	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this, &AZombieGameCharacter::SwitchToNextPrimaryWeapon);

	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &AZombieGameCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &AZombieGameCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &AZombieGameCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &AZombieGameCharacter::LookUpAtRate);
}

// void AZombieGameCharacter::OnPrimaryAction()
// {
// 	// Trigger the OnItemUsed Event
// 	OnUseItem.Broadcast();
// }

void AZombieGameCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AZombieGameCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AZombieGameCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AZombieGameCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void AZombieGameCharacter::ZoomIn()
{
	MainWidgetInstance->RemoveFromViewport();
	// uses UCharacterMovementComponent
	if (this->GetCharacterMovement()->MaxWalkSpeed == 1200.0f)
	{
		FastWalking = true;
		this->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		IsAiming = true;
	}

	else
	{
		this->GetCharacterMovement()->MaxWalkSpeed = 200.0f;
		IsAiming = true;
	}
}

void AZombieGameCharacter::ZoomOut()
{
	MainWidgetInstance->AddToViewport();
	if (FastWalking == true)
	{
		this->GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
		IsAiming = false;
	}
	else
	{
		this->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		IsAiming = false;
	}
}

void AZombieGameCharacter::Fire()
{
	UE_LOG(LogTemp, Warning, TEXT("Current ammo: %d"), Weapons[CurrentWeaponIndex]->CurrentAmmo);

	if (IsReloading)
	{
		UE_LOG(LogTemp, Log, TEXT("RELOADINGGGGG"));
	}

	if (IsShooting)
	{
		UE_LOG(LogTemp, Warning, TEXT("Shooting!!!!!!"));
		if (Weapons[CurrentWeaponIndex]->CurrentAmmo > 0)
		{
			if (IsReloading == false)
			{
				Weapons[CurrentWeaponIndex]->CurrentAmmo--;
				// PlayFiringAnimations();
				GunMesh->PlayAnimation(Weapons[CurrentWeaponIndex]->WeaponFireMontage, false);
				FVector Location;
				FRotator Rotation;
				// FHitResult Hit;
				GetController()->GetPlayerViewPoint(Location, Rotation);
				FVector Start = Location;
				FVector End = Location + Rotation.Vector() * MaxBulletRange;

				FCollisionQueryParams TraceParams = FCollisionQueryParams::DefaultQueryParam;
				TraceParams.bReturnPhysicalMaterial = true;
				TraceParams.AddIgnoredActor(this);

				bool bSuccess = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_WorldDynamic, TraceParams);

				if (bSuccess)
				{
					FVector ShotDirection = -Rotation.Vector();

					AActor *HitActor = Hit.GetActor();
					if (HitActor != nullptr) // very important nullptr check, was breaking randomly without this.
					{
						UE_LOG(LogTemp, Log, TEXT("actor is %s"), *HitActor->GetName());
						// Get hit surface type
						TempSurface = UGameplayStatics::GetSurfaceType(Hit);

						UE_LOG(LogTemp, Log, TEXT("The surface is %s"), *UEnum::GetValueAsString(TempSurface));
						if (HitActor != nullptr)
						{
							if (TempSurface == SurfaceType1) // for headshots on zombie
							{
								FPointDamageEvent DamageEvent(HeadDamage, Hit, ShotDirection, nullptr);
								HitActor->TakeDamage(HeadDamage, DamageEvent, GetInstigatorController(), this); // this calls the takedamage function in zombie.cpp when the zombie is hit
								UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), HeadshotFX, Hit.Location, ShotDirection.Rotation());
								Points += 50;
							}
							else if (TempSurface == SurfaceType2) // for body shots on zombie
							{
								UE_LOG(LogTemp, Log, TEXT("Surface 2"));
								FPointDamageEvent DamageEvent(BodyDamage, Hit, ShotDirection, nullptr);
								HitActor->TakeDamage(BodyDamage, DamageEvent, GetInstigatorController(), this);
								UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BodyShotFX, Hit.Location, ShotDirection.Rotation());
								Points += 10;
							}
							else if (TempSurface == SurfaceType3) // for the fire boss
							{
								UE_LOG(LogTemp, Log, TEXT("Entered Fireboss loop"));
								FPointDamageEvent DamageEvent(BodyDamage, Hit, ShotDirection, nullptr);
								HitActor->TakeDamage(BodyDamage, DamageEvent, GetInstigatorController(), this);
								UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireImpactEffect, Hit.Location, ShotDirection.Rotation());
								Points += 10;
							}
							else
							{
								UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
								UGameplayStatics::SpawnDecalAtLocation(GetWorld(), BulletHole, FVector(15, 15, 15), Hit.Location, ShotDirection.Rotation());
							}
						}
					}
				}
				GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AZombieGameCharacter::Fire, Weapons[CurrentWeaponIndex]->FireRate, false); // to make the weapon fully auto
			}
			else
			{
				// ReloadWeapon(Weapons[CurrentWeaponIndex]->WeaponType); this was for autoreload but made the animations not work
			}
		}
		else
		{
			// if we don't have any ammo in the gun, make a click noise
			UGameplayStatics::PlaySoundAtLocation(this, OutOfAmmoSound, this->GetActorLocation());
		}
	}
}

void AZombieGameCharacter::StartFiring()
{
	IsShooting = true;
	Fire();
}

void AZombieGameCharacter::StopFiring()
{
	IsShooting = false;
	FireTimerHandle.Invalidate();
}

void AZombieGameCharacter::ManualReload()
{
	if (Weapons[CurrentWeaponIndex] && Weapons[CurrentWeaponIndex]->CurrentAmmo != Weapons[CurrentWeaponIndex]->MaxClipSize) // if the player has a weapon and they don't have a full clip
	{
		UE_LOG(LogTemp, Warning, TEXT("Max Clip Size: %d"), Weapons[CurrentWeaponIndex]->TotalAmmo);

		ReloadWeapon(Weapons[CurrentWeaponIndex]->WeaponType);
	}
}

void AZombieGameCharacter::ReloadWeapon(EWeaponType _WeaponType)
{
	if (Weapons[CurrentWeaponIndex]->TotalAmmo != 0)
	{
		float MontageDuration = Weapons[CurrentWeaponIndex]->WeaponReloadMontage->GetPlayLength();
		float TimerDuration = MontageDuration - 0.2; // Skip the last 0.2second
		GunMesh->PlayAnimation(Weapons[CurrentWeaponIndex]->WeaponReloadMontage, false);
		IsReloading = true;
		// A delegate is created and is binded to the member function.
		FTimerDelegate TimerDelegate;
		TimerDelegate.BindUObject(this, &AZombieGameCharacter::CalculateAmmo);
		// this basically plays the ReloadCalcAndPlayAnimations once the animation is complete.
		GetWorldTimerManager().SetTimer(ReloadTimerHandle, TimerDelegate, TimerDuration, false);
	}
}

void AZombieGameCharacter::CalculateAmmo()
{
	UE_LOG(LogTemp, Log, TEXT("Switching Mags!"));
	if (Weapons[CurrentWeaponIndex]->CurrentAmmo == Weapons[CurrentWeaponIndex]->MaxClipSize || Weapons[CurrentWeaponIndex]->TotalAmmo <= 0)
	{
		// if the currentammo in the weapon is at the max clip size or is less than or the gun has no reserve ammo
	}
	else
	{
		// current ammo is the current ammo in the clip
		// Needed ammo is the ammount of ammo needed to make a full clip
		int NeededAmmo = Weapons[CurrentWeaponIndex]->MaxClipSize - Weapons[CurrentWeaponIndex]->CurrentAmmo;
		if (Weapons[CurrentWeaponIndex]->TotalAmmo >= NeededAmmo)
		{
			Weapons[CurrentWeaponIndex]->CurrentAmmo = Weapons[CurrentWeaponIndex]->CurrentAmmo + NeededAmmo; // adds the ammo needed for a full clip
			Weapons[CurrentWeaponIndex]->TotalAmmo = Weapons[CurrentWeaponIndex]->TotalAmmo - NeededAmmo;	  // deducts ammo added to clip
		}
		else if (Weapons[CurrentWeaponIndex]->TotalAmmo > 0) // if the ammo amount is less than the needed ammo
		{
			Weapons[CurrentWeaponIndex]->CurrentAmmo = Weapons[CurrentWeaponIndex]->CurrentAmmo + Weapons[CurrentWeaponIndex]->TotalAmmo;
			Weapons[CurrentWeaponIndex]->TotalAmmo = 0;
		}
	}
	AmmoArray[CurrentWeaponIndex] = Weapons[CurrentWeaponIndex]->TotalAmmo;
	IsReloading = false;
}






void AZombieGameCharacter::RegenerateHealth()
{
	if (Health == 100)
	{
		GetWorldTimerManager().ClearTimer(HealthRegenTimerHandle);
	}
	else
	{
		Health += 5;
		Health = FMath::Clamp(Health, 0.f, 100.f);
	}
	if (Health >= 50)
	{
		MainWidgetInstance->HideBloodEffect();
	}
}
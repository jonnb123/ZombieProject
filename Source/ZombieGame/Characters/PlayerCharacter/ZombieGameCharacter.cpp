// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/Image.h"
#include "Kismet/GameplayStatics.h"

AZombieGameCharacter::AZombieGameCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// animation camera component
	AnimationCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("AnimationCamera"));
	AnimationCameraComponent->SetupAttachment(GetCapsuleComponent());
	AnimationCameraComponent->SetRelativeLocation(FVector((-78.296213, -0.000000, 67.275858))); // Position the camera

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
	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	GunMesh->AttachToComponent(Mesh1P, AttachmentRules, TEXT("PistolSocket"));

	// Initialize the WeaponAmmoArray with default values
	AmmoArray.Init(0, static_cast<int>(EWeaponType::E_Size));
}

void AZombieGameCharacter::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();
	MainWidgetInstance = CreateWidget<UMainWidget>(GetWorld(), WidgetClass);
	MainWidgetInstance->AddToViewport();
}

void AZombieGameCharacter::HandleDamage(float const DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) // this is called in BTTask_Attack.cpp
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); // takes the damage values from the zombie in BTTask_Attack and plugs them into the base implementation of TakeDamage
	if (Health <= 0)
	{
		// when health is below 0
		UE_LOG(LogTemp, Log, TEXT("Health left %f"), Health);
		HandleCharacterDeath();
	}
	else // player is alive
	{
		// removing health regen and blood overlay for test
		if (Health <= 0.5f * MaxHealth)
		{
			check(MainWidgetInstance != nullptr);
			MainWidgetInstance->ShowBloodOverlay();
		}
		else if (Health <= 1.0f * MaxHealth)
		{
			// HealthRegenTimer();
			HealthRegenTimerDelegate.BindUObject(this, &AZombieGameCharacter::RegenerateHealth);
			// this basically plays the ReloadCalcAndPlayAnimations once the animation is complete.
			GetWorldTimerManager().SetTimer(HealthRegenTimerHandle, HealthRegenTimerDelegate, HealthRegenDuration, true);
		}
		DamageToApply = FMath::Min(Health, DamageToApply);
		Health -= DamageToApply; // deducts damage from health
		UE_LOG(LogTemp, Log, TEXT("Health left %f"), Health);
	}
}

void AZombieGameCharacter::HandleCharacterDeath()
{
	IsDead = true;
	MainWidgetInstance->ShowDeathWindow();
	APlayerController *PlayerController = Cast<APlayerController>(GetController());
	GetCharacterMovement()->StopMovementImmediately();
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->bShowMouseCursor = true;
	SetActorEnableCollision(false);
}

void AZombieGameCharacter::SwitchToNextPrimaryWeapon()
{
	if (ChangeCharacterState(ECharacterState::SwappingWeapon))
	{
		GetWorldTimerManager().ClearTimer(ReloadTimerHandle); // if the user switches weapon whilst reloading, the reload animation will not carry over to the next weapon.
		bool Success = false;

		for (int i = 0; i < Weapons.Num(); i++)
		{
			if (i > CurrentWeaponIndex)
			{
				if (Weapons[i]->IsObtained)
				{
					GetWorldTimerManager().SetTimer(WeaponSwapTimerHandle, this, &AZombieGameCharacter::WeaponSwapAfterDelay, WeaponSwapDelay, false);
					Success = true;
					CurrentWeaponIndex = i;
					UE_LOG(LogTemp, Display, TEXT("Current Weapon index: %d"), CurrentWeaponIndex);
					GunMesh->SetSkeletalMesh(Weapons[CurrentWeaponIndex]->WeaponMesh);
					// Attach GunMesh to the new socket
					GunMesh->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, Weapons[CurrentWeaponIndex]->WeaponSocketName);
					// This is referenced in the abp to change weapon
					EquippedWeaponCharacter = Weapons[CurrentWeaponIndex]->WeaponType;
				}
				break; // breaks out of the for loop
			}
		}

		if (!Success) // for the pistol
		{
			CurrentWeaponIndex = 0;
			UE_LOG(LogTemp, Display, TEXT("Current Weapon index: %d"), CurrentWeaponIndex);
			ChangeCharacterState(ECharacterState::Idle);
			GunMesh->SetSkeletalMesh(Weapons[CurrentWeaponIndex]->WeaponMesh);
			GunMesh->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale, Weapons[CurrentWeaponIndex]->WeaponSocketName);
			EquippedWeaponCharacter = Weapons[CurrentWeaponIndex]->WeaponType;
		}
	}
}

void AZombieGameCharacter::WeaponSwapAfterDelay()
{
	ChangeCharacterState(ECharacterState::Idle);
}

void AZombieGameCharacter::MaxAmmo()
{
	for (int i = 0; i < Weapons.Num(); i++)
	{
		Weapons[i]->TotalWeaponAmmo = Weapons[i]->MaxWeaponAmmo;
	}
}

void AZombieGameCharacter::OnInteractingPressed()
{
	if (OverlappingBuyableItem)
	{
		OverlappingBuyableItem->UseBuyableItem();
	}

	if (OverlappingFrontDoor)
	{
		OverlappingFrontDoor->UseFrontDoor();
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
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &AZombieGameCharacter::StartReload);

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
	if (ChangeCharacterState(ECharacterState::Aiming))
	{
		MainWidgetInstance->Crosshair->SetVisibility(ESlateVisibility::Hidden);
		if (HasMaxSpeed)
		{
			GetCharacterMovement()->MaxWalkSpeed = 400.0f;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 200.f;
		}
	}
}

void AZombieGameCharacter::ZoomOut()
{
	if (ChangeCharacterState(ECharacterState::Idle))
	{
		MainWidgetInstance->Crosshair->SetVisibility(ESlateVisibility::Visible);
		
		if (HasMaxSpeed)
		{
			GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 600.0f;
		}
	}
}

void AZombieGameCharacter::Fire()
{
	UE_LOG(LogTemp, Log, TEXT("Current ammo: %d"), Weapons[CurrentWeaponIndex]->CurrentWeaponAmmo);

	if (Weapons[CurrentWeaponIndex]->CurrentWeaponAmmo > 0)
	{
		Weapons[CurrentWeaponIndex]->CurrentWeaponAmmo--;
		GunMesh->PlayAnimation(Weapons[CurrentWeaponIndex]->WeaponFireMontage, false);

		// Location and Rotation
		const APlayerController *PlayerController = Cast<APlayerController>(GetController());
		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		// Added a fire socket where the bullet will come from
		const FVector MuzzleLocation = GunMesh->GetSocketLocation(TEXT("FireSocket"));

		// Set Spawn Collision
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// Spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AZombieGameProjectile>(Weapons[CurrentWeaponIndex]->ProjectileClass, MuzzleLocation, SpawnRotation, ActorSpawnParams);
		GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AZombieGameCharacter::Fire, Weapons[CurrentWeaponIndex]->WeaponFireRate, false); // to make the weapon fully auto
	}
	else
	{
		// if we don't have any ammo in the gun, make a click noise
		UGameplayStatics::PlaySoundAtLocation(this, OutOfAmmoSound, this->GetActorLocation());
	}
}

void AZombieGameCharacter::StartFiring() // rename
{
	if (ChangeCharacterState(ECharacterState::AimFiring))
	{
		Fire();
	}
	else if (ChangeCharacterState(ECharacterState::Firing))
	{
		Fire();
	}

}

void AZombieGameCharacter::StopFiring()
{
	if (CurrentState == ECharacterState::Reloading)
	{
		GetWorldTimerManager().ClearTimer(FireTimerHandle);
	}

	else if (ChangeCharacterState(ECharacterState::Aiming))
	{
		GetWorldTimerManager().ClearTimer(FireTimerHandle);
	}

	else if (ChangeCharacterState(ECharacterState::Idle))
	{
		GetWorldTimerManager().ClearTimer(FireTimerHandle);
	}
}

void AZombieGameCharacter::StartReload()
{
	if (Weapons[CurrentWeaponIndex]->TotalWeaponAmmo != 0 && Weapons[CurrentWeaponIndex]->CurrentWeaponAmmo != Weapons[CurrentWeaponIndex]->MaxWeaponClipSize)
	{
		if (ChangeCharacterState(ECharacterState::Reloading))
		{
			const float MontageDuration = Weapons[CurrentWeaponIndex]->WeaponReloadMontage->GetPlayLength();
			const float TimerDuration = MontageDuration - 0.2; // Skip the last 0.2second
			GunMesh->PlayAnimation(Weapons[CurrentWeaponIndex]->WeaponReloadMontage, false);
			// A delegate is created and is binded to the member function.
			FTimerDelegate TimerDelegate;
			TimerDelegate.BindUObject(this, &AZombieGameCharacter::RefillAmmo);
			// this basically plays the ReloadCalcAndPlayAnimations once the animation is complete.
			GetWorldTimerManager().SetTimer(ReloadTimerHandle, TimerDelegate, TimerDuration, false);
		}
	}
}

void AZombieGameCharacter::RefillAmmo()
{
	UE_LOG(LogTemp, Log, TEXT("Switching Mags!"));
	if (Weapons[CurrentWeaponIndex]->CurrentWeaponAmmo == Weapons[CurrentWeaponIndex]->MaxWeaponClipSize || Weapons[CurrentWeaponIndex]->TotalWeaponAmmo <= 0)
	{
		// if the CurrentWeaponAmmo in the weapon is at the max clip size or is less than or the gun has no reserve ammo
	}
	else
	{
		// current ammo is the current ammo in the clip
		// Needed ammo is the ammount of ammo needed to make a full clip
		const int NeededAmmo = Weapons[CurrentWeaponIndex]->MaxWeaponClipSize - Weapons[CurrentWeaponIndex]->CurrentWeaponAmmo;
		if (Weapons[CurrentWeaponIndex]->TotalWeaponAmmo >= NeededAmmo)
		{
			Weapons[CurrentWeaponIndex]->CurrentWeaponAmmo = Weapons[CurrentWeaponIndex]->CurrentWeaponAmmo + NeededAmmo; // adds the ammo needed for a full clip
			Weapons[CurrentWeaponIndex]->TotalWeaponAmmo = Weapons[CurrentWeaponIndex]->TotalWeaponAmmo - NeededAmmo;	  // deducts ammo added to clip
		}
		else if (Weapons[CurrentWeaponIndex]->TotalWeaponAmmo > 0) // if the ammo amount is less than the needed ammo
		{
			Weapons[CurrentWeaponIndex]->CurrentWeaponAmmo = Weapons[CurrentWeaponIndex]->CurrentWeaponAmmo + Weapons[CurrentWeaponIndex]->TotalWeaponAmmo;
			Weapons[CurrentWeaponIndex]->TotalWeaponAmmo = 0;
		}
	}
	AmmoArray[CurrentWeaponIndex] = Weapons[CurrentWeaponIndex]->TotalWeaponAmmo;
	ChangeCharacterState(ECharacterState::Idle);
}

void AZombieGameCharacter::RegenerateHealth()
{
	if (Health == MaxHealth)
	{
		GetWorldTimerManager().ClearTimer(HealthRegenTimerHandle);
	}
	else
	{
		Health += 5;
		Health = FMath::Clamp(Health, 0.f, MaxHealth);
	}
	if (Health >= 0.5f * MaxHealth)
	{
		MainWidgetInstance->HideBloodEffect();
	}
}

bool AZombieGameCharacter::ChangeCharacterState(ECharacterState NewState)
{
	bool CanChange = false; // replace with an if

	switch (NewState)
	{
	case ECharacterState::Idle:
		CanChange = true; // Allow transitioning to Idle state from any state
		break;
	case ECharacterState::Firing:
		CanChange = ((CurrentState == ECharacterState::Idle || CurrentState == ECharacterState::Aiming) && (CurrentState != ECharacterState::Reloading));
		break;
	case ECharacterState::SwappingWeapon:
		CanChange = true;
		break;
	case ECharacterState::Aiming:
		CanChange = (CurrentState == ECharacterState::Idle || CurrentState == ECharacterState::AimFiring);
		break;
	case ECharacterState::AimFiring:
		CanChange = (CurrentState == ECharacterState::Aiming);
		break;
	case ECharacterState::Reloading:
		CanChange = (CurrentState != ECharacterState::Reloading);
		break;
	}

	if (CanChange)
	{
		CurrentState = NewState;
	}

	return CanChange;
}

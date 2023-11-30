// Copyright Epic Games, Inc. All Rights Reserved.

#include "ZombieGameCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "States/AimingState.h"
#include "States/FireState.h"
#include "States/IdleState.h"
#include "States/ReloadingState.h"
#include "States/SwappingWeaponState.h"

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

	TryStateInstance = NewObject<UIdleState>(this);
	TryStateInstance->TryEnterState(this);
}

void AZombieGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
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
	PlayerInputComponent->BindAction("SwitchWeapon", IE_Pressed, this,
	                                 &AZombieGameCharacter::SwitchToNextPrimaryWeapon);

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
	TryStateInstance = NewObject<UAimingState>(this);
	TryStateInstance->TryEnterState(this);
}

void AZombieGameCharacter::ZoomOut()
{
	TryStateInstance = NewObject<UAimingState>(this);
	TryStateInstance->TryExitState(this);
}

void AZombieGameCharacter::StartFiring() // rename
{
	TryStateInstance = NewObject<UFireState>(this);
	TryStateInstance->TryEnterState(this);
}

void AZombieGameCharacter::StopFiring()
{
	TryStateInstance = NewObject<UFireState>(this);
	TryStateInstance->TryExitState(this);
}

void AZombieGameCharacter::StartReload()
{
	TryStateInstance = NewObject<UReloadingState>(this);
	TryStateInstance->TryEnterState(this);
}

void AZombieGameCharacter::SwitchToNextPrimaryWeapon()
{
	TryStateInstance = NewObject<USwappingWeaponState>(this);
	TryStateInstance->TryEnterState(this);
}

void AZombieGameCharacter::MaxAmmo()
{
	for (int i = 0; i < Weapons.Num(); i++)
	{
		Weapons[i]->TotalWeaponAmmo = Weapons[i]->MaxWeaponAmmo;
	}
}

void AZombieGameCharacter::HandleCharacterDeath()
{
	IsDead = true;
	MainWidgetInstance->ShowDeathWindow();
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	GetCharacterMovement()->StopMovementImmediately();
	PlayerController->SetInputMode(FInputModeUIOnly());
	PlayerController->bShowMouseCursor = true;
	SetActorEnableCollision(false);
}


void AZombieGameCharacter::HandleDamage(float const DamageAmount, struct FDamageEvent const& DamageEvent,
										class AController* EventInstigator,
										AActor* DamageCauser) // this is called in BTTask_Attack.cpp
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	// takes the damage values from the zombie in BTTask_Attack and plugs them into the base implementation of TakeDamage
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
			GetWorldTimerManager().SetTimer(HealthRegenTimerHandle, HealthRegenTimerDelegate, HealthRegenDuration,
											true);
		}
		DamageToApply = FMath::Min(Health, DamageToApply);
		Health -= DamageToApply; // deducts damage from health
		UE_LOG(LogTemp, Log, TEXT("Health left %f"), Health);
		}
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
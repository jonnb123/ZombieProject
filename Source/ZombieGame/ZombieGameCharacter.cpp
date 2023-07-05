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
#include "NiagaraFunctionLibrary.h"


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

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));


	IsAiming = false;

	WeaponIndex = 0;

	AssaultRifleAmmo = 180;
	PistolAmmo = 30;
	ShotgunAmmo = 16;

	IsShooting = false;

}

float AZombieGameCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) // this is called in BTTask_Attack.cpp
{
	float DamageToApply = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); // takes the damage values from the zombie in BTTask_Attack and plugs them into the base implementation of TakeDamage
	if (IsDead == false)
	{
		// removing health regen and blood overlay for test
		if (Health <= 90)
		{
			HealthRegenTimer(); 
		}
		if (Health <= 50)
		{
			BloodOverlay();
		}
		else
		{
			HideBlood();
		}
		DamageToApply  = FMath::Min(Health, DamageToApply);
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


void AZombieGameCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
}

void AZombieGameCharacter::SwitchToNextPrimaryWeapon() 
{
	bool Success = false;
	for (int i = 0; i < Weapons.Num(); i++)
	{
		if (i > WeaponIndex)
		{
			if (Weapons[i]->IsObtained)
			{
				Success = true;
				WeaponIndex = i;
				SwitchWeaponMesh(Weapons[WeaponIndex]->Index);
				break;
			}
		}
	}

	if (!Success)
	{
		WeaponIndex = 0;
		SwitchWeaponMesh(WeaponIndex);
	}

}

// Add ammo for a specific weapon
void AZombieGameCharacter::AddAmmo(EAmmoType _AmmoType, int _AmmoAmount) 
{
	switch(_AmmoType)
	{
	case EAmmoType::E_AssaultRifle:
		AssaultRifleAmmo += _AmmoAmount;
		break;

	case EAmmoType::E_Pistol:
		PistolAmmo += _AmmoAmount;
		break;
	
	case EAmmoType::E_Shotgun:
		ShotgunAmmo += _AmmoAmount;
		break;

	default: 
		break;
	}
}

void AZombieGameCharacter::MaxAmmo()
{
	AssaultRifleAmmo = 180;
	PistolAmmo = 30;
	ShotgunAmmo = 16;
}

//////////////////////////////////////////////////////////////////////////// Input

void AZombieGameCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Bind Interact events
	// PlayerInputComponent->BindAction("Interact", IE_Pressed, this, &AZombieGameCharacter::Interact);

	PlayerInputComponent->BindAction("Interacting", IE_Pressed, this, &AZombieGameCharacter::Interacting);




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
	UE_LOG(LogTemp, Warning, TEXT("Current ammo: %d"), Weapons[WeaponIndex]->CurrentAmmo);

	if (IsShooting)
	{
		UE_LOG(LogTemp, Warning, TEXT("Shooting!!!!!!"));
		if (Weapons[WeaponIndex]->CurrentAmmo > 0)
		{
			if (IsReloading == false)
			{
				Weapons[WeaponIndex]->CurrentAmmo--;
				PlayFireAnimations(); // Blueprint logic for firing weapon
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

					AActor* HitActor = Hit.GetActor();
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
							Points+=50;
						}
						else if (TempSurface == SurfaceType2) // for body shots on zombie
						{
							UE_LOG(LogTemp, Log, TEXT("Surface 2"));
							FPointDamageEvent DamageEvent(BodyDamage, Hit, ShotDirection, nullptr);
							HitActor->TakeDamage(BodyDamage, DamageEvent, GetInstigatorController(), this);
							UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), BodyShotFX, Hit.Location, ShotDirection.Rotation());
							Points+=10;
						}
						else if (TempSurface == SurfaceType3) // for the fire bosee
						{
							UE_LOG(LogTemp, Log, TEXT("Entered Fireboss loop"));
							FPointDamageEvent DamageEvent(BodyDamage, Hit, ShotDirection, nullptr);
							HitActor->TakeDamage(BodyDamage, DamageEvent, GetInstigatorController(), this);
							Points+=10;
						}
						else
						{
							UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Hit.Location, ShotDirection.Rotation());
							UGameplayStatics::SpawnDecalAtLocation(GetWorld(),BulletHole, FVector(15,15,15), Hit.Location, ShotDirection.Rotation());
						}
					}
				}
				GetWorld()->GetTimerManager().SetTimer(FireTimerHandle, this, &AZombieGameCharacter::Fire, Weapons[WeaponIndex]->FireRate, false); // to make the weapon fully auto
			}
			else
			{
				ReloadWeapon(Weapons[WeaponIndex]->WeaponType);
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
	if (Weapons[WeaponIndex])
	{
		ReloadWeapon(Weapons[WeaponIndex]->WeaponType);
	}
}

void AZombieGameCharacter::ReloadWeapon(EWeaponType _WeaponType) 
{
	if (Weapons[WeaponIndex])
	{
		IsReloading = true;
		switch (_WeaponType)
		{
		case EWeaponType::E_AssaultRifle:
			AssaultRifleAmmo = CalculateAmmo(AssaultRifleAmmo);
			break;
		case EWeaponType::E_Pistol:
			PistolAmmo = CalculateAmmo(PistolAmmo);
			break;
		case EWeaponType::E_Shotgun:
			ShotgunAmmo = CalculateAmmo(ShotgunAmmo);
			break;
		default:
			break;
		}
	}
}

int AZombieGameCharacter::CalculateAmmo(int _AmmoAmount) 
{
	if (Weapons[WeaponIndex]->CurrentAmmo == Weapons[WeaponIndex]->MaxClipSize || _AmmoAmount <= 0)
	{

	}
	else
	{
		IsReloading = true;
		int NeededAmmo = Weapons[WeaponIndex]->MaxClipSize - Weapons[WeaponIndex]->CurrentAmmo;
		if (_AmmoAmount >= NeededAmmo)
		{
			Weapons[WeaponIndex]->CurrentAmmo = Weapons[WeaponIndex]->CurrentAmmo + NeededAmmo;
			_AmmoAmount = _AmmoAmount - NeededAmmo;
		}
		else
		{
			if(_AmmoAmount > 0)
			{
				Weapons[WeaponIndex]->CurrentAmmo = Weapons[WeaponIndex]->CurrentAmmo + _AmmoAmount;
				_AmmoAmount = 0;
			}
		}
		
		PlayReloadAnimations();

		IsReloading = false;
	}

	return _AmmoAmount;
}

void AZombieGameCharacter::Interacting() 
{
	UE_LOG(LogTemp, Warning, TEXT("boop bop"));

	FVector Location;
	FRotator Rotation; 

	FHitResult InteractHit;

	GetController()->GetPlayerViewPoint(Location, Rotation);

	FVector Start = Location;
	// FVector End = Location + Rotation.Vector() * MaxRange;
	FVector End = Location + Rotation.Vector() * 150;

			
	FCollisionQueryParams TraceParams = FCollisionQueryParams::DefaultQueryParam;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.AddIgnoredActor(this);
			
	bool bSuccess = GetWorld()->LineTraceSingleByChannel(InteractHit, Start, End, ECC_WorldDynamic, TraceParams);

	if (bSuccess)
	{
		UE_LOG(LogTemp, Warning, TEXT("Hit Actor!"));

		if(InteractHit.GetActor())
		{
			UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *InteractHit.GetActor()->GetName());
			// play consume animation 
			// PlayConsumeAnimation();
			// for health juice 
			if (InteractHit.GetActor()->GetName() == "BP_PerkMachine_C_1")
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *InteractHit.GetActor()->GetName());
				Interface = Cast<IInteractionInterface>(InteractHit.GetActor());
				if (Interface)
				{
					Interface->HealthJuice();
				}
			}
			else if (InteractHit.GetActor()->GetName() == "BP_FullyAuto_C_1")
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *InteractHit.GetActor()->GetName());
				Interface = Cast<IInteractionInterface>(InteractHit.GetActor());
				if (Interface)
				{
					Interface->FullyAuto();
				}
			}

			else if (InteractHit.GetActor()->GetName() == "BP_MaxSpeed_C_1")
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *InteractHit.GetActor()->GetName());
				Interface = Cast<IInteractionInterface>(InteractHit.GetActor());
				if (Interface)
				{
					Interface->MaxSpeed();
				}
			}

			else if (InteractHit.GetActor()->GetName() == "BP_ExtendedMag_C_1")
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *InteractHit.GetActor()->GetName());
				Interface = Cast<IInteractionInterface>(InteractHit.GetActor());
				if (Interface)
				{
					Interface->ExtendedMag();
				}
			}

			else if (InteractHit.GetActor()->GetName() == "BP_TurretShop_C_1")
			{
				UE_LOG(LogTemp, Warning, TEXT("Hit Actor: %s"), *InteractHit.GetActor()->GetName());
				Interface = Cast<IInteractionInterface>(InteractHit.GetActor());
				if (Interface)
				{
					Interface->AddTurret();
				}
			}
			
		}
	}

}








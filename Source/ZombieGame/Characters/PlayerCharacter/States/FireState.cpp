// Fill out your copyright notice in the Description page of Project Settings.


#include "FireState.h"
#include "IdleFireState.h"
#include "IdleState.h"
#include "ReloadingState.h"
#include "SwappingWeaponState.h"
#include "Kismet/GameplayStatics.h"

void UFireState::EnterState(AZombieGameCharacter* Character)
{
	Fire(Character);
}

void UFireState::TryEnterState(AZombieGameCharacter* Character)
{
	if (Character->CurrentStateInstance->IsA<UReloadingState>() || Character->CurrentStateInstance->IsA<
		USwappingWeaponState>()) return;
	if (Character->CurrentStateInstance->IsA<UIdleState>())
	{
		// This is for the ABP to go from idle to fire
		Character->CurrentStateInstance = NewObject<UIdleFireState>(this);
	}
	// don't want to enter IdleFireState->EnterState, want to have FireState->EnterState as it's for both
	// Character->CurrentStateInstance->EnterState(Character);
	
	EnterState(Character);
}

void UFireState::Fire(AZombieGameCharacter* Character)
{
	UE_LOG(LogTemp, Log, TEXT("Current ammo: %d"),
	       Character->Weapons[Character->CurrentWeaponIndex]->CurrentWeaponAmmo);

	if (Character->Weapons[Character->CurrentWeaponIndex]->CurrentWeaponAmmo > 0)
	{
		Character->Weapons[Character->CurrentWeaponIndex]->CurrentWeaponAmmo--;
		Character->GunMesh->PlayAnimation(Character->Weapons[Character->CurrentWeaponIndex]->WeaponFireMontage, false);

		// Location and Rotation
		const APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
		const FRotator SpawnRotation = PlayerController->PlayerCameraManager->GetCameraRotation();
		// Added a fire socket where the bullet will come from
		const FVector MuzzleLocation = Character->GunMesh->GetSocketLocation(TEXT("FireSocket"));

		// Set Spawn Collision
		FActorSpawnParameters ActorSpawnParams;
		ActorSpawnParams.SpawnCollisionHandlingOverride =
			ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

		// Spawn the projectile at the muzzle
		GetWorld()->SpawnActor<AZombieGameProjectile>(
			Character->Weapons[Character->CurrentWeaponIndex]->ProjectileClass, MuzzleLocation,
			SpawnRotation, ActorSpawnParams);

		if (!bIsFiring)
		{
			bIsFiring = true;
			// A delegate is created and is bound to the member function.
			Character->FireDelegate = FTimerDelegate::CreateUObject(this, &UFireState::Fire, Character);
			
			Character->GetWorldTimerManager().SetTimer(Character->FireTimerHandle, Character->FireDelegate,
													   Character->Weapons[Character->CurrentWeaponIndex]->WeaponFireRate,
													   true);
		}
		
	}
	else
	{
		// if we don't have any ammo in the gun, make a click noise
		UGameplayStatics::PlaySoundAtLocation(this, Character->OutOfAmmoSound, Character->GetActorLocation());
	}
}

void UFireState::TryExitState(AZombieGameCharacter* Character)
{
	bIsFiring = false;
	Character->GetWorldTimerManager().ClearTimer(Character->FireTimerHandle);
	if (Character->CurrentStateInstance->IsA<UReloadingState>() || Character->CurrentStateInstance->IsA<USwappingWeaponState>()) return;
	if (Character->CurrentStateInstance->IsA<UIdleFireState>())
	{
		Character->CurrentStateInstance = NewObject<UIdleState>(Character);
		Character->CurrentStateInstance->EnterState(Character);
	}
	
}

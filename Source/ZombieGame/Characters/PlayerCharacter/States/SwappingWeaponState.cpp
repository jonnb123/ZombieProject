// Fill out your copyright notice in the Description page of Project Settings.


#include "SwappingWeaponState.h"
#include "IdleState.h"
#include "ReloadingState.h"

void USwappingWeaponState::EnterState(AZombieGameCharacter* Character)
{
	// Character->GetWorldTimerManager().ClearTimer(Character->ReloadTimerHandle);
	// if the user switches weapon whilst reloading, the reload animation will not carry over to the next weapon.
	bool Success = false;

	for (int i = 0; i < Character->Weapons.Num(); i++)
	{
		if (i > Character->CurrentWeaponIndex)
		{
			if (Character->Weapons[i]->IsObtained)
			{
				const FTimerDelegate SwapDelegate = FTimerDelegate::CreateUObject(this, &USwappingWeaponState::WeaponSwapAfterDelay, Character);
				Character->GetWorldTimerManager().SetTimer(WeaponSwapTimerHandle, SwapDelegate,WeaponSwapDelay,false);
				Success = true;
				Character->CurrentWeaponIndex = i;
				UE_LOG(LogTemp, Display, TEXT("Current Weapon index: %d"), Character->CurrentWeaponIndex);
				Character->GunMesh->SetSkeletalMesh(Character->Weapons[Character->CurrentWeaponIndex]->WeaponMesh);
				// Attach GunMesh to the new socket
				Character->GunMesh->AttachToComponent(Character->Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale,
										   Character->Weapons[Character->CurrentWeaponIndex]->WeaponSocketName);
				// This is referenced in the abp to change weapon
				Character->EquippedWeaponCharacter = Character->Weapons[Character->CurrentWeaponIndex]->WeaponType;
			}
			break; // breaks out of the for loop
		}
	}

	if (!Success) // for the pistol
		{
		Character->CurrentWeaponIndex = 0;
		UE_LOG(LogTemp, Display, TEXT("Current Weapon index: %d"), Character->CurrentWeaponIndex);
		Character->CurrentStateInstance = NewObject<UIdleState>(Character);
		Character->CurrentStateInstance->EnterState(Character);
		Character->GunMesh->SetSkeletalMesh(Character->Weapons[Character->CurrentWeaponIndex]->WeaponMesh);
		Character->GunMesh->AttachToComponent(Character->Mesh1P, FAttachmentTransformRules::SnapToTargetIncludingScale,
								   Character->Weapons[Character->CurrentWeaponIndex]->WeaponSocketName);
		Character->EquippedWeaponCharacter = Character->Weapons[Character->CurrentWeaponIndex]->WeaponType;
		}
}

void USwappingWeaponState::TryEnterState(AZombieGameCharacter* Character)
{
	if (Character->CurrentStateInstance->IsA<UReloadingState>()) return;
	Character->CurrentStateInstance = NewObject<USwappingWeaponState>(Character);
	Character->CurrentStateInstance->EnterState(Character);
}

void USwappingWeaponState::TryExitState(AZombieGameCharacter* Character)
{
	Character->CurrentStateInstance = NewObject<UIdleState>(Character);
	Character->CurrentStateInstance->EnterState(Character);
}

void USwappingWeaponState::WeaponSwapAfterDelay(AZombieGameCharacter* Character)
{
	Character->CurrentStateInstance->TryExitState(Character);
	// Character->CurrentStateInstance = NewObject<UIdleState>(Character);
	// Character->CurrentStateInstance->EnterState(Character);
}


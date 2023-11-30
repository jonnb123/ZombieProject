// Fill out your copyright notice in the Description page of Project Settings.


#include "ReloadingState.h"
#include "AimingState.h"
#include "IdleState.h"

void UReloadingState::EnterState(AZombieGameCharacter* Character)
{
	const float MontageDuration = Character->Weapons[Character->CurrentWeaponIndex]->WeaponReloadMontage->
		GetPlayLength();
	const float WeaponReloadDelay = MontageDuration - 0.2; // Skip the last 0.2second
	FTimerHandle ReloadTimerHandle;
	Character->GunMesh->PlayAnimation(Character->Weapons[Character->CurrentWeaponIndex]->WeaponReloadMontage, false);

	// A delegate is created and is bound to the member function.
	const FTimerDelegate ReloadDelegate = FTimerDelegate::CreateUObject(this, &UReloadingState::RefillAmmo, Character);
	Character->GetWorldTimerManager().SetTimer(ReloadTimerHandle, ReloadDelegate, WeaponReloadDelay, false);
}


void UReloadingState::RefillAmmo(AZombieGameCharacter* Character)
{
	UE_LOG(LogTemp, Log, TEXT("Switching Mags!"));
	if (Character->Weapons[Character->CurrentWeaponIndex]->CurrentWeaponAmmo == Character->Weapons[Character->
		CurrentWeaponIndex]->MaxWeaponClipSize || Character->Weapons[
		Character->CurrentWeaponIndex]->TotalWeaponAmmo <= 0)
	{
		// if the CurrentWeaponAmmo in the weapon is at the max clip size or is less than or the gun has no reserve ammo
	}
	else
	{
		// current ammo is the current ammo in the clip
		// Needed ammo is the amount of ammo needed to make a full clip
		const int NeededAmmo = Character->Weapons[Character->CurrentWeaponIndex]->MaxWeaponClipSize - Character->Weapons
			[Character->CurrentWeaponIndex]->
			CurrentWeaponAmmo;
		if (Character->Weapons[Character->CurrentWeaponIndex]->TotalWeaponAmmo >= NeededAmmo)
		{
			Character->Weapons[Character->CurrentWeaponIndex]->CurrentWeaponAmmo = Character->Weapons[Character->
					CurrentWeaponIndex]->CurrentWeaponAmmo +
				NeededAmmo; // adds the ammo needed for a full clip
			Character->Weapons[Character->CurrentWeaponIndex]->TotalWeaponAmmo = Character->Weapons[Character->
				CurrentWeaponIndex]->TotalWeaponAmmo - NeededAmmo;
			// deducts ammo added to clip
		}
		else if (Character->Weapons[Character->CurrentWeaponIndex]->TotalWeaponAmmo > 0)
		// if the ammo amount is less than the needed ammo
		{
			Character->Weapons[Character->CurrentWeaponIndex]->CurrentWeaponAmmo = Character->Weapons[Character->
				CurrentWeaponIndex]->CurrentWeaponAmmo + Character->Weapons[
				Character->CurrentWeaponIndex]->TotalWeaponAmmo;
			Character->Weapons[Character->CurrentWeaponIndex]->TotalWeaponAmmo = 0;
		}
	}
	Character->AmmoArray[Character->CurrentWeaponIndex] = Character->Weapons[Character->CurrentWeaponIndex]->TotalWeaponAmmo;
	Character->CurrentStateInstance = NewObject<UIdleState>(Character);
	Character->CurrentStateInstance->EnterState(Character);
}

void UReloadingState::ExitState(AZombieGameCharacter* Character)
{
}

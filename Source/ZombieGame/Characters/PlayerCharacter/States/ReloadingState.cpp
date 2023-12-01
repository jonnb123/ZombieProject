// Fill out your copyright notice in the Description page of Project Settings.


#include "ReloadingState.h"
#include "AimingState.h"
#include "IdleFireState.h"
#include "IdleState.h"
#include "Components/Button.h"
#include "ZombieGame/Characters/Grandad/Grandad.h"

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

void UReloadingState::TryEnterState(AZombieGameCharacter* Character)
{
	if (Character->Weapons[Character->CurrentWeaponIndex]->TotalWeaponAmmo != 0 && Character->Weapons[Character->
		CurrentWeaponIndex]->CurrentWeaponAmmo != Character->Weapons[
		Character->CurrentWeaponIndex]->MaxWeaponClipSize)
	{
		if (Character->CurrentStateInstance->IsA<UReloadingState>() || Character->CurrentStateInstance->IsA<
			UAimingState>() || Character->CurrentStateInstance->IsA<UIdleFireState>()) return;
		Character->CurrentStateInstance = NewObject<UReloadingState>(Character);
		Character->CurrentStateInstance->EnterState(Character);
	}
}

void UReloadingState::TryExitState(AZombieGameCharacter* Character)
{
	Character->CurrentStateInstance = NewObject<UIdleState>(Character);
	Character->CurrentStateInstance->EnterState(Character);
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
	const AGrandad* Grandad = AGrandad::GetInstance();
	if (Grandad->ShopWidgetInstance)
	{
		for (UShopItem* ShopItem : Grandad->ShopWidgetInstance->ShopItems)
		{
			if (ShopItem && ShopItem->Item && ShopItem->ItemButton)
			{
				if (Character->Weapons[Character->CurrentWeaponIndex]->WeaponType == EWeaponType::E_Pistol)
				{
					if (ShopItem->Item->Name.ToString() == TEXT("Pistol Ammo"))
					{
						ShopItem->bIsOwned = false;
						ShopItem->ItemButton->SetBackgroundColor(FLinearColor::Gray);
					}
				}
				else if (Character->Weapons[Character->CurrentWeaponIndex]->WeaponType == EWeaponType::E_AssaultRifle)
				{
					if (ShopItem->Item->Name.ToString() == TEXT("Assault Rifle Ammo"))
					{
						ShopItem->bIsOwned = false;
						ShopItem->ItemButton->SetBackgroundColor(FLinearColor::Gray);
					}
				}
				else if (Character->Weapons[Character->CurrentWeaponIndex]->WeaponType == EWeaponType::E_Shotgun)
				{
					if (ShopItem->Item->Name.ToString() == TEXT("Shotgun Ammo"))
					{
						ShopItem->bIsOwned = false;
						ShopItem->ItemButton->SetBackgroundColor(FLinearColor::Gray);
					}
				}

				// if (ShopItem->Item->Name.ToString() == TEXT("Pistol Ammo"))
				// {
				// 	ShopItem->bIsOwned = false;
				// 	ShopItem->ItemButton->SetBackgroundColor(FLinearColor::Gray);
				// }
				// if (ShopItem->Item->Name.ToString() == TEXT("Assault Rifle Ammo"))
				// {
				// 	ShopItem->bIsOwned = false;
				// 	ShopItem->ItemButton->SetBackgroundColor(FLinearColor::Gray);
				// }
				// if (ShopItem->Item->Name.ToString() == TEXT("Shotgun Ammo"))
				// {
				// 	ShopItem->bIsOwned = false;
				// 	ShopItem->ItemButton->SetBackgroundColor(FLinearColor::Gray);
				// }
			}
		}
	}

	Character->AmmoArray[Character->CurrentWeaponIndex] = Character->Weapons[Character->CurrentWeaponIndex]->
		TotalWeaponAmmo;
	Character->CurrentStateInstance->TryExitState(Character);
}

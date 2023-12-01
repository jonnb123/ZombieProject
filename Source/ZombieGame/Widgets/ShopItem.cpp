// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieGame/Widgets/ShopItem.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/Button.h"
#include "ZombieGame/Widgets/ShopWidget.h"
#include "ZombieGame/Characters/Grandad/Grandad.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieGame/GameMode/ZombieGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"


void UShopItem::NativeConstruct()
{
	if (ItemButton)
	{
		ItemButton->OnClicked.AddDynamic(this, &UShopItem::OnItemClicked);
	}

	const AGrandad* Grandad = AGrandad::GetInstance();
	const int ItemID = Grandad->ShopWidgetInstance->ItemID;
	if (ItemID >= 0)
	{
		FString ItemIDAsString = FString::FromInt(ItemID);
		static const FString ContextString(TEXT("Item Name Context"));
		Item = ItemDataTable->FindRow<FItemStructure>(FName(ItemIDAsString), ContextString, true);
		ItemNameText->SetText(Item->Name);
		ItemPriceText->SetText(FText::FromString(FString::FromInt(Item->Cost)));
		ItemImage->SetBrushFromTexture(Item->Thumbnail);

		if (Item->Name.ToString() == TEXT("Pistol Ammo") || Item->Name.ToString() == TEXT("Assault Rifle Ammo") || Item->Name.ToString() == TEXT("Shotgun Ammo"))
		{
			bIsOwned = true;
		}
		if (bIsOwned == true)
		{
			ItemButton->SetBackgroundColor(FLinearColor::Red);
		}
	}
	else
	{
		ItemNameText->SetText(FText::FromString(""));
		ItemPriceText->SetText(FText::FromString(""));
		ItemButton->SetVisibility(ESlateVisibility::Hidden);
	}
}

void UShopItem::OnItemClicked()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter);

	if (Character->GetPoints() >= Item->Cost && bIsOwned == false)
	{
		Character->SetPoints(Character->GetPoints() - Item->Cost);
		bIsOwned = true;
		ItemButton->SetBackgroundColor(FLinearColor::Red);
		
		if (Item->Name.ToString() == TEXT("Front Door"))
		{
			const AZombieGameMode* GameMode = Cast<AZombieGameMode>(UGameplayStatics::GetGameMode(this));

			AFrontDoor::SetInstance(
				Cast<AFrontDoor>(GetWorld()->SpawnActor(FrontDoor, &DoorSpawnLocation, &DoorSpawnRotation)));
			AFrontDoor::GetInstance()->bIsSpawned = true;
			GameMode->OnDoorSpawn.Broadcast();
		}

		if (Item->Name.ToString() == TEXT("Dog"))
		{
			UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), DogPawn, BehaviorTree, DogSpawnLocation);
		}

		if (Item->Name.ToString() == TEXT("Turret") && TurretComplete == false)
		{
			FActorSpawnParameters ActorSpawnParams;
			ActorSpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			AActor* Turret = GetWorld()->SpawnActor(TurretClass, &TurretSpawnLocation, &TurretSpawnRotation,
			                                        ActorSpawnParams);
		}

		// Accessing player weapons
		ABaseWeapon* Pistol = nullptr;
		ABaseWeapon* AssaultRilfe = nullptr;
		ABaseWeapon* Shotgun = nullptr;
		
		for (ABaseWeapon* Weapon : Character->Weapons)
		{
			if (Weapon->WeaponType == EWeaponType::E_Pistol)
			{
				Pistol = Weapon;
			}
			if (Weapon->WeaponType == EWeaponType::E_AssaultRifle)
			{
				AssaultRilfe = Weapon;
			}
			if (Weapon->WeaponType == EWeaponType::E_Shotgun)
			{
				Shotgun = Weapon;
			}
		}
		
		if (Item->Name.ToString() == TEXT("Pistol Ammo") && Pistol->TotalWeaponAmmo != Pistol->MaxWeaponAmmo)
		{
			Pistol->TotalWeaponAmmo = Pistol->MaxWeaponAmmo;
		}

		if (Item->Name.ToString() == TEXT("Assault Rifle Ammo") && AssaultRilfe->TotalWeaponAmmo != AssaultRilfe->MaxWeaponAmmo)
		{
			AssaultRilfe->TotalWeaponAmmo = AssaultRilfe->MaxWeaponAmmo;
		}
		
		if (Item->Name.ToString() == TEXT("Shotgun Ammo") && Shotgun->TotalWeaponAmmo != Shotgun->MaxWeaponAmmo)
		{
			Shotgun->TotalWeaponAmmo = Shotgun->MaxWeaponAmmo;
		}
	}
}



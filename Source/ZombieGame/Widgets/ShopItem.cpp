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
		const FString ItemIDAsString = FString::FromInt(ItemID);
		static const FString ContextString(TEXT("Item Name Context"));
		Item = ItemDataTable->FindRow<FItemStructure>(FName(ItemIDAsString), ContextString, true);
		ItemNameText->SetText(Item->Name);
		ItemPriceText->SetText(FText::FromString(FString::FromInt(Item->Cost)));
		ItemImage->SetBrushFromTexture(Item->Thumbnail);

		if (Item->Name.ToString() == TEXT("Pistol Ammo") || Item->Name.ToString() == TEXT("Assault Rifle Ammo") || Item
			->Name.ToString() == TEXT("Shotgun Ammo"))
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
		
		AActor* NewInstance = GetWorld()->SpawnActor<AActor>(Item->Actor, FVector::ZeroVector, FRotator::ZeroRotator);
		
		if (IBuyableItemInterface* TheInterface = Cast<IBuyableItemInterface>(NewInstance))
		{
			TheInterface->HandleBuyItem();
		}
		
	}
}

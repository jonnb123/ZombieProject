// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ShopItem.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API UShopItem : public UUserWidget
{
	GENERATED_BODY()

private:
	// Vertical Box
	
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox *ItemVerticalBox;

	// Text

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *ItemNameText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *ItemPriceText;

	// Size Box

	UPROPERTY(meta = (BindWidget))
	class USizeBox *SizeBox;

	// Button

	UPROPERTY(meta = (BindWidget))
	class UButton *ItemButton;

	// Image

	UPROPERTY(meta = (BindWidget))
	class UImage *ItemImage;

};

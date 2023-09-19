// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ZombieGame/Widgets/ShopItem.h"
#include "Engine/DataTable.h"
#include "ShopWidget.generated.h"

/**
 * 
 */



UCLASS()
class ZOMBIEGAME_API UShopWidget : public UUserWidget
{
	GENERATED_BODY()

private:	
	// Canvas Panel

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel *CanvasPanel;

	// Vertical Box
	
	UPROPERTY(meta = (BindWidget))
	class UVerticalBox *ShopVerticalBox;

	// Border

	UPROPERTY(meta = (BindWidget))
	class UBorder *Border;

	// Text

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *ShopText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *ExitText;

	// Button

	UPROPERTY(meta = (BindWidget))
	class UButton *ExitButton;

	// Grid Panel

	UPROPERTY(meta = (BindWidget))
	class UUniformGridPanel *ItemGrid;

	// Functions

	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void OnExitClicked();

	UFUNCTION()
	void FetchItem(int Index);

	

protected:

	// Widget References

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TSubclassOf<UUserWidget> WidgetClass;
	UShopItem* ShopItemWidgetInstance; // needs to be accessed in other classes


public:
	UPROPERTY(EditAnywhere)
	int ItemID;

	TArray<UShopItem*> ShopItems;

};

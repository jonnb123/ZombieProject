// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Engine/DataTable.h"
#include "ZombieGame/Characters/FrontDoor/FrontDoor.h"
#include "ZombieGame/Characters/Zombie/AI/ZombieAIController.h"
// #include "ZombieGame/Characters/Turret/Turret.h"
#include "ShopItem.generated.h"

/**
 *
 */

USTRUCT(BlueprintType)
struct FItemStructure : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTexture2D *Thumbnail;
};

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

	// Data Table
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	class UDataTable *ItemDataTable;

	// native construct
	void NativeConstruct() override;

	// button functions
	UFUNCTION(BlueprintCallable)
	void OnItemClicked();

	FItemStructure *Item;

	bool bIsOwned = false;

	bool TurretComplete = false;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<AFrontDoor> FrontDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector DoorSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator DoorSpawnRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<APawn> DogPawn;

	UPROPERTY(EditAnywhere)
	class UBehaviorTree *BehaviorTree;

	UPROPERTY(EditAnywhere)
	FVector DogSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector TurretSpawnLocation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator TurretSpawnRotation;

	// The class of the turret that will be spawned
	// UPROPERTY(EditAnywhere)
	// TSubclassOf<ATurret> TurretClass;

public:
	UPROPERTY()
	AFrontDoor *SpawnedDoor;
};

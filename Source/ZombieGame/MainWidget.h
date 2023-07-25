// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

/**
 *
 */
UCLASS()
class ZOMBIEGAME_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel *CanvasPanel;

	UPROPERTY(meta = (BindWidget))
	class UImage *Crosshair;

	UPROPERTY(meta = (BindWidget))
	class UProgressBar *HealthBar;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *WaveText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *ZombiesLeftText;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UHorizontalBox *WaveStartingBox;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UTextBlock *WaveStartingText;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UVerticalBox *WinWindow;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UVerticalBox *DeathWindow;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UImage *BloodOverlay;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UTextBlock *PointsText;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UTextBlock *AmmoText;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UTextBlock *EquipItemText;

	UPROPERTY(meta = (BindWidget), BlueprintReadWrite)
	class UTextBlock *WeaponNameText;

	void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	FText UpdateWaveText();

	UFUNCTION(BlueprintCallable)
	float UpdateHealthBar();

	UFUNCTION(BlueprintCallable)
	FText UpdateZombiesRemaining();

	UFUNCTION(BlueprintCallable)
	FText UpdateAmmunition();

	UFUNCTION(BlueprintCallable)
	FText UpdateCharacterPoints();

	UFUNCTION(BlueprintCallable)
	FText UpdatePlayerWeaponName();

	UFUNCTION(BlueprintCallable)
	void ShowEquipText();
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainWidget.generated.h"

UCLASS()
class ZOMBIEGAME_API UMainWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// Used in BuyableItem.cpp
	UPROPERTY(meta = (BindWidget))
	class UTextBlock *EquipItemText;

	// Used in HealthJuice.cpp
	UPROPERTY(meta = (BindWidget))
	class UProgressBar *HealthBar;

	// Used in HealthJuice.cpp
	UPROPERTY(meta = (BindWidget))
	class UProgressBar *HJHealthBar;

	// Used in ZombieGameMode.cpp
	UFUNCTION(BlueprintCallable)
	void ShowWaveStart(int CurrentWave);

	// Used in ZombieGameMode.cpp
	UFUNCTION(BlueprintCallable)
	void ShowWinText();

	// Used in ZombieGameCharacter.cpp
	UFUNCTION(BlueprintCallable)
	void ShowDeathWindow();

	// Used in ZombieGameCharacter.cpp
	UFUNCTION(BlueprintCallable)
	void ShowBloodOverlay();

	// Used in ZombieGameCharacter.cpp
	UFUNCTION(BlueprintCallable)
	void HideBloodEffect();

	// Images

	UPROPERTY(meta = (BindWidget))
	class UImage *Crosshair;

private:
	// Canvas Panel

	UPROPERTY(meta = (BindWidget))
	class UCanvasPanel *CanvasPanel;

	

	UPROPERTY(meta = (BindWidget))
	class UImage *BloodOverlay;

	// Text Blocks

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *WaveText;

	// UPROPERTY(meta = (BindWidget))
	// class UTextBlock *ZombiesLeftText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *WaveStartingText;

	UPROPERTY(meta = (BindWidget))
	class UHorizontalBox *WaveStartingBox;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *PointsText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *AmmoText;

	UPROPERTY(meta = (BindWidget))
	class UTextBlock *WeaponNameText;

	// Vertical boxes

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox *WinWindow;

	UPROPERTY(meta = (BindWidget))
	class UVerticalBox *DeathWindow;

	// Buttons

	UPROPERTY(meta = (BindWidget))
	class UButton *ExitToMainMenuButtonWin;

	UPROPERTY(meta = (BindWidget))
	class UButton *ExitToMainMenuButtonLose;

	UPROPERTY(meta = (BindWidget))
	class UButton *QuitGameButtonWin;

	UPROPERTY(meta = (BindWidget))
	class UButton *QuitGameButtonLose;

	UPROPERTY(meta = (BindWidget))
	class UButton *RetryButton;

	// Animations

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation *WaveAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation *WinWindowAnimation;

	UPROPERTY(Transient, meta = (BindWidgetAnim))
	class UWidgetAnimation *BloodAnimation;

	// Button handling

	UFUNCTION(BlueprintCallable)
	void OnMainMenuClicked();

	UFUNCTION(BlueprintCallable)
	void OnQuitClicked();

	UFUNCTION(BlueprintCallable)
	void OnRetryClicked();

	// Native Construct

	void NativeConstruct() override;

	// Update functions

	UFUNCTION(BlueprintCallable)
	FText UpdateWaveText();

	UFUNCTION(BlueprintCallable)
	FText UpdateZombiesRemaining();

	UFUNCTION(BlueprintCallable)
	FText UpdateAmmunition();

	UFUNCTION(BlueprintCallable)
	FText UpdateCharacterPoints();

	UFUNCTION(BlueprintCallable)
	FText UpdatePlayerWeaponName();

	UFUNCTION(BlueprintCallable)
	float UpdateHealthBar();

	UFUNCTION(BlueprintCallable)
	float UpdateHJHealthBar();
};

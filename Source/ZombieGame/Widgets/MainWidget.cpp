// Fill out your copyright notice in the Description page of Project Settings.

#include "MainWidget.h"
#include "ZombieGame/GameMode/ZombieGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "UMG/Public/Components/TextBlock.h"
#include "UMG/Public/Components/HorizontalBox.h"
#include "UMG/Public/Components/VerticalBox.h"
#include "UMG/Public/Components/Image.h"
#include "UMG/Public/Components/Button.h"


void UMainWidget::NativeConstruct()
{
	Super::NativeConstruct();

	// if the Exit to main menu button is clicked
	if (ExitToMainMenuButtonWin)
	{
		ExitToMainMenuButtonWin->OnClicked.AddDynamic(this, &UMainWidget::OnMainMenuClicked);
	}
	if (ExitToMainMenuButtonLose)
	{
		ExitToMainMenuButtonLose->OnClicked.AddDynamic(this, &UMainWidget::OnMainMenuClicked);
	}
	// if the quit game button is clicked
	if (QuitGameButtonWin)
	{
		QuitGameButtonWin->OnClicked.AddDynamic(this, &UMainWidget::OnQuitClicked);
	}
	if (QuitGameButtonLose)
	{
		QuitGameButtonLose->OnClicked.AddDynamic(this, &UMainWidget::OnQuitClicked);
	}
	// if the retry button is clicked
	if (RetryButton)
	{
		RetryButton->OnClicked.AddDynamic(this, &UMainWidget::OnRetryClicked);
	}
}

FText UMainWidget::UpdateWaveText()
{
	if (const AZombieGameMode* GameMode = Cast<AZombieGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		const FString Text = FString::Printf(TEXT("%d"), GameMode->GetCurrentWave());
		return FText::FromString(Text);
	}
	else
	{
		// If GameMode is nullptr, return "N/A"
		return FText::FromString("N/A");
	}
}

float UMainWidget::UpdateHealthBar()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (const AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter))
	{
		return Character->GetCharacterHealth() * 0.01;
	}
	else
	{
		return 0.f;
	}
}

float UMainWidget::UpdateHJHealthBar()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	if (const AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter))
	{
		return Character->GetCharacterHealth() * 0.005;
	}
	else
	{
		return 0.f;
	}
}

FText UMainWidget::UpdateZombiesRemaining()
{
	if (const AZombieGameMode* GameMode = Cast<AZombieGameMode>(UGameplayStatics::GetGameMode(this)))
	{
		const FString Text = FString::Printf(TEXT("Zombies Left: %d"), GameMode->GetZombiesLeft());
		return FText::FromString(Text);
	}
	else
	{
		// If GameMode is nullptr, return "N/A"
		return FText::FromString("N/A");
	}
}

FText UMainWidget::UpdateAmmunition()
{
	if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter);

		if (Character && Character->Weapons.Num() > 0 && Character->CurrentWeaponIndex < Character->Weapons.Num())
		{
			if (const ABaseWeapon* CurrentWeapon = Character->Weapons[Character->CurrentWeaponIndex])
			{
				const FString Text = FString::Printf(TEXT("%d/%d"), CurrentWeapon->CurrentWeaponAmmo,
				                                     CurrentWeapon->TotalWeaponAmmo);
				return FText::FromString(Text);
			}
		}
	}
	// If any check fails, return a default value or handle the case appropriately
	return FText::FromString("N/A");
}

FText UMainWidget::UpdateCharacterPoints()
{
	if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (const AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter))
		{
			const FString Text = FString::Printf(TEXT("%d"), Character->GetPoints());
			return FText::FromString(Text);
		}
	}

	return FText::FromString("N/A");
}

FText UMainWidget::UpdatePlayerWeaponName()
{
	if (ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
	{
		if (AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter))
		{
			const FString WeaponName = Character->Weapons[Character->CurrentWeaponIndex]->Name;
			return FText::FromString(WeaponName);
		}
	}
	return FText::FromString("N/A");
	
}

void UMainWidget::ShowWaveStart(int CurrentWave)
{
	const FString Text = FString::Printf(TEXT("Wave %d Starting!"), CurrentWave);
	const FText TextToSet = FText::FromString(Text);
	WaveStartingText->SetText(TextToSet);
	WaveStartingBox->SetVisibility(ESlateVisibility::Visible);
	PlayAnimationForward(WaveAnimation);
}

void UMainWidget::ShowWinText()
{
	WinWindow->SetVisibility(ESlateVisibility::Visible);
	PlayAnimationForward(WinWindowAnimation);
}

void UMainWidget::ShowDeathWindow()
{
	if (DeathWindow)
	{
		DeathWindow->SetVisibility(ESlateVisibility::Visible);
	}
}

void UMainWidget::ShowBloodOverlay()
{
	BloodOverlay->SetVisibility(ESlateVisibility::Visible);
	PlayAnimation(BloodAnimation, 0.f, 0); // setting no. of loops to play to 0 does infinite loops!
}

void UMainWidget::HideBloodEffect()
{
	BloodOverlay->SetVisibility(ESlateVisibility::Hidden);
	StopAnimation(BloodAnimation);
}

void UMainWidget::OnMainMenuClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("MainMenu"));
}

void UMainWidget::OnQuitClicked()
{
	FGenericPlatformMisc::RequestExit(false);
}

void UMainWidget::OnRetryClicked()
{
	UGameplayStatics::OpenLevel(GetWorld(), TEXT("ZombieMap"));

	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (APlayerController* PlayerController = Cast<APlayerController>(Character->GetController()))
	{
		PlayerController->Possess(Character);
		PlayerController->SetInputMode(FInputModeGameOnly());
	}
}

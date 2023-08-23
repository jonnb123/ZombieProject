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
    AZombieGameMode *GameMode = Cast<AZombieGameMode>(UGameplayStatics::GetGameMode(this));
    if (GameMode)
    {
        FString Text = FString::Printf(TEXT("Wave %d/%d"), GameMode->GetCurrentWave(), GameMode->GetMaxWaves());
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
    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
    if (Character)
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
    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
    if (Character)
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
    AZombieGameMode *GameMode = Cast<AZombieGameMode>(UGameplayStatics::GetGameMode(this));
    if (GameMode)
    {
        FString Text = FString::Printf(TEXT("Zombies Left: %d"), GameMode->GetZombiesLeft());
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
    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
    ABaseWeapon *CurrentWeapon = Character->Weapons[Character->CurrentWeaponIndex];
    if (CurrentWeapon)
    {
        FString Text = FString::Printf(TEXT("%d/%d"), CurrentWeapon->CurrentAmmo, CurrentWeapon->TotalAmmo);
        return FText::FromString(Text);
    }
    else
    {
        // If GameMode is nullptr, return "N/A"
        return FText::FromString("N/A");
    }
}

FText UMainWidget::UpdateCharacterPoints()
{
    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
    if (Character)
    {
        FString Text = FString::Printf(TEXT("%d"), Character->GetPoints());
        return FText::FromString(Text);
    }
    else
    {
        return FText::FromString("N/A");
    }
}

FText UMainWidget::UpdatePlayerWeaponName()
{
    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
    if (Character)
    {
        FString WeaponName = Character->Weapons[Character->CurrentWeaponIndex]->Name;
        return FText::FromString(WeaponName);
    }
    else
    {
        return FText::FromString("N/A");
    }
}

void UMainWidget::ShowWaveStart(int CurrentWave)
{
    FString Text = FString::Printf(TEXT("Wave %d Starting!"), CurrentWave);
    FText TextToSet = FText::FromString(Text);
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
    DeathWindow->SetVisibility(ESlateVisibility::Visible);
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

    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
    APlayerController* PlayerController = Cast<APlayerController>(Character->GetController());
    if (PlayerController)
    {
        PlayerController->Possess(Character);
        PlayerController->SetInputMode(FInputModeGameOnly());
    }
}


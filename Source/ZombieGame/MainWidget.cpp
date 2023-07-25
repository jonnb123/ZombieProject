// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieGameMode.h"
#include "MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Components/TextBlock.h"
#include "ZombieGameCharacter.h"
// #include "BaseWeapon.h"

void UMainWidget::NativeConstruct()
{
}

FText UMainWidget::UpdateWaveText()
{
    AZombieGameMode *GameMode = Cast<AZombieGameMode>(UGameplayStatics::GetGameMode(this));
    if (GameMode)
    {
        FString Text = FString::Printf(TEXT("Wave %d/%d"), GameMode->CurrentWave, GameMode->MaxWaves);
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
        return Character->Health * 0.01;
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
        FString Text = FString::Printf(TEXT("Zombies Left: %d"), GameMode->ZombiesLeft);
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
        FString Text = FString::Printf(TEXT("%d"), Character->Points);
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

void UMainWidget::ShowEquipText()
{
    
}

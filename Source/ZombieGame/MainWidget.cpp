// Fill out your copyright notice in the Description page of Project Settings.

# include "ZombieGameMode.h"
#include "MainWidget.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Components/TextBlock.h"


void UMainWidget::NativeConstruct()
{
   
}

FText UMainWidget::UpdateWaveText()
{
    AZombieGameMode* GameMode = Cast<AZombieGameMode>(UGameplayStatics::GetGameMode(this));
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

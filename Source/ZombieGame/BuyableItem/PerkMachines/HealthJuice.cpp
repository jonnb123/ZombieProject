// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthJuice.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Components/ProgressBar.h"

void AHealthJuice::UseBuyableItem() 
{
    UE_LOG(LogTemp, Warning, TEXT("You have interacted with health juice"));
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->GetPoints() >= 100 && Character->GetCharacterHealth() <= 100) // set the points to be 2000
	{
		Character->SetPoints(Character->GetPoints() - 100);
		Character->SetCharacterHealth(200);
		Character->SetCharacterMaxHealth(200);
		Character->MainWidgetInstance->HJHealthBar->SetVisibility(ESlateVisibility::Visible);
		Character->MainWidgetInstance->HealthBar->SetVisibility(ESlateVisibility::Hidden);
		PlayConsumeAnimation();
	}
}

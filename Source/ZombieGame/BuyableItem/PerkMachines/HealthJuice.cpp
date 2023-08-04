// Fill out your copyright notice in the Description page of Project Settings.

#include "HealthJuice.h"
// #include "ZombieGameCharacter.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"
// #include "BaseWeapon.h"
#include "ZombieGame/BuyableItem/Weapons/BaseWeapon.h"
#include "UMG/Public/Components/ProgressBar.h"
#include "Algo/Sort.h"
#include "Camera/CameraComponent.h"


void AHealthJuice::UseBuyableItem() 
{
    UE_LOG(LogTemp, Warning, TEXT("You have interacted with health juice"));
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->Points >= 100 && Character->Health <= 100) // set the points to be 2000
	{
		Character->Points -= 100; // set the points to be 2000
		Character->Health = 200;
		Character->MaxHealth = 200;
		Character->MainWidgetInstance->HJHealthBar->SetVisibility(ESlateVisibility::Visible);
		Character->MainWidgetInstance->HealthBar->SetVisibility(ESlateVisibility::Hidden);
		PlayConsumeAnimation();
	}
}

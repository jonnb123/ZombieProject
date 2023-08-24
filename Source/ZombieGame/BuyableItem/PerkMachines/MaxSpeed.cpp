// Fill out your copyright notice in the Description page of Project Settings.

#include "MaxSpeed.h"
// #include "ZombieGameCharacter.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"
// #include "BaseWeapon.h"
#include "ZombieGame/BuyableItem/Weapons/BaseWeapon.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Algo/Sort.h"
#include "Camera/CameraComponent.h"

void AMaxSpeed::UseBuyableItem()
{
    UE_LOG(LogTemp, Warning, TEXT("You have interacted with Max Speed"));
    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
    if (Character->GetPoints() >= 100 && Character->GetCharacterMovement()->MaxWalkSpeed < 1200.0f) // set the points to be 1000
    {
        Character->SetPoints(Character->GetPoints() - 100);
        Character->GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
        Character->SetHasMaxSpeed(true);
        PlayConsumeAnimation();
    }
}


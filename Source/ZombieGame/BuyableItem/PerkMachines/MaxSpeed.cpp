// Fill out your copyright notice in the Description page of Project Settings.

#include "MaxSpeed.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"

void AMaxSpeed::UseBuyableItem()
{
    UE_LOG(LogTemp, Warning, TEXT("You have interacted with Max Speed"));
    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
    if (Character->GetPoints() >= FCString::Atoi(*ItemPrice) && Character->GetCharacterMovement()->MaxWalkSpeed < 1200.0f) // set the points to be 1000
    {
        Character->SetPoints(Character->GetPoints() - FCString::Atoi(*ItemPrice));
        Character->GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
        Character->SetHasMaxSpeed(true);
        PlayConsumeAnimation();
    }
}


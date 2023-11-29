// Fill out your copyright notice in the Description page of Project Settings.


#include "AimingState.h"

#include "Components/Image.h"

void UAimingState::EnterState(AZombieGameCharacter* Character)
{
	UE_LOG(LogTemp, Display, TEXT("Aiming Now!!!"));

	Character->MainWidgetInstance->Crosshair->SetVisibility(ESlateVisibility::Hidden);
	if (Character->GetHasMaxSpeed())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = 400.0f;
	}
	else
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = 200.f;
	}
}

void UAimingState::ExitState(AZombieGameCharacter* Character)
{
	
}

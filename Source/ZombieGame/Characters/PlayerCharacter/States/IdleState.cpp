// Fill out your copyright notice in the Description page of Project Settings.


#include "IdleState.h"
#include "Components/Image.h"

void UIdleState::EnterState(AZombieGameCharacter* Character)
{
	// can enter idle from any state
	UE_LOG(LogTemp, Display, TEXT("Entering idle state"));
	
	Character->MainWidgetInstance->Crosshair->SetVisibility(ESlateVisibility::Visible);

	if (Character->GetHasMaxSpeed())
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
	}
	else
	{
		Character->GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	}
}

void UIdleState::TryEnterState(AZombieGameCharacter* Character)
{
	Character->CurrentStateInstance = NewObject<UIdleState>(Character);
	Character->CurrentStateInstance->EnterState(Character);
}


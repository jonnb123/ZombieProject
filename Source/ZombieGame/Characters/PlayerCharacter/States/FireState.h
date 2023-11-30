// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStates.h"
#include "FireState.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API UFireState : public UCharacterStates
{
	GENERATED_BODY()

protected:
	
	virtual void EnterState(AZombieGameCharacter* Character) override ;

	virtual void TryEnterState(AZombieGameCharacter* Character) override;

	void Fire(AZombieGameCharacter* Character);

	virtual void TryExitState(AZombieGameCharacter* Character) override;

	// The time for this is the fire-rate of the weapon, used in the weapon classes
	FTimerHandle FireTimerHandle; // Not used with delegate
};

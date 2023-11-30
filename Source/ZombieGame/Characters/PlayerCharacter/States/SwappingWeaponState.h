// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStates.h"
#include "UObject/Object.h"
#include "SwappingWeaponState.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API USwappingWeaponState : public UCharacterStates
{
	GENERATED_BODY()
	
protected:
	virtual void EnterState(AZombieGameCharacter* Character) override ;

	virtual void TryEnterState(AZombieGameCharacter* Character) override;
	
	virtual void TryExitState(AZombieGameCharacter* Character) override;
	
	// weapon swap delay timer
	const float WeaponSwapDelay = 0.7;
	FTimerHandle WeaponSwapTimerHandle; // Not used with delegate

	void WeaponSwapAfterDelay(AZombieGameCharacter* Character);
};

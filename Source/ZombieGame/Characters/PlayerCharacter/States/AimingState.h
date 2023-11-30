// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStates.h"
#include "UObject/Object.h"
#include "AimingState.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ZOMBIEGAME_API UAimingState : public UCharacterStates
{
	GENERATED_BODY()

protected:	
	
	virtual void EnterState(AZombieGameCharacter* Character) override ;

	virtual void TryEnterState(AZombieGameCharacter* Character) override;

	virtual void TryExitState(AZombieGameCharacter* Character) override;
};

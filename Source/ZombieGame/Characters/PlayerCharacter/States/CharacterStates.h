// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "CharacterStates.generated.h"

/**
 * 
 */
UCLASS(BlueprintType)
class ZOMBIEGAME_API UCharacterStates : public UObject
{
	GENERATED_BODY()

public:
	// pure functions
	virtual void EnterState(AZombieGameCharacter* Character);

	virtual void TryEnterState(AZombieGameCharacter* Character);

	virtual void TryExitState(AZombieGameCharacter* Character);

};

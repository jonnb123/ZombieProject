// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStates.h"
#include "UObject/Object.h"
#include "ReloadingState.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API UReloadingState : public UCharacterStates
{
	GENERATED_BODY()

public:	
	
	virtual void EnterState(AZombieGameCharacter* Character) override ;

	virtual void ExitState(AZombieGameCharacter* Character) override;

	void RefillAmmo(AZombieGameCharacter* Character);
};

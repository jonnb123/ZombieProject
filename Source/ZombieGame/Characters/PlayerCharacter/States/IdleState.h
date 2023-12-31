// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "CharacterStates.h"
#include "UObject/Object.h"
#include "IdleState.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API UIdleState : public UCharacterStates
{
	GENERATED_BODY()

protected:	
	
	virtual void EnterState(AZombieGameCharacter* Character) override ;

	virtual void TryEnterState(AZombieGameCharacter* Character) override;

	
};

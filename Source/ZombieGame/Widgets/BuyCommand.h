// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Command.h"
#include "BuyCommand.generated.h"

/**
 * 
 */
UCLASS()
class ZOMBIEGAME_API UBuyCommand : public UCommand
{
	GENERATED_BODY()
public:
	virtual void Execute() override;

	virtual void Undo() override;

	
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "Command.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class ZOMBIEGAME_API UCommand : public UObject
{
	GENERATED_BODY()

public:
	virtual void Execute() PURE_VIRTUAL(UCommand::Execute());

	virtual void Undo() PURE_VIRTUAL(UCommand::Undo() )
};

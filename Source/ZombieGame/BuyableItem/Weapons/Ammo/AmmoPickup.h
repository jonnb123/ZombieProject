// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AmmoPickup.generated.h"

UCLASS()
class ZOMBIEGAME_API AAmmoPickup : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAmmoPickup();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** sphere component */
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	class USphereComponent* Sphere;

};

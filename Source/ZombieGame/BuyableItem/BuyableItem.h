// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ZombieGame/Characters/Turret/Turret.h"
#include "BuyableItem.generated.h"

UCLASS()
class ZOMBIEGAME_API ABuyableItem : public AActor
{
	GENERATED_BODY()
	
public:	
	// Used in ZombieGameCharacter.cpp, implementation of function in child classes
	virtual void UseBuyableItem();

	// Used in MainWidget.cpp
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Name")
	FString Name;

protected:

	virtual void BeginPlay() override;
	
	ABuyableItem();
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Price")
	FString ItemPrice;

	// Child box collision component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UBoxComponent* BoxCollisionComponent;

	// Function to handle the begin overlap event
    UFUNCTION()
    void OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
    void OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

private:
	UPROPERTY(EditAnywhere)
	USceneComponent* _RootComponent;

  
};

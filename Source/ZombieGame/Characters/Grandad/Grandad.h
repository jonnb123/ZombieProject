// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieGame/Widgets/MainWidget.h"
#include "Grandad.generated.h"

UCLASS()
class ZOMBIEGAME_API AGrandad : public ACharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int Health = 100;

public:	
	// Static method to get the Singleton instance
    static AGrandad* GetInstance();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	// gets main widget reference
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TSubclassOf<UUserWidget> WidgetClass;
	UMainWidget *MainWidgetInstance; // needs to be accessed in other classes

	

private:
	// Sets default values for this character's properties
	AGrandad();

	virtual float TakeDamage(float const DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;
	
	void HandleDeath();

	bool IsDead = false;

	// Static instance of Singleton
	static AGrandad* GrandadInstance;

};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ZombieGame/Widgets/MainWidget.h"
#include "Components/BoxComponent.h"
#include "Camera/CameraComponent.h"
#include "ZombieGame/Widgets/ShopWidget.h"
#include "Grandad.generated.h"

UCLASS()
class ZOMBIEGAME_API AGrandad : public ACharacter
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	int Health = 100;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UBoxComponent *BoxCollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UCameraComponent *ShopCameraComponent;

	

	// Function to handle the begin overlap event
    UFUNCTION()
    void OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
                           int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

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

	// Widget References

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "References")
	TSubclassOf<UUserWidget> ShopWidgetClass;
	UShopWidget* ShopWidgetInstance; // needs to be accessed in other classes

	virtual float TakeDamage(float const DamageAmount, struct FDamageEvent const &DamageEvent, class AController *EventInstigator, AActor *DamageCauser) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> Inventory;

	


private:
	// Sets default values for this character's properties
	AGrandad();

	
	void HandleDeath();

	bool IsDead = false;

	// Static instance of Singleton
	static AGrandad* GrandadInstance;

};

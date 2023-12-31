// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieGame/Characters/Dog/Dog.h"

#include "Blueprint/AIBlueprintHelperLibrary.h"

// Sets default values
ADog::ADog()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADog::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADog::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ADog::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ADog::HandleBuyItem()
{
	UE_LOG(LogTemp, Log, TEXT("Bought a DOG bruh"));
	UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), DogPawn, BehaviorTree, DogSpawnLocation);
}




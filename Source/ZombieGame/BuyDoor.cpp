// Fill out your copyright notice in the Description page of Project Settings.


#include "BuyDoor.h"
#include "ZombieGameCharacter.h"
#include "BuyableItem.h"
#include "Kismet/GameplayStatics.h"


void ABuyDoor::BuyDoor() 
{
    UE_LOG(LogTemp, Warning, TEXT("You have interacted with buy door"));
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->Points >= 100 && !bDoorRotated) // set the points to be 2000
	{
		Character->Points -= 100; // set the points to be 2000
        // Destroy();
        FRotator NewRotation = PerkMachineMesh->GetRelativeRotation();
        NewRotation.Yaw += 90.0f; // Rotate by 90 degrees around the Z-axis
        PerkMachineMesh->SetRelativeRotation(NewRotation);
	}
}



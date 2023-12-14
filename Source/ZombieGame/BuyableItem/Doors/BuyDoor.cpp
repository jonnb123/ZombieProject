// Fill out your copyright notice in the Description page of Project Settings.

#include "BuyDoor.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "ZombieGame/BuyableItem/BuyableItem.h"
#include "Kismet/GameplayStatics.h"

void ABuyDoor::HandleBuyItem()
{
	UE_LOG(LogTemp, Warning, TEXT("You have interacted with buy door"));
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->GetPoints()  >= FCString::Atoi(*ItemPrice) && !bDoorRotated) // set the points to be 2000
	{
		Character->SetPoints(Character->GetPoints() - FCString::Atoi(*ItemPrice));
		FRotator NewRotation = DoorMesh->GetRelativeRotation();
		NewRotation.Yaw += 90.0f; // Rotate by 90 degrees around the Z-axis
		DoorMesh->SetRelativeRotation(NewRotation);
		bDoorRotated = true;
	}
}

ABuyDoor::ABuyDoor()
{
    DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Mesh"));
	DoorMesh->SetupAttachment(RootComponent);
}



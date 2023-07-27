// Fill out your copyright notice in the Description page of Project Settings.


#include "PerkMachine.h"
#include "ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "BaseWeapon.h"
#include "UMG/Public/Components/TextBlock.h"
#include "Algo/Sort.h"
#include "Camera/CameraComponent.h"


APerkMachine::APerkMachine()
{
    PerkMachineMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Perk Machine Mesh"));
	PerkMachineMesh->SetupAttachment(RootComponent);
}

void APerkMachine::PlayConsumeAnimation()
{
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	Character->AnimationCameraComponent->SetActive(true);
	Character->FirstPersonCameraComponent->SetActive(false);
	// UStaticMeshComponent *MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
	MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());


	// Set the static mesh using the exposed property
	if (PerkBottle)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		// GunMesh->AttachToComponent(Mesh1P, AttachmentRules, TEXT("PistolSocket"));
		MeshComponent->SetStaticMesh(PerkBottle);
		MeshComponent->AttachToComponent(Character->Mesh1P, FAttachmentTransformRules::KeepRelativeTransform, TEXT("BottleSocket"));
		MeshComponent->RegisterComponent();
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		
		if (DrinkingAnimationSequence)
		{
			Character->Mesh1P->PlayAnimation(DrinkingAnimationSequence, false);
			// delay of 2 seconds
			PlayAfterDelayDelegate.BindUObject(this, &APerkMachine::PlayConsumeAnimationSecondHalf);
			GetWorldTimerManager().SetTimer(PlayAfterDelayHandle, PlayAfterDelayDelegate, 2.0f, false);
		}
	}
}

void APerkMachine::PlayConsumeAnimationSecondHalf()
{
    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	MeshComponent->DestroyComponent();
	Character->FirstPersonCameraComponent->SetActive(true);
	Character->AnimationCameraComponent->SetActive(false);
	Character->Mesh1P->SetAnimationMode(EAnimationMode::AnimationBlueprint);
}

// void APerkMachine::UsePerk()
// {
    
// }




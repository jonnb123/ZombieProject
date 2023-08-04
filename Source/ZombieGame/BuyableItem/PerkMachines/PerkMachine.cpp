// Fill out your copyright notice in the Description page of Project Settings.


#include "PerkMachine.h"
// #include "ZombieGameCharacter.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"
// #include "BaseWeapon.h"
#include "ZombieGame/BuyableItem/Weapons/BaseWeapon.h"
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
	Character->GetAnimationCameraComponent()->SetActive(true);
	Character->GetFirstPersonCameraComponent()->SetActive(false);
	// UStaticMeshComponent *MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());
	MeshComponent = NewObject<UStaticMeshComponent>(this, UStaticMeshComponent::StaticClass());


	// Set the static mesh using the exposed property
	if (PerkBottle)
	{
		FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
		// GunMesh->AttachToComponent(Mesh1P, AttachmentRules, TEXT("PistolSocket"));
		MeshComponent->SetStaticMesh(PerkBottle);
		MeshComponent->AttachToComponent(Character->GetMesh1P(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("BottleSocket"));
		MeshComponent->RegisterComponent();
		MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		
		if (DrinkingAnimationSequence)
		{
			Character->GetMesh1P()->PlayAnimation(DrinkingAnimationSequence, false);
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
	Character->GetFirstPersonCameraComponent()->SetActive(true);
	Character->GetAnimationCameraComponent()->SetActive(false);
	Character->GetMesh1P()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
}

// void APerkMachine::UsePerk()
// {
    
// }




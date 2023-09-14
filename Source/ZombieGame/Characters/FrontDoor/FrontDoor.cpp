// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieGame/Characters/FrontDoor/FrontDoor.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "UMG/Public/Components/TextBlock.h"

// Sets default values
AFrontDoor::AFrontDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	_RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root Component"));
	RootComponent = _RootComponent;

	// Create the box collision and attach it to the root
	BoxCollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollisionComponent"));
	BoxCollisionComponent->InitBoxExtent(FVector(100.f, 100.f, 100.f));
	BoxCollisionComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	BoxCollisionComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Door Mesh"));
	DoorMesh->SetupAttachment(RootComponent);

}	

// Called when the game starts or when spawned
void AFrontDoor::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &AFrontDoor::OnItemBeginOverlap);
	BoxCollisionComponent->OnComponentEndOverlap.AddDynamic(this, &AFrontDoor::OnItemEndOverlap);
}

void AFrontDoor::OnItemBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
						   int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("OVERLAPPING FRONT DOOR"));
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (OtherActor && OtherActor->IsA<AZombieGameCharacter>())
	{
		if (Character->MainWidgetInstance)
		{
			FString Text = FString::Printf(TEXT("Press 'E' To Open/Close"));
			Character->MainWidgetInstance->EquipItemText->SetText(FText::FromString(Text));
			Character->MainWidgetInstance->EquipItemText->SetVisibility(ESlateVisibility::Visible);

			Character->OverlappingFrontDoor = Cast<AFrontDoor>(OverlappedComponent->GetOwner());

			// UE_LOG(LogTemp, Warning, TEXT("Overlapped Component: %s"), *OverlappedComponent->GetOwner()->GetName());
		}
	}
}

void AFrontDoor::OnItemEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (Character->MainWidgetInstance)
	{
		Character->MainWidgetInstance->EquipItemText->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AFrontDoor::UseFrontDoor()
{

	UE_LOG(LogTemp, Warning, TEXT("You have interacted with buy door"));
    ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	AZombieGameCharacter* Character = Cast<AZombieGameCharacter>(PlayerCharacter);
	if (!bDoorRotated) // set the points to be 2000
	{
        FRotator NewRotation = DoorMesh->GetRelativeRotation();
        NewRotation.Yaw += 90.0f; // Rotate by 90 degrees around the Z-axis
        DoorMesh->SetRelativeRotation(NewRotation);
        bDoorRotated = true;
	}
    else 
    {
        FRotator NewRotation = DoorMesh->GetRelativeRotation();
        NewRotation.Yaw -= 90.0f; // Rotate by 90 degrees around the Z-axis
        DoorMesh->SetRelativeRotation(NewRotation);
        bDoorRotated = false;
    }
}




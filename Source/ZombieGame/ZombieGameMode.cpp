// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieGameMode.h"
#include "Zombie.h"
#include "ZombieAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieGameCharacter.h"
#include "EngineUtils.h" // Include the appropriate header for TActorIterator

void AZombieGameMode::StartGame()
{
    SpawnZombies();
}

void AZombieGameMode::SpawnZombies()
{
    SpawnAmmo();
    ZombieTotal = CurrentWave * 4;
    // ZombieTotal = 0;

    ZombiesLeft = ZombieTotal;

    const FRotator Rotation;

    // opposite corners of the box i want the zombie to spawn in
    FVector CornerPoint1 = FVector(-40870.0, 409560.0, -378590.0);
    FVector CornerPoint2 = FVector(-63790.000000, 392000.0, -379670.0);

    // X center point: -53220 (use x1 + x2 corner points / 2)
    // Y center point: 400780 (use y1 + y2 corner points / 2)
    // Z center point: -379670 (use either z1 or z2 corner points)
    // Overall center point (-53220.0, 400780.0, -378590.0)
    FVector CenterPoint = FVector(-53220.0, 400780.0, -378590.0);
    // float CenterPointZMin = -379930.0;
    // float CenterPointZMax = -379170.0;
    float CenterPointZ = -377330.0;

    // HalfLength: the distance from the centerpoint to cornerpoint1

    float HalfLength = FVector::Dist(CenterPoint, CornerPoint1);

    // Calculating the bounds for the box
    // FVector MinBounds = FVector(CenterPoint.X - HalfLength, CenterPoint.Y - HalfLength, CenterPoint.Z);
    // FVector MaxBounds = FVector(CenterPoint.X + HalfLength, CenterPoint.Y + HalfLength, CenterPoint.Z);

    FVector MinBounds = FVector(CenterPoint.X - HalfLength, CenterPoint.Y - HalfLength, CenterPointZ);
    FVector MaxBounds = FVector(CenterPoint.X + HalfLength, CenterPoint.Y + HalfLength, CenterPointZ);

    TArray<FVector> SpawnLocations; // Array to store the spawn locations

    for (int i = 0; i < ZombieTotal; i++)
    {
        FVector SpawnLocation = FMath::RandPointInBox(FBox(MinBounds, MaxBounds)); // Generate a random point within the defined bounds

        UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), ZombiePawn, BehaviorTree, SpawnLocation); // Spawn a zombie at the random location
    }
}

void AZombieGameMode::ZombiesKilled()
{
    ZombiesLeft--; // zombies left -1
    if (ZombiesLeft == 0)
    {

        WaveIncrement();
    }
}

void AZombieGameMode::WaveIncrement()
{
    if (CurrentWave == MaxWaves)
    {
        UE_LOG(LogTemp, Warning, TEXT("Player has won game!"));
        PlayerWins();
    }
    else
    {
        // once all zombies are killed in the round, a 5 second timer will start.
        FTimerHandle ZombieDestructionTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            ZombieDestructionTimerHandle, [this]()
            {
            for (TActorIterator<AZombie> ActorItr(GetWorld()); ActorItr; ++ActorItr)
            {
                AZombie* Zombie = *ActorItr;
                if (Zombie && !Zombie->IsPendingKill())
                {
                    Zombie->Destroy();
                }
            } },
            5.0f, false); //

        // sets a timer of 10 seconds for both wave start and spawn zombies funcitons.
        FTimerDelegate TimerCallback;
        TimerCallback.BindLambda([this]()
        {
            WaveStart();
            SpawnZombies(); 
        });
        GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerCallback, 10.f, false);
    }
}

void AZombieGameMode::BeginPlay()
{
    Super::BeginPlay();

    StartGame();
}

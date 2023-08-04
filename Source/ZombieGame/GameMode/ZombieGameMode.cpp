// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieGameMode.h"
// #include "Zombie.h"
#include "ZombieGame/Characters/Zombie/Zombie.h"
// #include "ZombieAIController.h"
#include "ZombieGame/Characters/Zombie/ZombieAIController.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
// #include "ZombieGameCharacter.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
// #include "FireZombieBoss.h"
#include "ZombieGame/Characters/Zombie/FireZombieBoss.h"
#include "EngineUtils.h" // Include the appropriate header for TActorIterator

AZombieGameMode::AZombieGameMode()
{
    // Initialize the array with your AI pawn classes
    // ZombiePawnClasses.Add(ZombiePawn1);
    // ZombiePawnClasses.Add(ZombiePawn2);
}

void AZombieGameMode::StartGame()
{
    // SpawnZombies();
}

void AZombieGameMode::SpawnZombies()
{
    SpawnAmmo();

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
    // float CenterPointZ = -377330.0;
    float CenterPointZ = -373777.0;

    // HalfLength: the distance from the centerpoint to cornerpoint1

    float HalfLength = FVector::Dist(CenterPoint, CornerPoint1);

    // Calculating the bounds for the box
    // FVector MinBounds = FVector(CenterPoint.X - HalfLength, CenterPoint.Y - HalfLength, CenterPoint.Z);
    // FVector MaxBounds = FVector(CenterPoint.X + HalfLength, CenterPoint.Y + HalfLength, CenterPoint.Z);

    FVector MinBounds = FVector(CenterPoint.X - HalfLength, CenterPoint.Y - HalfLength, CenterPointZ);
    FVector MaxBounds = FVector(CenterPoint.X + HalfLength, CenterPoint.Y + HalfLength, CenterPointZ);

    TArray<FVector> SpawnLocations; // Array to store the spawn locations

    // for boss rounds
    if (CurrentWave % 5 == 0)
    {
        ZombieTotal = CurrentWave / 5; // Calculate the number of additional zombies to add
        ZombiesLeft = ZombieTotal;

        for (int i = 0; i < ZombieTotal; i++)
        {
            FVector SpawnLocation;
            bool bValidSpawnLocation = false;

            while (!bValidSpawnLocation)
            {
                SpawnLocation = FMath::RandPointInBox(FBox(MinBounds, MaxBounds)); // Generate a random point within the defined bounds

                FVector SweepStart = SpawnLocation + FVector(0.0f, 0.0f, 6500.0f); // Start the sweep slightly above the spawn location
                FVector SweepEnd = SpawnLocation - FVector(0.0f, 0.0f, 6500.0f);   // Sweep downward to find the ground

                FHitResult SweepResult;
                FCollisionQueryParams QueryParams;
                QueryParams.AddIgnoredActor(this); // Ignore the spawner actor during the sweep
                // Used a sweep instead of a line as it's more forgiving, this stopped the zombies spawning under the floor. I found a Sphere of 75.0f to work best.
                if (GetWorld()->SweepSingleByChannel(SweepResult, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(75.0f), QueryParams))
                {
                    // If the sweep hits a static mesh reassign
                    if (SweepResult.GetComponent() && SweepResult.GetComponent()->IsA<UStaticMeshComponent>())
                    {
                        // Do nothing and keep it as a non valid location.
                        continue;
                    }
                    else
                    {
                        // else it is a valid spawn location
                        bValidSpawnLocation = true;
                        // Adjust the zombie's Z-coordinate to the ground level
                        FVector GroundLocation = SweepResult.Location;
                        SpawnLocation.Z = GroundLocation.Z;
                    }
                }
            }

            // Spawn the zombie at the adjusted location
            UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), FireZombiePawn, BehaviorTree, SpawnLocation);
        }
    }

    else // for normal zombie rounds
    {
        // tried adding to the array in constructor but didn't work 
        ZombiePawnClasses.Add(ZombiePawn1); 
        ZombiePawnClasses.Add(ZombiePawn2);
        ZombiePawnClasses.Add(ZombiePawn3);
        ZombiePawnClasses.Add(ZombiePawn4);

        ZombieTotal = CurrentWave * 4;
        ZombiesLeft = ZombieTotal;

        for (int i = 0; i < ZombieTotal; i++)
        {
            FVector SpawnLocation;
            bool bValidSpawnLocation = false;

            while (!bValidSpawnLocation) // while bvaildspawnlocation is false
            {
                SpawnLocation = FMath::RandPointInBox(FBox(MinBounds, MaxBounds)); // Generate a random point within the defined bounds

                FVector SweepStart = SpawnLocation + FVector(0.0f, 0.0f, 6500.0f); // Start the sweep slightly above the spawn location
                FVector SweepEnd = SpawnLocation - FVector(0.0f, 0.0f, 6500.0f);   // Sweep downward to find the ground

                FHitResult SweepResult;
                FCollisionQueryParams QueryParams;
                QueryParams.AddIgnoredActor(this); // Ignore the spawner actor during the sweep

                if (GetWorld()->SweepSingleByChannel(SweepResult, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(75.0f), QueryParams))
                {
                    // If the sweep hits a static mesh reassign
                    if (SweepResult.GetComponent() && SweepResult.GetComponent()->IsA<UStaticMeshComponent>())
                    {
                        // Do nothing and keep it as a non valid location.
                        continue;
                    }
                    else
                    {
                        // else it is a valid spawn location
                        bValidSpawnLocation = true;
                        // Adjust the zombie's Z-coordinate to the ground level
                        FVector GroundLocation = SweepResult.Location;
                        SpawnLocation.Z = GroundLocation.Z;
                    }
                }
            }
            // Select a random pawn class from the array
            TSubclassOf<APawn> RandomPawnClass = ZombiePawnClasses[FMath::RandRange(0, ZombiePawnClasses.Num() - 1)];
            // Spawn the zombie at the adjusted location
            UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), RandomPawnClass, BehaviorTree, SpawnLocation);
        }
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
        UE_LOG(LogTemp, Warning, TEXT("Player has won the game!"));
        PlayerWins();
    }
    else
    {
        // Spawn regular zombies
        // ...

        // Once all zombies are killed in the round, a 5-second timer will start.
        FTimerHandle ZombieDestructionTimerHandle;
        GetWorld()->GetTimerManager().SetTimer(
            ZombieDestructionTimerHandle, [this]()
            {
                for (TActorIterator<AZombie> ZombieItr(GetWorld()); ZombieItr; ++ZombieItr)
                {
                    AZombie* Zombie = *ZombieItr;
                    Zombie->Destroy();
                } },
            5.0f, false);

        // Sets a timer of 10 seconds for both WaveStart and SpawnZombies functions.
        FTimerDelegate TimerCallback;
        TimerCallback.BindLambda([this]()
                                 {
                WaveStart();
                SpawnZombies(); });

        GetWorld()->GetTimerManager().SetTimer(TimerHandle, TimerCallback, 10.f, false);
    }
}

void AZombieGameMode::BeginPlay()
{
    Super::BeginPlay();

    StartGame();
}

void AZombieGameMode::WaveStart()
{
    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
    CurrentWave += 1;
    if (Character->MainWidgetInstance)
    {
        Character->MainWidgetInstance->ShowWaveStart(CurrentWave);
    }
}

void AZombieGameMode::PlayerWins()
{
    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
    APlayerController *PlayerController = Cast<APlayerController>(Character->GetController());
    if (Character->MainWidgetInstance)
    {
        Character->GetCharacterMovement()->StopMovementImmediately();
        PlayerController->SetInputMode(FInputModeUIOnly());
        PlayerController->bShowMouseCursor = true;
        Character->MainWidgetInstance->ShowWinText();
    }
}
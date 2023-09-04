// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieGameMode.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieGame/Characters/PlayerCharacter/ZombieGameCharacter.h"
#include "EngineUtils.h" // Include the appropriate header for TActorIterator

void AZombieGameMode::BeginPlay()
{
    Super::BeginPlay();

    StartGame();
}

void AZombieGameMode::StartGame()
{
    // SpawnZombies();
}

void AZombieGameMode::SpawnZombies()
{
    // these are all values to make up a box in the level for the zombies to spawn
    FVector const MinBounds = FVector(-40870.0, 392000.0, -379670.0);
    FVector const MaxBounds = FVector(-63790.000000, 409560.0, -378590.0);
    FVector const CenterPoint = FVector(-53220.0, 400780.0, -378590.0);
    float const CenterPointZ = -373777.0;
    float const HalfLength = FVector::Dist(CenterPoint, MinBounds);

    ZombiePawnClasses = {ZombiePawn1, ZombiePawn2, ZombiePawn3, ZombiePawn4};

    if (CurrentWave % 5 == 0)
    {
        ZombieTotal = CurrentWave / 5;
    }
    else
    {
        ZombieTotal = CurrentWave * 4;
    }

    ZombiesLeft = ZombieTotal;

    for (int i = 0; i < ZombieTotal; i++)
    {
        FVector SpawnLocation;
        bool bValidSpawnLocation = false;

        while (!bValidSpawnLocation)
        {
            SpawnLocation = FMath::RandPointInBox(FBox(MinBounds, MaxBounds));
            FVector SweepStart = SpawnLocation + FVector(0.0f, 0.0f, 6500.0f);
            FVector SweepEnd = SpawnLocation - FVector(0.0f, 0.0f, 6500.0f);
            FHitResult SweepResult;
            FCollisionQueryParams QueryParams;
            QueryParams.AddIgnoredActor(this);

            if (GetWorld()->SweepSingleByChannel(SweepResult, SweepStart, SweepEnd, FQuat::Identity, ECC_WorldStatic, FCollisionShape::MakeSphere(75.0f), QueryParams))
            {
                if (SweepResult.GetComponent() && SweepResult.GetComponent()->IsA<UStaticMeshComponent>())
                {
                    continue; // if the sweep hits something like a tree, i.e. invalid
                }
                else
                {
                    bValidSpawnLocation = true;
                    FVector GroundLocation = SweepResult.Location;
                    SpawnLocation.Z = GroundLocation.Z;
                }
            }
        }

        TSubclassOf<APawn> SpawnPawnClass;
        if (CurrentWave % 5 == 0)
        {
            SpawnPawnClass = FireZombiePawn;
        }
        else
        {
            SpawnPawnClass = ZombiePawnClasses[FMath::RandRange(0, ZombiePawnClasses.Num() - 1)];
        }

        UAIBlueprintHelperLibrary::SpawnAIFromClass(GetWorld(), SpawnPawnClass, BehaviorTree, SpawnLocation);
    }
}

void AZombieGameMode::HandleZombieCountAndRound()
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

        // Sets a timer of 10 seconds for both HandleWaveStart
        FTimerHandle RoundStartTimerHandle;
        FTimerDelegate TimerCallback;
        TimerCallback.BindLambda([this]()
                                 { HandleWaveStart(); });
        GetWorld()->GetTimerManager().SetTimer(RoundStartTimerHandle, TimerCallback, 10.f, false);
    }
}

void AZombieGameMode::HandleWaveStart()
{
    CurrentWave ++;
    SpawnZombies();
    SpawnAmmo();

    ACharacter *PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
    AZombieGameCharacter *Character = Cast<AZombieGameCharacter>(PlayerCharacter);
   
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
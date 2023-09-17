// Fill out your copyright notice in the Description page of Project Settings.


#include "ZombieGame/Characters/Dog/AI/DogAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieGame/Characters/Zombie/Zombie.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "ZombieGame/GameMode/ZombieGameMode.h"


void ADogAIController::BeginPlay()
{
    Super::BeginPlay();

    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AZombie::StaticClass(), AllZombies);

    // if (AIPerceptionComponent != nullptr)
    // {
    //     AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &ADogAIController::EnemyDetected);
    // }


    if (AIBehavior != nullptr)
    {
        RunBehaviorTree(AIBehavior);
    }
}

void ADogAIController::Tick(float DeltaSeconds)
{
    // if (AIPerceptionComponent->HasAnyActiveStimulus(*Zombie))
    // {
    //     GetBlackboardComponent()->SetValueAsVector(TEXT("ZombieLocation"), Zombie->GetActorLocation());
    //     // AZombie* ZombieNew = Cast<AZombie>(Zombie);
    //     // UCharacterMovementComponent *ZombieMovement = Cast<UCharacterMovementComponent>(ZombieNew->GetMovementComponent());
    //     // ZombieMovement->MaxWalkSpeed = 300;
    // }
    // else
    // {
    //     // AZombie* ZombieNew = Cast<AZombie>(Zombie);
    //     // UCharacterMovementComponent *ZombieMovement = Cast<UCharacterMovementComponent>(ZombieNew->GetMovementComponent());
    //     // ZombieMovement->MaxWalkSpeed = 60;
    // }

    Super::Tick(DeltaSeconds);
    if (AllZombies.Num() > 0)
    {
	    AZombieGameMode *MyMode = Cast<AZombieGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
        AZombie* ClosestZombie = MyMode->ClosestZombie = GetClosestZombie();
        if (ClosestZombie && ClosestZombie->GetIsZombieDead() == false)
        {
            GetBlackboardComponent()->SetValueAsVector(TEXT("ZombieLocation"), ClosestZombie->GetActorLocation());
            UCharacterMovementComponent *DogMovement = Cast<UCharacterMovementComponent>(GetPawn()->GetMovementComponent());
            DogMovement->MaxWalkSpeed = 150.f;
        }
    }

}

ADogAIController::ADogAIController()
{
    AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
}


// // // // Bound on begin play if there's an AIPerceptionComponent
// void ADogAIController::EnemyDetected(AActor *Actor, FAIStimulus Stimulus)
// {

//     Zombie = Cast<AZombie>(Actor);
    
//     Zombie = Actor;
//     if (Zombie == nullptr)
//         return;
//     UE_LOG(LogTemp, Warning, TEXT("THIS IS WORKING"));
//     // AZombie *ZombieCharacter = Cast<AZombie>(GetPawn());
// }

AZombie* ADogAIController::GetClosestZombie()
{
    AZombie* ClosestZombie = nullptr;
    float MinDistance = MAX_FLT;
    FVector DogLocation = GetPawn()->GetActorLocation();


    for (AActor* Actor : AllZombies)
    {
        AZombie* Zombie = Cast<AZombie>(Actor);
        if (Zombie && !Zombie->GetIsZombieDead())
        {
            float Distance = FVector::Dist(Actor->GetActorLocation(), DogLocation);
            if (Distance < MinDistance)
            {
                ClosestZombie = Zombie;
                MinDistance = Distance;
            }
        }
    }
    return ClosestZombie;
}
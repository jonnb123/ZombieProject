// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieGame/Characters/Dog/AI/BTTask_AttackZombie.h"
#include "ZombieGame/Characters/Zombie/Zombie.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieGame/GameMode/ZombieGameMode.h"
#include "ZombieGame/Characters/Dog/Dog.h"
#include "AIController.h"

UBTTask_AttackZombie::UBTTask_AttackZombie()
{
    NodeName = TEXT("AttackZombie");
}

EBTNodeResult::Type UBTTask_AttackZombie::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    // Stores a reference for OwnerComp for use in OnAttackEnd
    CachedOwnerComp = &OwnerComp;

    AZombieGameMode *MyMode = Cast<AZombieGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
    AZombie *Zombie = MyMode->ClosestZombie;

    FDamageEvent DamageEvent;

    // gets the dog
    AAIController *AIController{OwnerComp.GetAIOwner()};
    const APawn *AIPawn{AIController->GetPawn()};
    ACharacter *AICharacter{AIController->GetCharacter()};
    ADog *DogCharacter = Cast<ADog>(AICharacter);

    UAnimInstance *AnimInstance = AICharacter->GetMesh()->GetAnimInstance();

    if (AnimInstance)
    {
        // Bind a delegate function to the OnMontageEnded event
        AnimInstance->OnMontageEnded.AddDynamic(this, &UBTTask_AttackZombie::OnAttackEnd);
    }

    if (Zombie)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), DogAttackSound, AICharacter->GetActorLocation());
        AICharacter->PlayAnimMontage(AttackMontage);
        Zombie->TakeDamage(BiteDamage, DamageEvent, AIController, AICharacter);
    }
    else
    {
        
    }
    return EBTNodeResult::InProgress;
}

void UBTTask_AttackZombie::OnAttackEnd(UAnimMontage *Montage, bool bInterrupted)
{
    UE_LOG(LogTemp, Warning, TEXT("DOG IS ATTACKING"));
    FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}

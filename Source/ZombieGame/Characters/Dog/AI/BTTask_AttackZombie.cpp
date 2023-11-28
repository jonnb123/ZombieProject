// Fill out your copyright notice in the Description page of Project Settings.

#include "ZombieGame/Characters/Dog/AI/BTTask_AttackZombie.h"
#include "ZombieGame/Characters/Zombie/Zombie.h"
#include "Kismet/GameplayStatics.h"
#include "ZombieGame/GameMode/ZombieGameMode.h"
#include "ZombieGame/Characters/Dog/Dog.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "ZombieGame/Characters/Zombie/Zombie.h"

UBTTask_AttackZombie::UBTTask_AttackZombie()
{
    NodeName = TEXT("AttackZombie");
}

EBTNodeResult::Type UBTTask_AttackZombie::ExecuteTask(UBehaviorTreeComponent &OwnerComp, uint8 *NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    // Stores a reference for OwnerComp for use in OnAttackEnd
    CachedOwnerComp = &OwnerComp;
    
    AIController = OwnerComp.GetAIOwner();
    
    const UBlackboardComponent* BlackboardComponent = OwnerComp.GetBlackboardComponent();
    ZombieActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("Zombie"));
    const AZombie* Zombie = Cast<AZombie>(ZombieActor);
    
    // get the character
    ACharacter *AICharacter = AIController->GetCharacter();
    
    if (UAnimInstance *AnimInstance = AICharacter->GetMesh()->GetAnimInstance())
    {
        // Bind a delegate function to the OnMontageEnded event
        AnimInstance->OnMontageEnded.AddDynamic(this, &UBTTask_AttackZombie::OnAttackEnd);
    }

    if (ZombieActor)
    {
        UGameplayStatics::PlaySoundAtLocation(GetWorld(), DogAttackSound, AICharacter->GetActorLocation());
        AICharacter->PlayAnimMontage(AttackMontage);
        if (IDamageableInterface *TheInterface = Cast<IDamageableInterface>(ZombieActor))
        {
            FDamageEvent DamageEvent;
            TheInterface->HandleDamage(BiteDamage, DamageEvent, AIController, ZombieActor);
        }
    }
    else
    {
        // do nothing
    }
    return EBTNodeResult::InProgress;
}


void UBTTask_AttackZombie::OnAttackEnd(UAnimMontage *Montage, bool bInterrupted)
{
    UE_LOG(LogTemp, Warning, TEXT("DOG IS ATTACKING"));
    
    FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
}

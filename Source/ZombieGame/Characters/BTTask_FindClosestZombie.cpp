// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindClosestZombie.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryManager.h"
#include "Zombie/Zombie.h"

UBTTask_FindClosestZombie::UBTTask_FindClosestZombie()
{
	NodeName = TEXT("FindClosestZombie");
}

EBTNodeResult::Type UBTTask_FindClosestZombie::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	// Stores a reference for OwnerComp for use in OnAttackEnd
	CachedOwnerComp = &OwnerComp;
	AIController = Cast<AAIController>(OwnerComp.GetAIOwner());

	FEnvQueryRequest HidingSpotQueryRequest = FEnvQueryRequest(FindZombieEQS, AIController->GetPawn());
	HidingSpotQueryRequest.Execute(EEnvQueryRunMode::SingleResult, this, &UBTTask_FindClosestZombie::ZombieQueryResult);
	return EBTNodeResult::InProgress;
}

void UBTTask_FindClosestZombie::ZombieQueryResult(TSharedPtr<FEnvQueryResult> Result)
{
	if (Result->IsSuccessful())
	{
		AActor *PreferredActor = Result->GetItemAsActor(0);
		if (AZombie *ClosestZombie = Cast<AZombie>(PreferredActor))
		{
			UBlackboardComponent* BlackboardComponent = CachedOwnerComp->GetBlackboardComponent();
			UObject* ZombieObject = Cast<UObject>(ClosestZombie);
			BlackboardComponent->SetValueAsObject("Zombie", ZombieObject);
			FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
		}
	}
}

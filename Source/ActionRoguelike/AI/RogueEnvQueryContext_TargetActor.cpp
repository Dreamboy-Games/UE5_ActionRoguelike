// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueEnvQueryContext_TargetActor.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "EnvironmentQuery/EnvQueryTypes.h"
#include "RogueGameTypes.h"
#include "EnvironmentQuery/Items/EnvQueryItemType_Actor.h"

void URogueEnvQueryContext_TargetActor::ProvideContext(FEnvQueryInstance& QueryInstance, FEnvQueryContextData& ContextData) const
{
	// Expected Character
	const APawn* QuerierPawn = Cast<APawn>(QueryInstance.Owner.Get());
	if (ensure(QuerierPawn))
	{
		AAIController* Controller = Cast<AAIController>(QuerierPawn->GetController());
		check(Controller);
		
		const AActor* TargetActor = Cast<AActor>(Controller->GetBlackboardComponent()->GetValueAsObject(NAME_TargetActor));
		UEnvQueryItemType_Actor::SetContextHelper(ContextData, TargetActor);
	}
}

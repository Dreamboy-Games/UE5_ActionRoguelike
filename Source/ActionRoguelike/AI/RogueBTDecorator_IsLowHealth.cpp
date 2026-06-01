// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueBTDecorator_IsLowHealth.h"

#include "AIController.h"
#include "RogueGameplayTags.h"
#include "ActionSystem/RogueActionSystemComponent.h"

bool URogueBTDecorator_IsLowHealth::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	const APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	check(Pawn);
	
	URogueActionSystemComponent* ActionComp = Pawn->GetComponentByClass<URogueActionSystemComponent>();
	if (ensure(ActionComp))
	{
		// Is low health?
		const float HealthFraction = ActionComp->GetAttributeValue(SharedGameplayTags::Attribute_Health) / ActionComp->GetAttributeValue(SharedGameplayTags::Attribute_HealthMax);
		return HealthFraction < LowHealthFraction;
	}
	
	return false;
}

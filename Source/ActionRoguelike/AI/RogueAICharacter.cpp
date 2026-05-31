// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAICharacter.h"

#include "RogueGameplayTags.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "ActionSystem/RogueAttributeSet.h"


ARogueAICharacter::ARogueAICharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>(TEXT("ActionSystemComponent"));
	ActionSystemComponent->SetDefaultAttributeSet(URogueMonsterAttributeSet::StaticClass());
}

float ARogueAICharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
	class AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	ActionSystemComponent->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, -ActualDamage, Base);
	
	return ActualDamage;
}

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

void ARogueAICharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	GetMesh()->SetOverlayMaterialMaxDrawDistance(1);
}

float ARogueAICharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                    class AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	ActionSystemComponent->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, -ActualDamage, Base);
	
	GetMesh()->SetOverlayMaterialMaxDrawDistance(0);
	// Material Instance Dynamic
	// GetMesh()->SetScalarParameterValueOnMaterials("TimeToHit", GetWorld()->GetWorld()->TimeSeconds);
	
	// Custom Primitive Data
	GetMesh()->SetCustomPrimitiveDataFloat(0, GetWorld()->TimeSeconds);
	
	GetWorldTimerManager().SetTimer(OverlayTimerHandle, [this]()
	{
		if (IsValid(this))
		{
			GetMesh()->SetOverlayMaterialMaxDrawDistance(1);
		}
	}, 1.0f, false); 
	
	return ActualDamage;
}

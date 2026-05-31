// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAttributeSet.h"

#include "RogueActionSystemComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


URogueActionSystemComponent* URogueAttributeSet::GetOwningComponent() const
{
	return Cast<URogueActionSystemComponent>(GetOuter());
}

URogueHealthAttributeSet::URogueHealthAttributeSet()
{
	Health = FRogueAttribute(100);
	HealthMax = FRogueAttribute(100);
}

void URogueHealthAttributeSet::PostAttributeChanged()
{
	Health.Base = FMath::Clamp(Health.Base, 0.0f, HealthMax.GetValue());
}


URoguePawnAttributeSet::URoguePawnAttributeSet()
{
	MoveSpeed = FRogueAttribute(550);
	MoveSpeedMultiplier = FRogueAttribute(1.0f);
}
void URoguePawnAttributeSet::InitializeAttributes()
{
	Super::InitializeAttributes();
	ApplyMovementSpeed();
}
void URoguePawnAttributeSet::PostAttributeChanged()
{
	Super::PostAttributeChanged();
	ApplyMovementSpeed();
}
void URoguePawnAttributeSet::ApplyMovementSpeed() const
{
	const ACharacter* OwningCharacter = Cast<ACharacter>(GetOwningComponent()->GetOwner());
	OwningCharacter->GetCharacterMovement()->MaxWalkSpeed = MoveSpeed.GetValue() * MoveSpeedMultiplier.GetValue();
}


URoguePlayerAttributeSet::URoguePlayerAttributeSet()
{
	Rage = FRogueAttribute(0);
}

URogueMonsterAttributeSet::URogueMonsterAttributeSet()
{
	MoveSpeed = FRogueAttribute(450);
}

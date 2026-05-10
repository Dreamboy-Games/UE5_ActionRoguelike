// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionSystemComponent.h"


URogueActionSystemComponent::URogueActionSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URogueActionSystemComponent::ApplyHealthChange(float InValueChange)
{
	const float OldHealth = Attributes.Health;
	
	//Attributes.Health += InValueChange;
	Attributes.Health = FMath::Clamp(Attributes.Health + InValueChange, 0.0f, Attributes.HealthMax);

	if (!FMath::IsNearlyEqual(OldHealth, Attributes.Health))
	{
		OnHealthChanged.Broadcast(Attributes.Health, OldHealth);
	}
	
	UE_LOG(LogTemp, Log, TEXT("New health: %f / %f"), Attributes.Health, Attributes.HealthMax);
}

bool URogueActionSystemComponent::IsFullHealth() const
{
	return FMath::IsNearlyEqual(Attributes.HealthMax, Attributes.Health);
}

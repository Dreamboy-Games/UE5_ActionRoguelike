// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionSystemComponent.h"


URogueActionSystemComponent::URogueActionSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void URogueActionSystemComponent::ApplyHealthChange(float InValueChange)
{
	const float MaxHealth = GetDefault<URogueActionSystemComponent>()->Attributes.Health;
	const float OldHealth = Attributes.Health;
	
	//Attributes.Health += InValueChange;
	Attributes.Health = FMath::Clamp(Attributes.Health + InValueChange, 0.0f, MaxHealth);

	if (!FMath::IsNearlyEqual(OldHealth, Attributes.Health))
	{
		OnHealthChanged.Broadcast(Attributes.Health, OldHealth);
	}
	
	UE_LOG(LogTemp, Log, TEXT("New health: %f / %f"), Attributes.Health, MaxHealth);
}

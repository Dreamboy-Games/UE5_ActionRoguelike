// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAction.h"

#include "ActionRoguelike.h"
#include "RogueActionSystemComponent.h"


void URogueAction::StartAction_Implementation()
{
	bIsRunning = true;
	
	float GameTime = GetWorld()->GetTimeSeconds();
	
	UE_LOGFMT(LogGame, Log, "Started Action:  {ActionName} - {WorldTime}", ("ActionName", ActionName.ToString()), ("WorldTime", GameTime));
	
	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantTags);
	
	// Consume required resources
	for (const TPair<FGameplayTag, float> Cost : ActivationCost)
	{
		GetOwningComponent()->ApplyAttributeChange(Cost.Key, -Cost.Value, Modifer);
	}
}

void URogueAction::StopAction_Implementation()
{
	bIsRunning = false;
	
	float GameTime = GetWorld()->GetTimeSeconds();
	
	UE_LOGFMT(LogGame, Log, "Stopped Action:  {ActionName} - {WorldTime}", ("ActionName", ActionName.ToString()), ("WorldTime", GameTime));
	
	CooldownUntil = GetWorld()->TimeSeconds + CooldownTime;
	
	GetOwningComponent()->ActiveGameplayTags.RemoveTags(GrantTags);
}

URogueActionSystemComponent* URogueAction::GetOwningComponent() const
{
	return Cast<URogueActionSystemComponent>(GetOuter());
}

bool URogueAction::CanStart() const
{
	if (IsRunning())
	{
		return false;
	}
	
	if (GetCooldownTimeRemaining() > 0.0f)
	{
		UE_LOG(LogGame, Log, TEXT("Cooldown remining: %f"), GetCooldownTimeRemaining());
		return false;
	}
	
	if (GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	
	for (TPair<FGameplayTag, float> Cost : ActivationCost)
	{
		float AvailableAttributeAmount = GetOwningComponent()->GetAttributeValue(Cost.Key);
		if (AvailableAttributeAmount < Cost.Value)
		{
			// Not enough resources
			UE_LOGFMT(LogGame, Log, "Not enough {AttributeName} to activate {ActionName}." "Have {AvailableAttributeValue} and need {RequiredAttributeValue}",
				("AttributeName", Cost.Key.ToString()),
				("ActionName", ActionName.ToString()),
				("AvailableAttributeValue", AvailableAttributeAmount),
				("RequiredAttributeValue", Cost.Value));
			return false;
		}
	}
	
	return true;
}

float URogueAction::GetCooldownTimeRemaining() const
{
	return FMath::Max(0.0f, CooldownUntil - GetWorld()->GetTimeSeconds());
}


// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueAction.h"
#include "RogueActionSystemComponent.h"


void URogueAction::StartAction_Implementation()
{
	bIsRunning = true;
	
	float GameTime = GetWorld()->GetTimeSeconds();
	
	UE_LOGFMT(LogTemp, Log, "Started Action:  {ActionName} - {WorldTime}", ("ActionName", ActionName.ToString()), ("WorldTime", GameTime));
	
	GetOwningComponent()->ActiveGameplayTags.AppendTags(GrantTags);
}

void URogueAction::StopAction_Implementation()
{
	bIsRunning = false;
	
	float GameTime = GetWorld()->GetTimeSeconds();
	
	UE_LOGFMT(LogTemp, Log, "Stopped Action:  {ActionName} - {WorldTime}", ("ActionName", ActionName.ToString()), ("WorldTime", GameTime));
	
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
		UE_LOG(LogTemp, Log, TEXT("Cooldown remining: %f"), GetCooldownTimeRemaining());
		return false;
	}
	
	if (GetOwningComponent()->ActiveGameplayTags.HasAny(BlockedTags))
	{
		return false;
	}
	
	return true;
}

float URogueAction::GetCooldownTimeRemaining() const
{
	return FMath::Max(0.0f, CooldownUntil - GetWorld()->GetTimeSeconds());
}


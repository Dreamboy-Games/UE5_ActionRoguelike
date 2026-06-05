// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionSystemComponent.h"

#include "ActionRoguelike.h"
#include "GameplayTagContainer.h"

#include "RogueAction.h"
#include "RogueActionEffect.h"
#include "RogueAttributeSet.h"
#include "RogueGameplayTags.h"


URogueActionSystemComponent::URogueActionSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
}

void URogueActionSystemComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	// Fallback for Blueprint and CPP having not yet defined a default AttributeSet
	if (Attributes == nullptr)
	{
		Attributes = NewObject<URogueAttributeSet>(this, URogueAttributeSet::StaticClass());
		UE_LOG(LogGame, Warning, TEXT("No default AttributeSet defined. "
			"Set using SetDefaultAttributeSet() during Actor construction or assign in Blueprint ActionComponent for %s."), *GetNameSafe(GetOwner()));
	}
	
	// Iterate all the available UPROPERTY members of the class <URogueAttributeSet> Attributes
	// Add all members to CachedAttributes Map with GameplayTag as key-value pair
	for (TFieldIterator<FStructProperty> PropIt(Attributes->GetClass()); PropIt; ++PropIt)
	{
		FRogueAttribute* FoundAttribute = PropIt->ContainerPtrToValuePtr<FRogueAttribute>(Attributes);
		
		FName AttributeTagName = FName("Attribute." + PropIt->GetName());
		FGameplayTag AttributeTag = FGameplayTag::RequestGameplayTag(AttributeTagName);
		
		CachedAttributes.Add(AttributeTag, FoundAttribute);
	}
	
	// Assign Actions to this component
	for (const TSubclassOf<URogueAction> ActionClass : DefaultActions)
	{
		if (ensure(ActionClass))
		{
			GrantAction(ActionClass);
		}
	}
}

void URogueActionSystemComponent::SetDefaultAttributeSet(TSubclassOf<URogueAttributeSet> AttributeSetClass)
{
	check(!HasBeenInitialized());
	
	// Only available during constructors of UObjects
	const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get();
	Attributes = Cast<URogueAttributeSet>(ObjectInitializer.CreateDefaultSubobject(this, TEXT("Attributes"), AttributeSetClass, AttributeSetClass));
}

void URogueActionSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	Attributes->InitializeAttributes();
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ACTIONS
/////////////////////////////////////////////////////////////////////////////////////////////////////////
void URogueActionSystemComponent::StartAction(const FGameplayTag InActionName)
{
	for (URogueAction* Action : Actions)
	{
		if (Action->GetActionName() == InActionName)
		{
			if (Action->CanStart())
			{
				Action->StartAction();
			}
			return;
		}
	}
	
	UE_LOG(LogGame, Warning, TEXT("Action '%s' not found"), *InActionName.ToString());
}

void URogueActionSystemComponent::StopAction(const FGameplayTag InActionName)
{
	for (URogueAction* Action : Actions)
	{
		if (Action->GetActionName() == InActionName)
		{
			if (Action->IsRunning())
			{
				Action->StopAction();
			}
			return;
		}
	}
	
	UE_LOG(LogGame, Warning, TEXT("Action '%s' not found"), *InActionName.ToString());
}

void URogueActionSystemComponent::GrantAction(TSubclassOf<URogueAction> NewActionClass)
{
	URogueAction* NewAction = NewObject<URogueAction>(this, NewActionClass);
	Actions.Add(NewAction);
	
	if (NewAction->IsA(URogueActionEffect::StaticClass()))
	{
		// Sanity check that buffs are allowed to run. W do not handle this case yet.
		ensureMsgf(NewAction->CanStart(), TEXT("Effect can not start CanStart returns FALSE. Case not handled."));
		NewAction->StartAction();
	}
}

void URogueActionSystemComponent::RemoveAction(URogueAction* ActionToRemove)
{
	const int32 RemoveCount = Actions.RemoveSingle(ActionToRemove);
	ensure(RemoveCount == 1);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ATTRIBUTES
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Convenience function to access any members within <URogueAttributeSet> class,
// regardless of which derived Attribute Set they might exist in
FRogueAttribute* URogueActionSystemComponent::GetAttribute(const FGameplayTag InAttributeTag) const
{
	FRogueAttribute* FoundAttribute = *CachedAttributes.Find(InAttributeTag);
	return FoundAttribute;
}

float URogueActionSystemComponent::GetAttributeValue(const FGameplayTag InAttributeTag) const
{
	return GetAttribute(InAttributeTag)->GetValue();
}

void URogueActionSystemComponent::ApplyAttributeChange(const FGameplayTag AttributeTag, const float Delta, EAttributeModifyType ModifyType)
{
	FRogueAttribute* FoundAttribute = GetAttribute(AttributeTag);
	check(FoundAttribute);
	
	float OldValue = FoundAttribute->GetValue();

	switch (ModifyType)
	{
	case Base:
		FoundAttribute->Base += Delta;
		break;
	case Modifer:
		FoundAttribute->Modifier += Delta;
		break;
	case OverrideBase:
		FoundAttribute->Base = Delta;
		break;
	default:
		check(false);
	}
	
	Attributes->PostAttributeChanged();

	// Native C++ listeners
	if (const FOnAttributeChanged* Event = AttributeListeners.Find(AttributeTag))
	{
		Event->Broadcast(AttributeTag, FoundAttribute->GetValue(), OldValue);
	}
	// Blueprint listeners
	if (TArray<FOnAttributeDynamicChanged>* Events = AttributeDynamicListeners.Find(AttributeTag))
	{		
		for (int i = Events->Num() - 1; i >= 0; --i) // Reverse for loop. Start at the back.
		{
			FOnAttributeDynamicChanged& Event = (*Events)[i];
			bool bIsBound = Event.ExecuteIfBound(AttributeTag,  FoundAttribute->GetValue(), OldValue);
			if (!bIsBound)
			{
				Events->RemoveAt(i);
				UE_LOG(LogGame, Warning, TEXT("Cleaned up expired attribute delegate for %s"), *GetNameSafe(GetOwner()));
			}
		}
	}
	
	UE_LOGFMT(LogGame, Log, "Attribute: {0}, New: {1}, Old: {2}", AttributeTag.ToString(), FoundAttribute->GetValue(), OldValue);
}

FOnAttributeChanged& URogueActionSystemComponent::GetAttributeListener(FGameplayTag AttributeTag)
{
	return AttributeListeners.FindOrAdd(AttributeTag);
}

void URogueActionSystemComponent::AddDynamicAttributeListener(const FOnAttributeDynamicChanged Event,	const FGameplayTag AttributeTag)
{
	TArray<FOnAttributeDynamicChanged>& Events = AttributeDynamicListeners.FindOrAdd(AttributeTag);
	Events.Add(Event);
}

void URogueActionSystemComponent::RemoveDynamicAttributeListener(const FOnAttributeDynamicChanged Event)
{
	for (TPair<FGameplayTag, TArray<FOnAttributeDynamicChanged>>& Listener : AttributeDynamicListeners)
	{
		if (Listener.Value.RemoveSingle(Event) > 0)
		{
			UE_LOG(LogGame, Warning, TEXT("Successfully removed blueprint binding from AttributeDynamicListeners."))
			break;
		}
		
	}
}

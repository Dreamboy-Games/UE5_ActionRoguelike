// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueActionSystemComponent.h"
#include "GameplayTagContainer.h"

#include "RogueAction.h"
#include "RogueAttributeSet.h"
#include "RogueGameplayTags.h"


URogueActionSystemComponent::URogueActionSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	
	AttributeSetClass = URogueAttributeSet::StaticClass();
}

void URogueActionSystemComponent::InitializeComponent()
{
	Super::InitializeComponent();
	
	Attributes = NewObject<URogueAttributeSet>(this, AttributeSetClass);
	
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
	
	UE_LOG(LogTemp, Warning, TEXT("Action '%s' not found"), *InActionName.ToString());
}

void URogueActionSystemComponent::StopAction(const FGameplayTag InActionName)
{
	for (URogueAction* Action : Actions)
	{
		if (Action->GetActionName() == InActionName)
		{
			Action->StopAction();
			return;
		}
	}
	
	UE_LOG(LogTemp, Warning, TEXT("Action '%s' not found"), *InActionName.ToString());
}

void URogueActionSystemComponent::GrantAction(TSubclassOf<URogueAction> NewActionClass)
{
	URogueAction* NewAction = NewObject<URogueAction>(this, NewActionClass);
	Actions.Add(NewAction);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
// ATTRIBUTES
/////////////////////////////////////////////////////////////////////////////////////////////////////////

// Convenience function to access all members within <URogueAttributeSet> class,
// regardless of which derived Attribute Set they might exist in
FRogueAttribute* URogueActionSystemComponent::GetAttribute(const FGameplayTag InAttributeTag) const
{
	FRogueAttribute* FoundAttribute = *CachedAttributes.Find(InAttributeTag);
	return FoundAttribute;
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

	if (const FOnAttributeChanged* Event = AttributeListeners.Find(AttributeTag))
	{
		Event->Broadcast(AttributeTag, FoundAttribute->GetValue(), OldValue);
	}
	
	UE_LOGFMT(LogTemp, Log, "Attribute: {0}, New: {1}, Old: {2}", AttributeTag.ToString(), FoundAttribute->GetValue(), OldValue);
}

FOnAttributeChanged& URogueActionSystemComponent::GetAttributeListener(FGameplayTag AttributeTag)
{
	return AttributeListeners.FindOrAdd(AttributeTag);
}

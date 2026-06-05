// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "RogueActionSystemComponent.generated.h"


struct FRogueAttribute;
class URogueAttributeSet;
struct FGameplayTag;
class URogueAction;


UENUM(BlueprintType)
enum EAttributeModifyType {
	Base,
	Modifer,
	OverrideBase,
	Invalid
};

// Native C++ delegate
DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, FGameplayTag /*AttributeTag*/, float /*NewAttributeValue*/, float /*OldAttributeValue*/);
// Blueprint delegate
DECLARE_DYNAMIC_DELEGATE_ThreeParams(FOnAttributeDynamicChanged, FGameplayTag, AttributeTag, float, NewAttributeValue, float, OldAttributeValue);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent), HideCategories=(Navigation,Cooking,Tags))
class ACTIONROGUELIKE_API URogueActionSystemComponent : public UActorComponent {
	GENERATED_BODY()
	
public:
	URogueActionSystemComponent();
	virtual void InitializeComponent() override;
	void SetDefaultAttributeSet(TSubclassOf<URogueAttributeSet> AttributeSetClass);
	
	virtual void BeginPlay() override;
	
protected:	
	UPROPERTY(EditAnywhere, Instanced, NoClear, Category="Action System")
	TObjectPtr<URogueAttributeSet> Attributes;   

	// Find Attribute via GameplayTag from CachedAttributes
	// Map of all Attributes with a GameplayTag key-value pair (Attribute Name must == GameplayTag)
	TMap<FGameplayTag, FRogueAttribute*> CachedAttributes;   
	
	TMap<FGameplayTag, FOnAttributeChanged> AttributeListeners;
	TMap<FGameplayTag, TArray<FOnAttributeDynamicChanged>> AttributeDynamicListeners;

	UPROPERTY()
	TArray<TObjectPtr<URogueAction>> Actions;
	
	UPROPERTY(EditAnywhere, Category="Action System")
	TArray<TSubclassOf<URogueAction>> DefaultActions;

public:
	UFUNCTION(BlueprintCallable)
	void GrantAction(TSubclassOf<URogueAction> NewActionClass);
	
	void RemoveAction(URogueAction* ActionToRemove);
	void StartAction(const FGameplayTag InActionName);
	void StopAction(const FGameplayTag InActionName);

	FRogueAttribute* GetAttribute(const FGameplayTag InAttributeTag) const;
	UFUNCTION(BlueprintCallable)
	float GetAttributeValue(const FGameplayTag InAttributeTag) const;
	
	FOnAttributeChanged& GetAttributeListener(FGameplayTag AttributeTag);
	
	UFUNCTION(BlueprintCallable, DisplayName="Add Attribute Listener", meta=(Keywords="events,delegate"))
	void AddDynamicAttributeListener(FOnAttributeDynamicChanged Event, const FGameplayTag AttributeTag);
	
	UFUNCTION(BlueprintCallable, DisplayName="Remove Attribute Listener", meta=(Keywords="events,delegate"))
	void RemoveDynamicAttributeListener(FOnAttributeDynamicChanged Event);

	UFUNCTION(BlueprintCallable)
	void ApplyAttributeChange(FGameplayTag AttributeTag, float Delta, EAttributeModifyType ModifyType);
	
	FGameplayTagContainer ActiveGameplayTags;
};
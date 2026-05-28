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


DECLARE_MULTICAST_DELEGATE_ThreeParams(FOnAttributeChanged, FGameplayTag /*AttributeTag*/, float /*NewAttributeValue*/, float /*OldAttributeValue*/);


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class ACTIONROGUELIKE_API URogueActionSystemComponent : public UActorComponent {
	GENERATED_BODY()
	
public:
	URogueActionSystemComponent();
	virtual void InitializeComponent() override;
	
	virtual void BeginPlay() override;
	
protected:	
	UPROPERTY()
	TObjectPtr<URogueAttributeSet> Attributes;
	
	TMap<FGameplayTag, FRogueAttribute*> CachedAttributes;
	
	UPROPERTY(EditAnywhere, Category=Attributes, NoClear)
	TSubclassOf<URogueAttributeSet> AttributeSetClass;
	
	TMap<FGameplayTag, FOnAttributeChanged> AttributeListeners;

	UPROPERTY()
	TArray<TObjectPtr<URogueAction>> Actions;
	
	UPROPERTY(EditAnywhere, Category="Actions")
	TArray<TSubclassOf<URogueAction>> DefaultActions;

public:
	void GrantAction(TSubclassOf<URogueAction> NewActionClass);
	void StartAction(const FGameplayTag InActionName);
	void StopAction(const FGameplayTag InActionName);

	FRogueAttribute* GetAttribute(const FGameplayTag InAttributeTag) const;
	
	FOnAttributeChanged& GetAttributeListener(FGameplayTag AttributeTag);

	UFUNCTION(BlueprintCallable)
	void ApplyAttributeChange(FGameplayTag AttributeTag, float Delta, EAttributeModifyType ModifyType);
	
	FGameplayTagContainer ActiveGameplayTags;
};
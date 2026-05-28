// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "RogueAttributeSet.generated.h"

class URogueActionSystemComponent;

USTRUCT()
struct FRogueAttribute {
	GENERATED_BODY()

	FRogueAttribute() {}
	FRogueAttribute(const float InBase) : Base(InBase) {}
	
	UPROPERTY(EditAnywhere)
	float Base = 0.0f;
	
	UPROPERTY(Transient)
	float Modifier = 0.0f;
	
	float GetValue() const
	{
		return Base + Modifier;
	}
};

/**
 * 
 */
UCLASS()
class ACTIONROGUELIKE_API URogueAttributeSet : public UObject {
	GENERATED_BODY()
	
public:
	virtual void InitializeAttributes() {}
	virtual  void PostAttributeChanged() {}
	
protected:
	URogueActionSystemComponent* GetOwningComponent() const;
};


UCLASS()
class URogueHealthAttributeSet : public URogueAttributeSet {
	GENERATED_BODY()
	
public:
	URogueHealthAttributeSet();
	virtual void PostAttributeChanged() override;

	UPROPERTY(EditAnywhere, Category=Attributes)
	FRogueAttribute Health;

	UPROPERTY(EditAnywhere, Category=Attributes)
	FRogueAttribute HealthMax;
};


UCLASS()
class URoguePawnAttributeSet : public URogueHealthAttributeSet {
	GENERATED_BODY()
	
public:
	URoguePawnAttributeSet();
	virtual void InitializeAttributes() override;
	virtual void PostAttributeChanged() override;
	
	void ApplyMovementSpeed() const;
	
	// Walking speed directly linked with Character Movement Component
	UPROPERTY(EditAnywhere, Category=Attributes)
	FRogueAttribute MoveSpeed;
};


UCLASS()
class URoguePlayerAttributeSet : public URoguePawnAttributeSet {
	GENERATED_BODY()
	
public:
	URoguePlayerAttributeSet();
	
};


UCLASS()
class URogueMonsterAttributeSet : public URoguePawnAttributeSet {
	GENERATED_BODY()
	
public:
	URogueMonsterAttributeSet();
	
};
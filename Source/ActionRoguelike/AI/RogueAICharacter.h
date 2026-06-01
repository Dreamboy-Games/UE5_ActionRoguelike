// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RogueAICharacter.generated.h"

class URogueActionSystemComponent;

UCLASS()
class ACTIONROGUELIKE_API ARogueAICharacter : public ACharacter {
	GENERATED_BODY()

public:
	ARogueAICharacter();
	
	virtual void PostInitializeComponents() override;
	
	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<URogueActionSystemComponent> ActionSystemComponent;
	
	FTimerHandle OverlayTimerHandle;

private:
};

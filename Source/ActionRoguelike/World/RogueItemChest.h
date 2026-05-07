// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionRoguelike/Core/RogueInteractionInterface.h"
#include "GameFramework/Actor.h"
#include "RogueItemChest.generated.h"

class UStaticMeshComponent;

UCLASS()
class ACTIONROGUELIKE_API ARogueItemChest : public AActor, public IRogueInteractionInterface {
	GENERATED_BODY()

public:
	ARogueItemChest();
	virtual void Tick(float DeltaTime) override;

	virtual void Interact_Implementation() override;

protected:
	UPROPERTY(EditAnywhere, Category="Animation")
	float AnimationTargetPitch = 120.0f;
	
	UPROPERTY(EditAnywhere, Category="Animation")
	float AnimationSpeed = 150.0f;
	
	UFUNCTION(BlueprintImplementableEvent)
	void ChestAnimationComplete();

private:
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> BaseMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Components")
	TObjectPtr<UStaticMeshComponent> LidMeshComponent;

	float CurrentAnimationPitch = 0.0f;
};

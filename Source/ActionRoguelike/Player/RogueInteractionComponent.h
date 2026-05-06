// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RogueInteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent, PrioritizeCategories = "_Interaction"))
class ACTIONROGUELIKE_API URogueInteractionComponent : public UActorComponent {
	GENERATED_BODY()

public:
	URogueInteractionComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	
	void Interact();

protected:
	UPROPERTY(EditDefaultsOnly, Category="_Interaction", meta=(DisplayPriority = "0"))
	float InteractionRadius = 400;
	
	UPROPERTY()
	TObjectPtr<AActor> SelectedActor;

private:
};

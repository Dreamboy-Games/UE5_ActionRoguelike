// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "RogueAIController.generated.h"

class UBehaviorTree;

UCLASS()
class ACTIONROGUELIKE_API ARogueAIController : public AAIController {
	GENERATED_BODY()

public:
	ARogueAIController();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category="AI")
	TObjectPtr<UBehaviorTree> BehaviorTree;

private:
};

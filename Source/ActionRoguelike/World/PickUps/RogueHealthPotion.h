// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoguePickupActor.h"
#include "RogueHealthPotion.generated.h"

UCLASS()
class ACTIONROGUELIKE_API ARogueHealthPotion : public ARoguePickupActor {
	GENERATED_BODY()

public:
	ARogueHealthPotion();

protected:
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UStaticMeshComponent> PickupMeshComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="PickUp")
	TObjectPtr<USoundBase> PickupSound;
	
	UPROPERTY(EditDefaultsOnly, Category="PickUp")
	float HealingAmount = 50.0f;
	
	virtual void OnActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

private:
};

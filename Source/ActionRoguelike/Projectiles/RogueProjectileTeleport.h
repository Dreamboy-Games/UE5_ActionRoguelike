// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RogueProjectile.h"
#include "RogueProjectileTeleport.generated.h"

class UNiagaraSystem;

UCLASS()
class ACTIONROGUELIKE_API ARogueProjectileTeleport : public ARogueProjectile {
	GENERATED_BODY()

public:
	ARogueProjectileTeleport();

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> ExpandEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> ExpandSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<UNiagaraSystem> TeleportEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Effects")
	TObjectPtr<USoundBase> TeleportSound;

	void StartDelayedTeleport();
	void HandleTeleportation();
	
	virtual void OnActorHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;
	
private:
	FTimerHandle TeleportTimerHandle;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActionSystem/RogueAction.h"
#include "RogueAction_ProjectileAttack.generated.h"

class URogueActionSystemComponent;
class ARogueProjectile;
class UNiagaraSystem;
class USoundBase;
class UAnimMontage;

/**
 * 
 */
UCLASS(Abstract)
class ACTIONROGUELIKE_API URogueAction_ProjectileAttack : public URogueAction {
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TSubclassOf<ARogueProjectile> ProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	FName MuzzleSocketName = FName("Muzzle_01");

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TObjectPtr<UNiagaraSystem> CastingEffect;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TObjectPtr<USoundBase> CastingSound;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TObjectPtr<UAnimMontage> AttackMontage;

	void AttackTimerElapsed();

public:
	URogueAction_ProjectileAttack();
	
	virtual void StartAction_Implementation() override;
	
	virtual void StopAction_Implementation() override;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RoguePlayerCharacter.generated.h"

class ARogueProjectile;
class UNiagaraSystem;
class UAnimMontage;
class ARogueProjectileBlackhole;
class ARogueProjectileMagic;
struct FInputActionInstance;
struct FInputActionValue;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class ACTIONROGUELIKE_API ARoguePlayerCharacter : public ACharacter {
	GENERATED_BODY()

public:
	ARoguePlayerCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	void Move(const FInputActionValue& InValue);
	void Look(const FInputActionInstance& InValue);
	virtual void Jump() override;
	
	void StartProjectileAttack(const TSubclassOf<ARogueProjectile> ProjectileClass);

protected:
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TSubclassOf<ARogueProjectile> PrimaryProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TSubclassOf<ARogueProjectile> SecondaryProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TSubclassOf<ARogueProjectile> SpecialProjectileClass;
	
	UPROPERTY(VisibleAnywhere, Category="Attack")
	FName MuzzleSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TObjectPtr<UNiagaraSystem> CastingEffect;
	
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TObjectPtr<USoundBase> CastingSound;
	
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UCameraComponent> CameraComponent;
	
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Move;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Look;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_Jump;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_PrimaryAttack;

	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_SecondaryAttack;
	
	UPROPERTY(EditDefaultsOnly, Category="Input")
	TObjectPtr<UInputAction> Input_SpecialAttack;
	
	void PlayAttackMontage();
	void AttackTimerElapsed(const TSubclassOf<ARogueProjectile> ProjectileClass);
};

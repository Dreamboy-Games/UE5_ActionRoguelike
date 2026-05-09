// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RoguePlayerCharacter.generated.h"

class URogueActionSystemComponent;
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
	virtual void PostInitializeComponents() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void Move(const FInputActionValue& InValue);
	void Look(const FInputActionInstance& InValue);
	virtual void Jump() override;

	void StartProjectileAttack(const TSubclassOf<ARogueProjectile> ProjectileClass);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

protected:
	// COMPONENTS
	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere, Category="Components")
	TObjectPtr<USpringArmComponent> SpringArmComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Components")
	TObjectPtr<URogueActionSystemComponent> ActionSystemComponent;
	
	// INPUTS
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

	// ATTACKS
	UPROPERTY(VisibleAnywhere, Category="Attack")
	FName MuzzleSocketName;
	
	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TSubclassOf<ARogueProjectile> PrimaryProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TSubclassOf<ARogueProjectile> SecondaryProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TSubclassOf<ARogueProjectile> SpecialProjectileClass;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TObjectPtr<UNiagaraSystem> CastingEffect;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TObjectPtr<USoundBase> CastingSound;

	UPROPERTY(EditDefaultsOnly, Category="Attack")
	TObjectPtr<UAnimMontage> AttackMontage;
	
	// MONTAGES
	UPROPERTY(EditDefaultsOnly, Category="Montages")
	TObjectPtr<UAnimMontage> DeathMontage;
	
	// FUNCTIONS
	void PlayAttackMontage();
	void AttackTimerElapsed(const TSubclassOf<ARogueProjectile> ProjectileClass);
	
	UFUNCTION()
	void OnHealthChanged(float NewHealth, float OldHealth);
};

// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePlayerCharacter.h"

#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "RogueGameTypes.h"
#include "Camera/CameraComponent.h"
#include "Components/ActionSystem/RogueActionSystemComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Projectiles/RogueProjectile.h"

TAutoConsoleVariable<float> CVarProjectileAimDebugDrawing(TEXT("game.projectile.DebugDraw"), 0.0f, TEXT("Enable projectile aim adjustment debug rendering. (0 = off, > 0 = duration)"), ECVF_Cheat);

ARoguePlayerCharacter::ARoguePlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	ActionSystemComponent = CreateDefaultSubobject<URogueActionSystemComponent>(TEXT("ActionSystemComponent"));
	
	MuzzleSocketName = FName("Muzzle_01");
}

void ARoguePlayerCharacter::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	ActionSystemComponent->OnHealthChanged.AddDynamic(this, &ThisClass::OnHealthChanged);
}

void ARoguePlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInput->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Move);
	EnhancedInput->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Look);
	EnhancedInput->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::Jump);
	
	EnhancedInput->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::StartProjectileAttack, PrimaryProjectileClass);
	EnhancedInput->BindAction(Input_SecondaryAttack, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::StartProjectileAttack, SecondaryProjectileClass);
	EnhancedInput->BindAction(Input_SpecialAttack, ETriggerEvent::Triggered, this, &ARoguePlayerCharacter::StartProjectileAttack, SpecialProjectileClass);
}


void ARoguePlayerCharacter::Move(const FInputActionValue& InValue) 
{
	const FVector2D InputValue = InValue.Get<FVector2D>();
	
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	AddMovementInput(ControlRot.Vector(), InputValue.X);
	
	//const FVector RightDirection = GetActorRightVector();
	const FVector RightDirection = ControlRot.RotateVector(FVector::RightVector);
	AddMovementInput(RightDirection, InputValue.Y);
}

void ARoguePlayerCharacter::Look(const FInputActionInstance& InValue)
{
	const FVector2D InputValue = InValue.GetValue().Get<FVector2D>();
	AddControllerPitchInput(InputValue.Y);
	AddControllerYawInput(InputValue.X);
}

void ARoguePlayerCharacter::Jump()
{
	Super::Jump();
}


// ATTACK
void ARoguePlayerCharacter::StartProjectileAttack(const TSubclassOf<ARogueProjectile> ProjectileClass)
{
	PlayAttackMontage();
	
	FTimerHandle AttackTimerHandle;
	constexpr float AttackDelayTimer = 0.2f;
	FTimerDelegate AttackDelegate;
	AttackDelegate.BindUObject(this, &ARoguePlayerCharacter::AttackTimerElapsed, ProjectileClass);
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, AttackDelegate, AttackDelayTimer, false);
}

void ARoguePlayerCharacter::PlayAttackMontage()
{
	PlayAnimMontage(AttackMontage);
	UNiagaraFunctionLibrary::SpawnSystemAttached(CastingEffect, GetMesh(), MuzzleSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	UGameplayStatics::PlaySound2D(this, CastingSound);
}

void ARoguePlayerCharacter::AttackTimerElapsed(const TSubclassOf<ARogueProjectile> ProjectileClass)
{
	FVector SpawnLocation = GetMesh()->GetSocketLocation(MuzzleSocketName);
	FVector AdjustedTargetLocation;
	FVector EyeLocation = CameraComponent->GetComponentLocation();
	FRotator EyeRotation = GetControlRotation();
	FVector TraceEnd = EyeLocation + (EyeRotation.Vector() * 5000.0f);
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(this);
	UWorld* World = GetWorld();
	
	FHitResult Hit;
	if (World->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd, COLLISION_PROJECTILE, QueryParams))
	{
		AdjustedTargetLocation = Hit.Location;
	}
	else
	{
		AdjustedTargetLocation = TraceEnd;
	}	
	FRotator SpawnRotation = (AdjustedTargetLocation - SpawnLocation).Rotation();
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AActor* NewProjectile = World->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	
	MoveIgnoreActorAdd(NewProjectile);
	
#if !UE_BUILD_SHIPPING
	if (float DebugDrawDuration = CVarProjectileAimDebugDrawing.GetValueOnGameThread(); DebugDrawDuration > 0.0f)
	{
		// hit location or trace end
		DrawDebugBox(World, AdjustedTargetLocation, FVector(20.0f), FColor::Green, false, DebugDrawDuration);
		// adjusted line trace
		DrawDebugLine(World, EyeLocation, TraceEnd, FColor::Green, false, DebugDrawDuration);
		// new projectile path
		DrawDebugLine(World, SpawnLocation, AdjustedTargetLocation, FColor::Yellow, false, DebugDrawDuration);
		// original projectile path
		DrawDebugLine(World, SpawnLocation, SpawnLocation + (GetControlRotation().Vector() * 5000.0f), FColor::Purple, false, DebugDrawDuration);
	}
#endif
}

float ARoguePlayerCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
                                        class AController* EventInstigator, AActor* DamageCauser)
{
	float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	
	ActionSystemComponent->ApplyHealthChange(-ActualDamage);
	
	return ActualDamage;
}

void ARoguePlayerCharacter::OnHealthChanged(float NewHealth, float OldHealth)
{
	// Died?
	if (FMath::IsNearlyZero(NewHealth))
	{
		DisableInput(nullptr);
		GetMovementComponent()->StopMovementImmediately();
		PlayAnimMontage(DeathMontage);
	}
}

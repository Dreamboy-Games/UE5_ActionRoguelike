// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCharacter.h"

#include "EnhancedInputComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "ActionRoguelike/Projectiles/RogueProjectileMagic.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"


ARogueCharacter::ARogueCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);
	SpringArmComponent->bUsePawnControlRotation = true;
	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComponent->SetupAttachment(SpringArmComponent);
	
	MuzzleSocketName = FName("Muzzle_01");
}

void ARogueCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	EnhancedInput->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ARogueCharacter::Move);
	EnhancedInput->BindAction(Input_Look, ETriggerEvent::Triggered, this, &ARogueCharacter::Look);
	EnhancedInput->BindAction(Input_Jump, ETriggerEvent::Triggered, this, &ARogueCharacter::Jump);
	
	EnhancedInput->BindAction(Input_PrimaryAttack, ETriggerEvent::Triggered, this, &ARogueCharacter::PrimaryAttack);
}


void ARogueCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}


void ARogueCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ARogueCharacter::Move(const FInputActionValue& InValue) 
{
	const FVector2D InputValue = InValue.Get<FVector2D>();
	
	FRotator ControlRot = GetControlRotation();
	ControlRot.Pitch = 0.0f;
	AddMovementInput(ControlRot.Vector(), InputValue.X);
	
	//const FVector RightDirection = GetActorRightVector();
	const FVector RightDirection = ControlRot.RotateVector(FVector::RightVector);
	AddMovementInput(RightDirection, InputValue.Y);
}

void ARogueCharacter::Look(const FInputActionInstance& InValue)
{
	const FVector2D InputValue = InValue.GetValue().Get<FVector2D>();
	AddControllerPitchInput(InputValue.Y);
	AddControllerYawInput(InputValue.X);
}

void ARogueCharacter::Jump()
{
	Super::Jump();
}

void ARogueCharacter::PrimaryAttack()
{
	PlayAnimMontage(AttackMontage);
	
	FTimerHandle AttackTimerHandle;
	const float AttackDelayTimer = 0.2f;
	
	// Spawn Attack Effect & Play Attack Sound
	UNiagaraFunctionLibrary::SpawnSystemAttached(CastingEffect, GetMesh(), MuzzleSocketName,
		FVector::ZeroVector, FRotator::ZeroRotator, EAttachLocation::Type::SnapToTarget, true);
	UGameplayStatics::PlaySound2D(this, CastingSound);
	
	// Spawn Projectile after timer delay
	GetWorld()->GetTimerManager().SetTimer(AttackTimerHandle, this, &ARogueCharacter::AttackTimerElapsed, AttackDelayTimer);
}

void ARogueCharacter::AttackTimerElapsed()
{
	const FVector SpawnLocation = GetMesh()->GetSocketLocation(MuzzleSocketName);
	const FRotator SpawnRotation = GetControlRotation();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Instigator = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	AActor* NewProjectile = GetWorld()->SpawnActor<AActor>(ProjectileClass, SpawnLocation, SpawnRotation, SpawnParams);
	
	MoveIgnoreActorAdd(NewProjectile);
}

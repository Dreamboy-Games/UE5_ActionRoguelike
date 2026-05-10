// Fill out your copyright notice in the Description page of Project Settings.


#include "RoguePickupActor.h"

#include "Components/SphereComponent.h"


ARoguePickupActor::ARoguePickupActor()
{
	PrimaryActorTick.bCanEverTick = false;
	OverlapComponent = CreateDefaultSubobject<USphereComponent>(TEXT("OverlapComponent"));
	RootComponent = OverlapComponent;
	OverlapComponent->SetSphereRadius(OverlapRadius);
	OverlapComponent->SetCollisionProfileName("Pickups");
}

void ARoguePickupActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	OverlapComponent->OnComponentBeginOverlap.AddDynamic(this, &ARoguePickupActor::OnActorOverlapped);
}

void ARoguePickupActor::OnActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueHealthPotion.h"

#include "RogueGameplayTags.h"
#include "ActionSystem/RogueActionSystemComponent.h"
#include "Core/RogueGameplayStatics.h"
#include "Kismet/GameplayStatics.h"


ARogueHealthPotion::ARogueHealthPotion()
{
	PrimaryActorTick.bCanEverTick = false;
	
	PickupMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMeshComp"));
	PickupMeshComponent->SetCollisionProfileName("NoCollision");
	PickupMeshComponent->SetupAttachment(RootComponent);
}

void ARogueHealthPotion::OnActorOverlapped(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	URogueActionSystemComponent* ActionComp = OtherActor->GetComponentByClass<URogueActionSystemComponent>();
	
	// Asset if null, then we misconfigured what we can overlap with, any Pawn should have an action component
	// Skip pickup if already full health
	if (ensure(ActionComp != nullptr) && !URogueGameplayStatics::IsFullHealth(ActionComp))
	{
		// Will clamp to the HealthMax
		ActionComp->ApplyAttributeChange(SharedGameplayTags::Attribute_Health, HealingAmount, Base);
		
		// Play before destroying actor, to have valid context and location
		UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation(), FRotator::ZeroRotator);
		
		// Remove Actor from world, eventually memory will be freed
		Destroy();
	}
}

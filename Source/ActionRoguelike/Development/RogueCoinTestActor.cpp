// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCoinTestActor.h"

#include "NavigationSystem.h"
#include "Pickups/RogueCoinPickupSubsystem.h"


ARogueCoinTestActor::ARogueCoinTestActor()
{
	PrimaryActorTick.bCanEverTick = false;
	DefaultSceneComp = CreateDefaultSubobject<USceneComponent>(TEXT("DefaultSceneComponent"));
	RootComponent = DefaultSceneComp;
}


void ARogueCoinTestActor::SpawnCoins(const int32 SpawnCount)
{
	TArray<FVector> CoinLocations;
	TArray<int32> CoinAmounts;

	const UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(this);
	const FVector ActorLocation = GetActorLocation();
	
	for (int i = 0; i < SpawnCount; i++)
	{
		FNavLocation NavLocation;
		NavSystem->GetRandomPointInNavigableRadius(ActorLocation, 1024, NavLocation);
		CoinLocations.Add(NavLocation.Location);
		CoinAmounts.Add(10);
	}
	
	URogueCoinPickupSubsystem* CoinSystem = GetWorld()->GetSubsystem<URogueCoinPickupSubsystem>();
	
	CoinSystem->AddCoinPickups(CoinLocations, CoinAmounts);
}

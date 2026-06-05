// Fill out your copyright notice in the Description page of Project Settings.


#include "RogueCoinPickupSubsystem.h"

#include "ActionRoguelike.h"
#include "EngineUtils.h"
#include "Components/AudioComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Core/RogueDeveloperSettings.h"
#include "Player/RoguePlayerCharacter.h"
#include "ProfilingDebugging/CountersTrace.h"


TRACE_DECLARE_INT_COUNTER(CoinInstanceCount, TEXT("Coins in World"));

void URogueCoinPickupSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	
	UWorld* World = GetWorld();
	
	// Temporary Hack
	// const FSoftObjectPath MeshAssetPath(TEXT("/Game/ExampleContent/Meshes/SM_Pickup_Coin.SM_Pickup_Coin"));
	// UStaticMesh* LoadedMesh = Cast<UStaticMesh>(MeshAssetPath.TryLoad());

	WorldISM = NewObject<UInstancedStaticMeshComponent>(World, NAME_None, RF_Transient);
	WorldISM->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	WorldISM->SetAffectDistanceFieldLighting(false);
	WorldISM->RegisterComponentWithWorld(World);

	WorldAudioComp = NewObject<UAudioComponent>(World, NAME_None, RF_Transient);
	WorldAudioComp->SetAutoActivate(false);
	WorldAudioComp->bAllowSpatialization = false;
	WorldAudioComp->RegisterComponentWithWorld(World);
	
	//FLoadSoftObjectPathAsyncDelegate Delegate;
	//Delegate.BindUObject(this, &ThisClass::OnPickupMeshLoadComplete);

	const URogueDeveloperSettings* DevSettings = GetDefault<URogueDeveloperSettings>();
	DevSettings->CoinPickupMesh.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateUObject(this, &ThisClass::OnPickupMeshLoadComplete));
	DevSettings->CoinPickupSound.LoadAsync(FLoadSoftObjectPathAsyncDelegate::CreateUObject(this, &ThisClass::OnPickupSoundLoadComplete));
	CoinPickupAudioTriggerParamName = DevSettings->CoinPickupTriggerParameter;
	
	TRACE_COUNTER_SET(CoinInstanceCount, 0);
}

inline void URogueCoinPickupSubsystem::OnPickupMeshLoadComplete(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject) const
{
	WorldISM->SetStaticMesh(Cast<UStaticMesh>(LoadedObject));
}

void URogueCoinPickupSubsystem::OnPickupSoundLoadComplete(const FSoftObjectPath& SoftObjectPath, UObject* LoadedObject) const
{
	WorldAudioComp->SetSound(Cast<USoundBase>(LoadedObject));
}


void URogueCoinPickupSubsystem::AddCoinPickups(TArray<FVector> NewLocations, TArray<int32> NewAmounts)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(CoinPickupSubsystem::AddCoinPickups);
	
	CoinLocations.Append(NewLocations);
	CoinAmounts.Append(NewAmounts);
	
	TArray<FTransform> MeshTransforms;
	for (int i = 0; i < NewLocations.Num(); i++)
	{
		MeshTransforms.Add(FTransform(NewLocations[i] + FVector(0, 0, 50.0f)));
	}

	const TArray<FPrimitiveInstanceId> NewMeshIDs = WorldISM->AddInstancesById(MeshTransforms, true, false);
	MeshIDs.Append(NewMeshIDs);
	
	TRACE_COUNTER_SET(CoinInstanceCount, CoinLocations.Num());
}

void URogueCoinPickupSubsystem::RemoveCoinPickup(const int32 IndexToRemove)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(CoinPickupSubsystem::RemoveCoinPickup);
	
	CoinLocations.RemoveAt(IndexToRemove);
	CoinAmounts.RemoveAt(IndexToRemove);
	
	WorldISM->RemoveInstanceById(MeshIDs[IndexToRemove]);
	MeshIDs.RemoveAt(IndexToRemove);
	
	TRACE_COUNTER_SET(CoinInstanceCount, CoinLocations.Num());
}

void URogueCoinPickupSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TRACE_CPUPROFILER_EVENT_SCOPE(CoinPickupSubsystem::Tick);

	const UWorld* World = GetWorld();
	
	FVector PlayerLocation = FVector::ZeroVector;
	for (const ARoguePlayerCharacter* PlayerCharacter : TActorRange<ARoguePlayerCharacter>(World))
	{
		PlayerLocation = PlayerCharacter->GetActorLocation();
	}
	
	const float PickupRadius = 200.0f;
	
	TArray<int32> ProcessList;
	
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(CoinPickupSubsystem::Tick::DistanceCheck);
		
		for (int i = 0; i < CoinLocations.Num(); i++)
		{
			const float Dist = FVector::Dist(PlayerLocation, CoinLocations[i]);
			if (Dist < PickupRadius)
			{
				ProcessList.Add(i);
			}
		}
	}
	
	int32 TotalCoinsToGrant = 0;
	{
		TRACE_CPUPROFILER_EVENT_SCOPE(CoinPickupSubsystem::Tick::HandlePickups);
		
		for (int i = ProcessList.Num()-1; i >= 0; --i)
		{
			const int32 CoinIndex = ProcessList[i];
			TotalCoinsToGrant += CoinAmounts[CoinIndex];
			RemoveCoinPickup(CoinIndex);
		}
		
		if (TotalCoinsToGrant > 0)
		{
			//WorldAudioComp->SetWorldLocation(PlayerLocation);
			PlayPickupSound();
		}
	}
	
#if 0
	// @todo: grant coins to player(s)
	UE_CLOG(TotalCoinsToGrant > 0, LogGame, Log, TEXT("Pickup Coin Amount = %d"), TotalCoinsToGrant);
	
	for (int i = 0; i < CoinLocations.Num(); i++)
	{
		DrawDebugPoint(World, CoinLocations[i], 8.0f, FColor::White);
	}
#endif
}

void URogueCoinPickupSubsystem::PlayPickupSound()
{
	if (!WorldAudioComp->IsPlaying())
	{
		WorldAudioComp->Play();
	}
	WorldAudioComp->SetTriggerParameter(CoinPickupAudioTriggerParamName);
}

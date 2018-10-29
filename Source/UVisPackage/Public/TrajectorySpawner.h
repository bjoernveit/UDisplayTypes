#pragma once
#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "TrajectoryMarker.h"
#include "TrajectorySpawner.generated.h"


USTRUCT()
struct UVISPACKAGE_API FTrajectorySpawner
{
	GENERATED_USTRUCT_BODY()

public:

	static ATrajectoryMarker* SpawnTrajectoryFromPoints(UWorld* World, TArray<FVector>& Points, FColor Color);

	static ATrajectoryMarker* SpawnTrajectoryFromPoints(UWorld* World, TArray<FVector>& Points, FColor ColorStart, FColor ColorEnd);
	static bool AddTrajectoryToMarker(UWorld* World, ATrajectoryMarker& Marker, TArray<FVector>& Points, FColor Color);
	static bool AddTrajectoryToMarker(UWorld* World, ATrajectoryMarker& Marker, TArray<FVector>& Points, FColor ColorBegin, FColor ColorEnd);

private:
	static bool AddTrajectoryToMarkerInternal(UWorld* World, ATrajectoryMarker& Actor, TArray<FVector>& Points, FColor ColorBegin, FColor ColorEnd);
};

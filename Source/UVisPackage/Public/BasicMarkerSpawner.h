#pragma once
#include "CoreMinimal.h"
#include "Materials/MaterialInterface.h"
#include "Engine/World.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "VisualMarker.h"
#include "BasicMarkerSpawner.generated.h"


UENUM()
enum class EBasicMarkerType : uint8
{
	Point,
	Cube,
	Sphere,
	Cylinder,
	Cone,
	Arrow
};


USTRUCT()
struct UVISPACKAGE_API FBasicMarkerSpawner
{
	GENERATED_USTRUCT_BODY()

public:
	static AVisualMarker* SpawnVisualMarker(UWorld* World, EBasicMarkerType Type, FVector Location, FRotator Rotation,
	                                 FColor Color = FColor::White);

	static bool AddVisualToActor(UWorld* World, AActor& Actor, EBasicMarkerType Type, FVector Location, FRotator Rotation, FColor Color);

private:
	static bool AddPointVisualToActor(UWorld* World, AActor& Actor, FVector Location, FRotator Rotation, FColor Color);
	static bool AddVisualToActorInternal(UWorld* World, AActor & Actor, EBasicMarkerType Type, FVector Location, FRotator Rotation, FColor Color);
	static FString GetMeshPathOfType(EBasicMarkerType Type);
};



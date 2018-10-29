#include "TrajectorySpawner.h"
#include "ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GPUPointCloudRendererComponent.h"
#include "Engine/World.h"
#include "StaticHelpers.h"



ATrajectoryMarker* FTrajectorySpawner::SpawnTrajectoryFromPoints(UWorld* World, TArray<FVector>& Points, FColor Color)
{
	return SpawnTrajectoryFromPoints(World, Points, Color, Color);
}

ATrajectoryMarker* FTrajectorySpawner::SpawnTrajectoryFromPoints(UWorld* World, TArray<FVector>& Points, FColor ColorBegin, FColor ColorEnd)
{

	ATrajectoryMarker* TrajectoryMarker;
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.OverrideLevel = World->GetCurrentLevel();

	if (IsInGameThread())
	{
		TrajectoryMarker = World->SpawnActor<ATrajectoryMarker>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
	}
	else
	{

		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			// Setup BaseActor and PCRComponent
			TrajectoryMarker = World->SpawnActor<ATrajectoryMarker>(FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		}, TStatId(), nullptr, ENamedThreads::GameThread);

		//wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
	}

	return AddTrajectoryToMarker(World, *TrajectoryMarker, Points, ColorBegin, ColorEnd) ? TrajectoryMarker : nullptr;
}

bool FTrajectorySpawner::AddTrajectoryToMarker(UWorld* World, ATrajectoryMarker& Marker, TArray<FVector>& Points, FColor Color)
{
	return AddTrajectoryToMarker(World, Marker, Points, Color, Color);
}


bool FTrajectorySpawner::AddTrajectoryToMarker(UWorld* World, ATrajectoryMarker& Marker, TArray<FVector>& Points, FColor ColorBegin, FColor ColorEnd)
{

	if (IsInGameThread())
	{
		return AddTrajectoryToMarkerInternal(World, Marker, Points, ColorBegin, ColorEnd);
	}
	else
	{
		bool bSuccess;
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			bSuccess = AddTrajectoryToMarkerInternal(World, Marker, Points, ColorBegin, ColorEnd);
		}, TStatId(), nullptr, ENamedThreads::GameThread);

		//wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
		return bSuccess;
	}
}


bool FTrajectorySpawner::AddTrajectoryToMarkerInternal(UWorld* World, ATrajectoryMarker& Marker, TArray<FVector>& Points, FColor ColorBegin, FColor ColorEnd)
{

	UGPUPointCloudRendererComponent* PointCloudRendererComponent = NewObject<UGPUPointCloudRendererComponent>(&Marker);
	
	// Prepare point data
	// This is a potential bottleneck on the gamethread, but switching back and forth,
	// caused some unexpected nullpointer problems.
	int i = 0;
	for (auto Point : Points)
	{
		i++;
		Marker.PointsAsColor.Add(FLinearColor(Point.Z, Point.X, Point.Y, Point.Z));
		double Fraction = i / static_cast<double>(Points.Num());
		Marker.ColorOfPoint.Add(StaticHelpers::GetInBetweenColor(ColorBegin, ColorEnd, Fraction));
	}
	
	// Pass data to PCR
	PointCloudRendererComponent->SetDynamicProperties(1, 1, 10);

	PointCloudRendererComponent->SetInputAndConvert1(Marker.PointsAsColor, Marker.ColorOfPoint);

	//Attach and register
	PointCloudRendererComponent->bTickInEditor = true;
	Marker.PCRComponent = PointCloudRendererComponent;
	PointCloudRendererComponent->SetupAttachment(Marker.GetRootComponent());
	PointCloudRendererComponent->RegisterComponent();
	return true;
}


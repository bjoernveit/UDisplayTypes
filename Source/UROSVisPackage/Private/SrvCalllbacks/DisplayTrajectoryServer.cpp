#include "DisplayTrajectoryServer.h"
#include "Ids.h"
#include "TrajectorySpawner.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSDisplayTrajectoryServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSDisplayTrajectorySrv::Request> Request =
		MakeShareable(new FROSDisplayTrajectorySrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSDisplayTrajectoryServer::Callback(
	TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{

	TSharedPtr<FROSDisplayTrajectorySrv::Request> DisplayTrajectoryRequest =
		StaticCastSharedPtr<FROSDisplayTrajectorySrv::Request>(Request);

	
	const FString Id = DisplayTrajectoryRequest->GetMarkerId().IsEmpty() ? FIds::NewGuidInBase64() : DisplayTrajectoryRequest->GetMarkerId();
	const FColor ColorBegin = DisplayTrajectoryRequest->GetColorBegin().GetColor();
	const FColor ColorEnd = DisplayTrajectoryRequest->GetColorEnd().GetColor();
	TArray<FVector> Points;
	for(auto GeometryPoint : DisplayTrajectoryRequest->GetPoints())
	{
		Points.Add(GeometryPoint.GetVector());
	}

	//AVisualMarker** VisualMarker = Controller->IdToMarkerMap.Find(Id);
	AActor** VisualMarker = Controller->IdToActorMap.Find(Id);

	if (VisualMarker == nullptr)
	{
		// Marker does not exist => create Marker
		AVisualMarker* NewMarker = FTrajectorySpawner::SpawnTrajectoryFromPoints(World, Points, ColorBegin, ColorEnd);
		//Controller->IdToMarkerMap.Add(Id, NewMarker);
		Controller->IdToActorMap.Add(Id, NewMarker);
	}
	else
	{
		//Marker does exist => Add Visualisation to the Actor
		const auto Marker = static_cast<ATrajectoryMarker*>(*VisualMarker);
		if (Marker)
			FTrajectorySpawner::AddTrajectoryToMarker(World, *Marker, Points, ColorBegin, ColorEnd);
		else
			UE_LOG(LogTemp, Error, TEXT("[%s] Trajectories can only be added to TrajectoryMarkers."), *FString(__FUNCTION__));
	}

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSDisplayTrajectorySrv::Response(Id));
}

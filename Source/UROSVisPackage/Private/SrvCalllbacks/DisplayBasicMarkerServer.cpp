#include "DisplayBasicMarkerServer.h"
#include "BasicMarkerSpawner.h"
#include "Ids.h"

TSharedPtr<FROSBridgeSrv::SrvRequest> FROSDisplayBasicMarkerServer::FromJson(TSharedPtr<FJsonObject> JsonObject) const
{
	TSharedPtr<FROSDisplayBasicMarkerSrv::Request> Request =
		MakeShareable(new FROSDisplayBasicMarkerSrv::Request());
	Request->FromJson(JsonObject);
	return TSharedPtr<FROSBridgeSrv::SrvRequest>(Request);
}

TSharedPtr<FROSBridgeSrv::SrvResponse> FROSDisplayBasicMarkerServer::Callback(
	TSharedPtr<FROSBridgeSrv::SrvRequest> Request)
{
	TSharedPtr<FROSDisplayBasicMarkerSrv::Request> DisplayMarkerRequest =
		StaticCastSharedPtr<FROSDisplayBasicMarkerSrv::Request>(Request);
	

	const FString Id = DisplayMarkerRequest->GetMarkerId().IsEmpty() ? FIds::NewGuidInBase64() : DisplayMarkerRequest->GetMarkerId();

	//AVisualMarker** VisualMarker = Controller->IdToMarkerMap.Find(Id);
	AActor** VisualMarker = Controller->IdToActorMap.Find(Id);

	const auto MarkerType = EBasicMarkerType(DisplayMarkerRequest->GetMarkerType());
	const FVector Location = DisplayMarkerRequest->GetPose().GetPosition().GetVector();
	const FRotator Rotation = DisplayMarkerRequest->GetPose().GetOrientation().GetQuat().Rotator();
	const FColor Color = DisplayMarkerRequest->GetColor().GetColor();
	double Scale = DisplayMarkerRequest->GetScale();

	if (VisualMarker == nullptr)
	{
		// Marker does not exist => create Marker
		AVisualMarker* NewMarker = FBasicMarkerSpawner::SpawnVisualMarker(World, MarkerType, Location, Rotation, Color, Scale);


		//Controller->IdToMarkerMap.Add(Id, NewMarker);
		Controller->IdToActorMap.Add(Id, NewMarker);
	}
	else
	{
		FBasicMarkerSpawner::AddVisualToActor(World, **VisualMarker, MarkerType, Location, Rotation, Color, Scale);
	}

	return MakeShareable<FROSBridgeSrv::SrvResponse>
		(new FROSDisplayBasicMarkerSrv::Response(Id));
}

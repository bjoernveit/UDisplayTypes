#pragma once
#include "CoreMinimal.h"
#include "ROSBridgeHandler.h"
#include "ROSBridgeSrvServer.h"
#include "DisplayBasicMarker.h"
#include "UVisManager.h"
#include "BasicMarkerSpawner.h"


class FROSDisplayBasicMarkerServer final : public FROSBridgeSrvServer
{
private:
	FROSDisplayBasicMarkerServer(){};
	
	FThreadSafeBool ServiceSuccess;

	UVisManager* Controller;
	FBasicMarkerSpawner* MarkerSpawner;
	UWorld* World;

public:

	FROSDisplayBasicMarkerServer(FString Namespace, FString Name, UWorld* InWorld,
		UVisManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("unreal_vis_msgs/DisplayBasicMarker"))
	{
		Controller = InController;
		World = InWorld;
	};

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};


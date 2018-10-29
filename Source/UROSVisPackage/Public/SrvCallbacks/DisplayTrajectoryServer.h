#pragma once
#include "CoreMinimal.h"
#include "DisplayTrajectory.h"
#include "UVisManager.h"
#include "ROSBridgeSrvServer.h"
#include "TrajectorySpawner.h"

class FROSDisplayTrajectoryServer final : public FROSBridgeSrvServer
{
private:
	FROSDisplayTrajectoryServer(){};

	FThreadSafeBool ServiceSuccess;

	
	UVisManager* Controller;
	UWorld* World;

public:
	FROSDisplayTrajectoryServer(FString Namespace, FString Name, UWorld* InWorld,
		UVisManager* InController) :
		FROSBridgeSrvServer(Namespace + TEXT("/") + Name, TEXT("unreal_vis_msgs/DisplayTrajectory"))
	{
		Controller = InController;
		World = InWorld;
	};

	TSharedPtr<FROSBridgeSrv::SrvRequest> FromJson(TSharedPtr<FJsonObject> JsonObject) const override;

	TSharedPtr<FROSBridgeSrv::SrvResponse> Callback(TSharedPtr<FROSBridgeSrv::SrvRequest> Request) override;
};



#include "UVisManager.h"
#include "DisplayBasicMarkerServer.h"
#include "RemoveMarkerServer.h"
#include "DisplayTrajectoryServer.h"


void UVisManager::Register(FString DefaultNamespace)
{
	Namespace = DefaultNamespace;

	if (!GetWorld())
	{
		UE_LOG(LogTemp, Warning, TEXT("[%s]: GetWorld returned, NULL."), *FString(__FUNCTION__));
		return;
	}
	
	// Setup IDMap
	IdToActorMap = FTags::GetKeyValuesToActor(GetWorld(), "SemLog", "Id");

	SetupServiceServers();
}


void UVisManager::SetupServiceServers()
{
	// Add Service Servers
	ServicesToPublish.Add(MakeShareable<FROSDisplayBasicMarkerServer>(new FROSDisplayBasicMarkerServer(Namespace, TEXT("display_basic_marker"), GetWorld(), this)));
	ServicesToPublish.Add(MakeShareable<FROSRemoveMarkerServer>(new FROSRemoveMarkerServer(Namespace, TEXT("remove_marker"), this)));
	ServicesToPublish.Add(MakeShareable<FROSDisplayTrajectoryServer>(new FROSDisplayTrajectoryServer(Namespace, TEXT("display_trajectory"), GetWorld(), this)));
	
}
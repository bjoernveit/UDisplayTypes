#include "BasicMarkerSpawner.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMesh.h"
#include "VisualMarker.h"
#include "GPUPointCloudRendererComponent.h"


FString FBasicMarkerSpawner::GetMeshPathOfType(EBasicMarkerType Type)
{
	switch(Type)
	{
		case EBasicMarkerType::Cube :
			return "StaticMesh'/UVisPackage/Meshes/BasicMarkerMeshes/SM_Cube.SM_Cube'";
		case EBasicMarkerType::Sphere :
			return "StaticMesh'/UVisPackage/Meshes/BasicMarkerMeshes/SM_Sphere.SM_Sphere'";
		case EBasicMarkerType::Cylinder :
			return "StaticMesh'/UVisPackage/Meshes/BasicMarkerMeshes/SM_Cylinder.SM_Cylinder'";
		case EBasicMarkerType::Cone :
			return "StaticMesh'/UVisPackage/Meshes/BasicMarkerMeshes/SM_Cone.SM_Cone'";
		case EBasicMarkerType::Arrow:
			return "StaticMesh'/UVisPackage/Meshes/BasicMarkerMeshes/SM_Arrow.SM_Arrow'";
		default:
			return "";

	}
}

AVisualMarker* FBasicMarkerSpawner::SpawnVisualMarker(UWorld* World, EBasicMarkerType Type, FVector Location, FRotator Rotation,
	FColor Color)
{
	
	AVisualMarker* BaseActor;
	
	FActorSpawnParameters SpawnParams;
	SpawnParams.bNoFail = true;
	SpawnParams.OverrideLevel = World->GetCurrentLevel();
	
	if (IsInGameThread())
	{
		// Spawn Marker Base Actor
		BaseActor = World->SpawnActor<AVisualMarker>(SpawnParams);
		//BaseActor = World->SpawnActor<AVisualMarker>();
		BaseActor->AddActorWorldTransform(FTransform(Rotation, Location));
	}
	else
	{
		// Run on Gamethread
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			// Spawn Marker Base Actor
			BaseActor = World->SpawnActor<AVisualMarker>(SpawnParams);
			BaseActor->AddActorWorldTransform(FTransform(Rotation, Location));

		}, TStatId(), nullptr, ENamedThreads::GameThread);

		// wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
	}

	if (AddVisualToActor(World, *BaseActor, Type, Location, Rotation, Color))
		return BaseActor;

	BaseActor->Destroy();
	return nullptr;

}

bool FBasicMarkerSpawner::AddVisualToActor(UWorld* World, AActor& Actor, EBasicMarkerType Type, FVector Location, FRotator Rotation, FColor Color)
{
	//Make sure actual Code is run on Gamethread
	if (IsInGameThread())
	{
		return AddVisualToActorInternal(World, Actor, Type, Location, Rotation, Color);
	}
	else
	{
		bool bSuccess;
		//Run on Gamethread
		FGraphEventRef Task = FFunctionGraphTask::CreateAndDispatchWhenReady([&]()
		{
			bSuccess = AddVisualToActorInternal(World, Actor, Type, Location, Rotation, Color);
		}, TStatId(), nullptr, ENamedThreads::GameThread);

		//wait code above to complete
		FTaskGraphInterface::Get().WaitUntilTaskCompletes(Task);
		return bSuccess;
	}

}


bool FBasicMarkerSpawner::AddVisualToActorInternal(UWorld* World, AActor& Actor, EBasicMarkerType Type, FVector Location, FRotator Rotation, FColor Color)
{

	if (Type == EBasicMarkerType::Point)
	{
		return AddPointVisualToActor(World, Actor, Location, Rotation, Color);
	}

	UStaticMeshComponent* MeshComponent = NewObject<UStaticMeshComponent>(&Actor);

	// Asign Mesh to Component
	UStaticMesh* Mesh = Cast<UStaticMesh>(StaticLoadObject(UStaticMesh::StaticClass(), nullptr, *GetMeshPathOfType(Type)));
	if (!Mesh)
	{
		UE_LOG(LogTemp, Error, TEXT("[%s] Basic Mesh could not be loaded from Path: %s"), *FString(__FUNCTION__), *GetMeshPathOfType(Type));
		return false;
	}
	MeshComponent->SetStaticMesh(Mesh);
	auto Material = Cast<UMaterialInterface>(StaticLoadObject(UMaterialInterface::StaticClass(), nullptr, TEXT("Material'/UVisPackage/Meshes/BasicMarkerMeshes/BasicShapeMaterial.BasicShapeMaterial'")));
	UMaterialInstanceDynamic* DynMaterial = UMaterialInstanceDynamic::Create(Material, &Actor);
	DynMaterial->SetVectorParameterValue("Color", Color);
	MeshComponent->SetMaterial(0, DynMaterial);
	MeshComponent->SetupAttachment(Actor.GetRootComponent());
	MeshComponent->SetWorldLocationAndRotation(Location, Rotation);
	MeshComponent->RegisterComponent();
	return true;
}


bool FBasicMarkerSpawner::AddPointVisualToActor(UWorld* World, AActor& Actor, FVector Location, FRotator Rotation, FColor Color)
{
	UGPUPointCloudRendererComponent* PointCloudRendererComponent = NewObject<UGPUPointCloudRendererComponent>(&Actor);

	// Prepare point data
	TArray<FLinearColor> Points;
	TArray<FColor> Colors;
	Points.Add(FLinearColor(Location.Z, Location.X, Location.Y, Location.Z));
	Colors.Add(Color);

	// Pass data to PCR
	PointCloudRendererComponent->SetDynamicProperties(1, 1, 10);

	PointCloudRendererComponent->SetInputAndConvert1(Points, Colors);

	//Attach and register
	PointCloudRendererComponent->bTickInEditor = true;
	PointCloudRendererComponent->SetupAttachment(Actor.GetRootComponent());
	PointCloudRendererComponent->RegisterComponent();
	return true;
}

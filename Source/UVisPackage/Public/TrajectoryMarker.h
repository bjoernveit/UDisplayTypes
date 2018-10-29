// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "VisualMarker.h"
#include "TrajectoryMarker.generated.h"


UCLASS()
class UVISPACKAGE_API ATrajectoryMarker : public AVisualMarker
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
		UGPUPointCloudRendererComponent* PCRComponent;

	UPROPERTY()
		TArray<FLinearColor> PointsAsColor;

	UPROPERTY()
		TArray<FColor> ColorOfPoint;
	
	
};

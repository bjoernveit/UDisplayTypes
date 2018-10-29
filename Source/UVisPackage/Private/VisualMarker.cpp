// Fill out your copyright notice in the Description page of Project Settings.

#include "VisualMarker.h"


// Sets default values
AVisualMarker::AVisualMarker()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
}

// Called when the game starts or when spawned
void AVisualMarker::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AVisualMarker::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


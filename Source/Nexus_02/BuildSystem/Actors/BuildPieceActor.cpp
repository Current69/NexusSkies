// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildPieceActor.h"

// Sets default values
ABuildPieceActor::ABuildPieceActor()
{
 	// Buildings are passive objects unless damaged or interacted with.
	PrimaryActorTick.bCanEverTick = false;

	// Create the Static Mesh Component.
	MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));

	// Set the mesh component as the root of the actor.
	RootComponent = MeshComponent;

	// Set a default collision profile.
	MeshComponent->SetCollisionProfileName(TEXT("BlockAll"));

	// Allow the mesh to generate overlap events if needed later.
	// (Useful for future snapping / validation systems.)
	MeshComponent->SetGenerateOverlapEvents(true);

}

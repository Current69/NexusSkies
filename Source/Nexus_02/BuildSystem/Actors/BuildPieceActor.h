// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "BuildPieceActor.generated.h"

UCLASS(Blueprintable)
class NEXUS_02_API ABuildPieceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildPieceActor();

protected:
	// The mesh that visually represents this building piece.
	// This will become the root component of the actor.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Build Piece")
	UStaticMeshComponent* MeshComponent;

};

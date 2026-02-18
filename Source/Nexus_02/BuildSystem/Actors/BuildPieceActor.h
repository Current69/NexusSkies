// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Data/BuildPieceDefinition.h"
#include "Data/StructureDomain.h"
#include "BuildPieceActor.generated.h"

UCLASS(Blueprintable)
class NEXUS_02_API ABuildPieceActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildPieceActor();

	// Initializes this build piece from a definition asset
	void InitializeFromDefinition(UBuildPieceDefinition* InDefinition);

	// Data asset that defines this build piece's properties
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category="Build|Definition")
	UBuildPieceDefinition* Definition;

	// Cached structural domain copied from the definition at initialization
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Build|Structure")
	EStructureDomain StructureDomain = EStructureDomain::None;

protected:
	// The mesh that visually represents this building piece.
	// This will become the root component of the actor.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Build Piece")
	UStaticMeshComponent* MeshComponent;

};

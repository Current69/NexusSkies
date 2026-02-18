// Ensures this header is only included once during compilation
#pragma once

// Includes Unreal's core types and base functionality
#include "CoreMinimal.h"

// Includes Unreal's DataAsset base class
#include "Engine/DataAsset.h"

// Allows use of the EStructureDomain enum
#include "StructureDomain.h"

// Required for Unreal Header Tool processing
#include "BuildPieceDefinition.generated.h"

// Declares a data asset class that defines properties of a build piece
UCLASS(BlueprintType)
class NEXUS_02_API UBuildPieceDefinition : public UDataAsset
{
	GENERATED_BODY()

public:

	// Defines what structural domain this build piece belongs to
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Build|Structure")
	EStructureDomain StructureDomain = EStructureDomain::None;
};
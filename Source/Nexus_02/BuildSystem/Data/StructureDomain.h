// Ensures this header is only included once during compilation
#pragma once

// Includes Unreal's core types and basic engine functionality
#include "CoreMinimal.h"

// Required for Unreal Header Tool (UHT) to process this file
#include "StructureDomain.generated.h"

// Declares an enum that can be used in both C++ and Blueprints
UENUM(BlueprintType)
enum class EStructureDomain : uint8
{
    // Default state — piece does not belong to any structural domain
    None        UMETA(DisplayName = "None"),

    // Domain for base foundation pieces (grounded structural base)
    Foundation  UMETA(DisplayName = "Foundation"),

    // Domain for pieces that form wall chains or vertical connections
    WallChain   UMETA(DisplayName = "Wall Chain")
};
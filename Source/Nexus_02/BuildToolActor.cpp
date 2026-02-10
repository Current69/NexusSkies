// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildToolActor.h"

// For line tracing + collision queries
#include "Engine/World.h"
#include "DrawDebugHelpers.h"

// To get the player camera viewpoint
#include "Kismet/GameplayStatics.h"

// Sets default values
ABuildToolActor::ABuildToolActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ABuildToolActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildToolActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Prints a debug message on screen
void ABuildToolActor::TestPing()
{
	// GEngine gives access to global engine utilities
	if (GEngine)
	{
		// Print a green message on screen
		// -1 means create a new message slot
		// 2.0f = message duration in seconds
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Green,
			FString::Printf(
				TEXT("BuildToolActor ping! TraceDistance = %.1f"),
				TraceDistance
			)
		);
	}
}

// Performs a forward line trace from the player camera
// Updates bHasBuildHit and lastBuildHit for Blueprint to use
void ABuildToolActor::UpdateBuildTrace()
{
	// Reset hit state each time we trace
	bHasBuildHit = false;
	LastBuildHit = FHitResult();

	// Get the player controller so we can read the camera position + rotation
	APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0);
	if (!PC)
	{
		return; // No player controller found
	}

	// Get camera location + rotation
	FVector CamLoc;
	FRotator CamRot;
	PC->GetPlayerViewPoint(CamLoc, CamRot);

	// Build start/end points for the trace
	const FVector Start = CamLoc;
	const FVector End = Start + (CamRot.Vector() * TraceDistance);

	// Set up collision query params (ignore self so we don't hit our own actor)
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	// Perform the line trace on the Visibility channel
	const bool bHit = GetWorld()->LineTraceSingleByChannel(
		LastBuildHit,
		Start,
		End,
		ECC_Visibility,
		Params
	);

	// Store result so Blueprint can use it
	bHasBuildHit = bHit;

	// Draw debug line so you can SEE the trace (temporary)
	// Green = hit, Red = no hit
	const FColor LineColor = bHit ? FColor::Green : FColor::Red;
	DrawDebugLine(GetWorld(), Start, End, LineColor, false, 0.0f, 0, 1.0f);

	// Draw a small debug sphere at the hit point (if we hit)
	if (bHit)
	{
		DrawDebugSphere(GetWorld(), LastBuildHit.ImpactPoint, 8.0f, 12, FColor::Green, false, 0.0f);
	}
}

// Returns actor hit by build trace
AActor* ABuildToolActor::GetHitActor()
{
	return LastBuildHit.GetActor();
}

void ABuildToolActor::ApplyBuildDamage(float DamageAmount)
{
	if (!bHasBuildHit) return;

	AActor* HitActor = LastBuildHit.GetActor();
	if (!HitActor) return;

	UGameplayStatics::ApplyDamage(
		HitActor,
		DamageAmount,
		GetInstigatorController(),
		this,
		nullptr
	);
}



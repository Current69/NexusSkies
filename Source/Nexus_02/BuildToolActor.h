// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/HitResult.h" // Needed for FHitResult type
#include "BuildToolActor.generated.h"

UCLASS(Blueprintable)
class NEXUS_02_API ABuildToolActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABuildToolActor();

	// Controls how far our build trace will reach
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="BuildTool")
	float TraceDistance = 8000.0f;

	// Can be called from Blueprint
	UFUNCTION(BlueprintCallable, Category="BuildTool")
	void TestPing();

	// Stores whether our trace hit something this frame
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="BuildTool")
	bool bHasBuildHit = false;

	// Stores the last trace hit result (location, normal, actor hit, etc.)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="BuildTool")
	FHitResult LastBuildHit;

	// Runs the trace and updates bHasBuildHit + LastBuildHit
	UFUNCTION(BlueprintCallable, Category="BuildTool")
	void UpdateBuildTrace();

	// Returns the actor currently under the build trace
	UFUNCTION(BlueprintCallable, Category="BuildTool")
	AActor* GetHitActor();

	// Applies build damage to the actor we are currently hitting
	UFUNCTION(BlueprintCallable, Category="BuildTool")
	void ApplyBuildDamage(float DamageAmount);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

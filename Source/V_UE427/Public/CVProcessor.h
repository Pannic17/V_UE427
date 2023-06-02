// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HiKVisionActor.h"
// #include "NiagaraFunctionLibrary.h"
// #include "NiagaraComponent.h"
#include "CVProcessor.generated.h"

/**
 * 
 */
UCLASS()
class V_UE427_API ACVProcessor : public AHikVisionActor
{
	GENERATED_BODY()

public:
	TArray<FVector> points;
	// TArray<UNiagaraComponent> planets;

	float NPointForceCoefficient;

	UFUNCTION(BlueprintCallable, Category = "CVProcessor")
	TArray<FVector> Set3VectorPoints(TArray<float> XPos, TArray<float> YPos, TArray<float> Sizes);

	UFUNCTION(BlueprintCallable, Category = "CVProcessor")
	
	void IterateAllPlanet();

	UFUNCTION(BlueprintCallable, Category = "CVProcessor")
	TArray<FVector> GetNearest2Points(FVector Position, float Limit);

	UFUNCTION(BlueprintCallable, Category = "CVProcessor")
	FVector GetGravityPoint(FVector Position, float PSizeCoefficient, float PForceCoefficient);

	UFUNCTION(BlueprintCallable, Category = "CVProcessor")
	TArray<FVector> AdjustPlanetPoint(TArray<FVector> CameraPointPosition, int SceneSizeX, int SceneSizeY);

	UFUNCTION(BlueprintCallable, Category = "CVProcessor")
	TArray<FVector> GetRelativePositionArray(TArray<FVector> ScenePoints, FVector PlanetPosition);

	UFUNCTION(BlueprintCallable, Category = "CVProcessor")
	float GetForce2Distance(FVector Point, FVector Planet, int ForceCoefficient, int PlanetSize) const;

	UFUNCTION(BlueprintCallable, Category = "CVProcessor")
	FVector GetGravityDirection(FVector Point, FVector Planet, int ForceCoefficient);

	UFUNCTION(BlueprintCallable, Category = "CVProcessor")
	FVector GetPullBackDirection(FVector Origin, FVector Planet);
	
private:
	static TArray<FVector> Generate3Vector(TArray<float> XPos, TArray<float> YPos, TArray<float> Sizes);
	
	static TArray<FVector> Generate3Vector(TArray<float> XPos, TArray<float> YPos, TArray<float> Sizes,
		float PCoefficient);

	static TMap<FVector, float> CalculateGetNearest3Point(TArray<FVector> Points, FVector Position, float Limit);

	static FVector CalculateGravityPoint(TMap<FVector, float> PointDistances2Planet,
	                                     float PSizeCoefficient, float PForceCoefficient);
	
	static TArray<FVector> AdjustCameraOffset(TArray<FVector> points, FVector2D center, float XOffset, float YOffset);

	static FVector CalRelativePosition(FVector Point, FVector Planet);

	
};

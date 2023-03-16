// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HiKVisionActor.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
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
	TArray<UNiagaraComponent> planets;

	double NPointForceCoefficient;

	void Set3VectorPoints(TArray<float>& XPos, TArray<float>& YPos, TArray<float>& Sizes);

	void IterateAllPlanet();

private:
	static TArray<FVector> Generate3Vector(TArray<float>& XPos, TArray<float>& YPos, TArray<float>& Sizes);
	
	static TArray<FVector> Generate3Vector(TArray<float>& XPos, TArray<float>& YPos, TArray<float>& Sizes,
		double PCoefficient);

	static TMap<FVector, float> GetNearest3Point(TArray<FVector> Points, FVector Position, float Limit);

	static FVector CalculateGravityPoint(TMap<FVector, float> PointDistances2Planet,
	                                     double PSizeCoefficient, double PForceCoefficient);
	
	static TArray<FVector> AdjustCameraOffset(TArray<FVector> points, FVector2D center, float XOffset, float YOffset);
};

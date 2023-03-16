// Fill out your copyright notice in the Description page of Project Settings.


#include "CVProcessor.h"

void ACVProcessor::Set3VectorPoints(TArray<float>& XPos, TArray<float>& YPos, TArray<float>& Sizes)
{
	points = Generate3Vector(XPos, YPos, Sizes);
}

void ACVProcessor::IterateAllPlanet()
{
	// TODO: Iterate all planets
	// calculate the gravity point via CalculateGravityPoint
	// assign 3 attraction point for the planet via GetNearest3Point
}

TArray<FVector> ACVProcessor::Generate3Vector(TArray<float>& XPos, TArray<float>& YPos, TArray<float>& Sizes)
{
	TArray<FVector> Positions;
	for (int i = 0; i < Sizes.Num(); i++)
	{
		FVector Position = FVector(XPos[i], YPos[i], Sizes[i]);
		Positions.Add(Position);
	}
	return Positions;
}

TArray<FVector> ACVProcessor::Generate3Vector(TArray<float>& XPos, TArray<float>& YPos, TArray<float>& Sizes,
	double PCoefficient)
{
	TArray<FVector> Positions;
	for (int i = 0; i < Sizes.Num(); i++)
	{
		FVector Position = FVector(XPos[i], YPos[i], Sizes[i] * PCoefficient);
		Positions.Add(Position);
	}
	return Positions;
}

TMap<FVector, float> ACVProcessor::GetNearest3Point(TArray<FVector> Points, FVector Position, float Limit)
{
	TMap<FVector, float> AllDistances2Planet;
	for (int i = 0; i < Points.Num(); i++)
	{
		float distance = FVector::Distance(Points[i], Position);
		if (distance >= Limit)
		{
			AllDistances2Planet.Add(Points[i], distance);
		}
	}
	AllDistances2Planet.ValueSort([](const float& a, const float& b) { return a < b; });
	TMap<FVector, float> First3Distances2Planet;
	
	TArray<FVector> KeyVectorArray;
	AllDistances2Planet.GenerateKeyArray(KeyVectorArray);
	TArray<float> ValueDistanceArray;
	AllDistances2Planet.GenerateValueArray(ValueDistanceArray);

	for (int i = 0; i < 3 && i < AllDistances2Planet.Num(); i++)
	{
		First3Distances2Planet.Add(KeyVectorArray[i], ValueDistanceArray[i]);
	}
	return First3Distances2Planet;
}

FVector ACVProcessor::CalculateGravityPoint(TMap<FVector, float> PointDistances2Planet,
	double PSizeCoefficient, double PForceCoefficient)
{
	// Calculate the sum of vector for all vectors in PointDistances2Planet
	FVector SumVector = FVector(0, 0, 0);
	TArray<FVector> KeyVectorArray;
	PointDistances2Planet.GenerateKeyArray(KeyVectorArray);
	for (int i = 0; i < PointDistances2Planet.Num(); i++)
	{
		KeyVectorArray[i].Z = KeyVectorArray[i].Z * PSizeCoefficient;
		SumVector += KeyVectorArray[i] * PForceCoefficient;
	}

	const FVector GravityPoint = SumVector / PointDistances2Planet.Num();

	return GravityPoint;
}

TArray<FVector> ACVProcessor::AdjustCameraOffset(TArray<FVector> points, FVector2D center, float XOffset, float YOffset)
{
	for (int i = 0; i < points.Num(); i++)
	{
		points[i].X = (points[i].X - center.X) * XOffset + center.X;
		points[i].Y = (points[i].Y - center.Y) * YOffset + center.Y;
	}
	return points;
}




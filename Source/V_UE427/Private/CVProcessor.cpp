// Fill out your copyright notice in the Description page of Project Settings.


#include "CVProcessor.h"

TArray<FVector> ACVProcessor::Set3VectorPoints(TArray<float> XPos, TArray<float> YPos, TArray<float> Sizes)
{
	points = Generate3Vector(XPos, YPos, Sizes);
	return points;
}

void ACVProcessor::IterateAllPlanet()
{
	// TODO: Iterate all planets
	// calculate the gravity point via CalculateGravityPoint
	// assign 3 attraction point for the planet via CalculateGetNearest3Point
}

TArray<FVector> ACVProcessor::GetNearest2Points(FVector Position, float Limit)
{
	TMap<FVector, float> nearest = CalculateGetNearest3Point(points, Position, Limit);
	TArray<FVector> nearestVectors;
	nearest.GenerateKeyArray(nearestVectors);
	return nearestVectors;
}

FVector ACVProcessor::GetGravityPoint(FVector Position, float PSizeCoefficient, float PForceCoefficient)
{
	const TMap<FVector, float> nearest = CalculateGetNearest3Point(points, Position, 1000);
	const FVector gravityPoint = CalculateGravityPoint(nearest, PSizeCoefficient, PForceCoefficient);
	return gravityPoint;
}

TArray<FVector> ACVProcessor::AdjustPlanetPoint(TArray<FVector> CameraPointPosition, int SceneSizeX, int SceneSizeY)
{
	TArray<FVector> ScenePointPosition;
	int MidX = SceneSizeX / 2;
	int MidY = SceneSizeY / 2;
	for (FVector Point : CameraPointPosition)
	{
		float AdjustX = (Point.X / 1920) * SceneSizeX - MidX;
		float AdjustY = (Point.Y / 1082) * SceneSizeY - MidY;
		ScenePointPosition.Add(FVector(AdjustX, AdjustY, Point.Z));
	}
	points = ScenePointPosition;
	return ScenePointPosition;
}

TArray<FVector> ACVProcessor::GetRelativePositionArray(TArray<FVector> ScenePoints, FVector PlanetPosition)
{
	TArray<FVector> RelativePositionArray;
	for (FVector Point : ScenePoints)
	{
		RelativePositionArray.Add(CalRelativePosition(Point, PlanetPosition));
	}
	return RelativePositionArray;
}

TArray<FVector> ACVProcessor::Generate3Vector(TArray<float> XPos, TArray<float> YPos, TArray<float> Sizes)
{
	TArray<FVector> Positions;
	for (int i = 0; i < Sizes.Num(); i++)
	{
		FVector Position = FVector(XPos[i], YPos[i], Sizes[i]);
		Positions.Add(Position);
	}
	return Positions;
}

TArray<FVector> ACVProcessor::Generate3Vector(TArray<float> XPos, TArray<float> YPos, TArray<float> Sizes,
	float PCoefficient)
{
	TArray<FVector> Positions;
	for (int i = 0; i < Sizes.Num(); i++)
	{
		FVector Position = FVector(XPos[i], YPos[i], Sizes[i] * PCoefficient);
		Positions.Add(Position);
	}
	return Positions;
}

TMap<FVector, float> ACVProcessor::CalculateGetNearest3Point(TArray<FVector> Points, FVector Position, float Limit)
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
	float PSizeCoefficient, float PForceCoefficient)
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

FVector ACVProcessor::CalRelativePosition(FVector Point, FVector Planet)
{
	return Point - Planet;
}

float ACVProcessor::GetForce2Distance(FVector Point, FVector Planet, int ForceCoefficient, int PlanetSize) const
{
	const float Distance = FVector::Distance(Point, Planet);
	if (Distance < PlanetSize)
	{
		const float InsidePercentage = (Distance / PlanetSize) * (PI / 2);
		return sin(InsidePercentage) * ForceCoefficient;
	}
	return ForceCoefficient;
}

FVector ACVProcessor::GetGravityDirection(FVector Point, FVector Planet, int ForceCoefficient)
{
	FVector RelativePosition = CalRelativePosition(Point, Planet);
	FVector DirectionVector = RelativePosition.GetSafeNormal();

	return DirectionVector * ForceCoefficient;
}

FVector ACVProcessor::GetPullBackDirection(FVector Origin, FVector Planet)
{
	return Origin - Planet;
}
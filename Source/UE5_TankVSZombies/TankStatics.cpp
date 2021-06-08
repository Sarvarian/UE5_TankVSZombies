// © 2021 Mohammad Jamal Sarvarian No rights reserved


#include "TankStatics.h"

#define fn auto


fn UTankStatics::FindDeltaAngleDegrees(const float A1, const float A2) -> float
{
	// Find the difference
	float Delta = A2 - A1;

	while (Delta > 180.f)
	{
		Delta = Delta - 360.f;
	}

	while (Delta < -180.f)
	{
		Delta = Delta + 360.f;
	}

	return Delta;
}

fn UTankStatics::FindLookAtAngle2D(const FVector2D& Start, const FVector2D& Target, float& Angle) -> bool
{
	const FVector2D Normal = (Target - Start);

	if (Normal.IsNearlyZero())
		return false;

	Angle = FMath::RadiansToDegrees(FMath::Atan2(Normal.Y, Normal.X));

	return true;
}

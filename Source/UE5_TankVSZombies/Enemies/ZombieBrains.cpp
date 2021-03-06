// © 2021 Mohammad Jamal Sarvarian No rights reserved


#include "ZombieBrains.h"
#include "Zombie.h"

#define PROC void

PROC AZombieBrains::Tick(const float DeltaTime)
{
	if (PawnAsZombie)
	{
		if (const AActor* const Target{ PawnAsZombie->GetTarget() })
		{
			// We do have a target. Shamble toward it and attempt violence!
			const FVector DirectionToTarget{ (Target->GetActorLocation() - PawnAsZombie->GetActorLocation()).GetSafeNormal2D() };
			const float DotToTarget{ FVector::DotProduct(DirectionToTarget, PawnAsZombie->GetActorForwardVector()) };
			const float SidewaysDotToTarget{ FVector::DotProduct(DirectionToTarget, PawnAsZombie->GetActorRightVector()) };
			const float DeltaYawDesired{ FMath::Atan2(SidewaysDotToTarget, DotToTarget) };

			if (PawnAsZombie->ZombieAIShouldAttack())
			{
				PawnAsZombie->AddAttackInput();
			}
			else
			{
				// Move faster when facing toward the target so that we turn more accurately/don't ...
				PawnAsZombie->AddMovementInput(FVector(1.f, 1.f, 1.f), FMath::GetMappedRangeValueClamped(FVector2D(-0.707f, 0.707f), FVector2D(0.f, 1.f), DotToTarget));
				// Attempt the entire turn in one frame. The Zombie itself will cap this, we're only ...
				PawnAsZombie->AddRotationInput(DeltaYawDesired);
			}
		}
	}
}

PROC AZombieBrains::OnPossess(APawn* const InPawn)
{
	Super::OnPossess(InPawn);
	PawnAsZombie = Cast<AZombie>(GetPawn());
}

PROC AZombieBrains::OnUnPossess()
{
	Super::OnUnPossess();
	PawnAsZombie = nullptr;
}

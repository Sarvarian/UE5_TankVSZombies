// © 2021 Mohammad Jamal Sarvarian No rights reserved


#include "Zombie.h"
#include "UE5_TankVSZombies/Tank.h"

#define PROC void
#define SUB auto
#define SETTER void
#define GETTER auto

// Sets default values
AZombie::AZombie()
	:
	Health{ 100.f },
	SightDistance{ 500.f },
	SightAngle{ 60.f },
	YawSpeed{ 90.f },
	WalkSpeed{ 25.f },
	RunSpeed{ 45.f },
	AttackDistance{ 100.f },
	AttackAngle{ 30.f },
	AttackCooldown{ 1.f }
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

// Called when the game starts or when spawned
PROC AZombie::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
PROC AZombie::Tick(const float DeltaTime)
{
	Super::Tick(DeltaTime);

	ZombieAI(DeltaTime);

	// Clear out leftover movement;
	ConsumeMovementInputVector();
	ConsumeRotationInput();
	ConsumeAttackInput();
}

// Called to bind functionality to input
PROC AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

PROC AZombie::ZombieAI_Implementation(const float DeltaSeconds)
{
	// The zombie always moves unless attacking. If moving, it moves between WalkSpeed and RunSpeed.
	const FVector DesiredMovement{ GetAttackInput() ? FVector::ZeroVector : (FMath::GetMappedRangeValueClamped(FVector2D(0.f, 1.f), FVector2D(WalkSpeed, RunSpeed), GetPendingMovementInputVector().X)) * DeltaSeconds * GetActorForwardVector() };
	const FVector OriginalLocation{ GetActorLocation() };
	const FVector DesiredLoc{ OriginalLocation + DesiredMovement };
	const float MaxYawThisFrame{ YawSpeed * DeltaSeconds };
	const FRotator DesiredRot{ GetActorRotation() + FRotator(0.f, FMath::Clamp(GetRotationInput(), -MaxYawThisFrame, MaxYawThisFrame), 0.f) };

	SetActorLocationAndRotation(DesiredLoc, DesiredRot.Quaternion(), true);
	const FVector DistanceWalked{ GetActorLocation() - OriginalLocation };
	if (!DistanceWalked.IsNearlyZero())
	{
		ZombieWalk(DeltaSeconds, DistanceWalked);
	}

	// See if we have a target and deal with it if we do. Find a target if we don't.
	if (const AActor* const Target{ GetTarget() })
	{
		// We do have a target. See if we're attacking, since our attacks always hit.
		const FVector OurLocation{ GetActorLocation() };
		const FVector DirectionToTarget{ (Target->GetActorLocation() - OurLocation).GetSafeNormal2D() };
		const float DotToTarget{ FVector::DotProduct(DirectionToTarget, GetActorForwardVector()) };

		// Check to see if we should attack based on button status and attack cool-down.
		const float CurrentTime{ GetWorld()->GetTimeSeconds() };
		if (GetAttackInput() && (AttackAvailableTime <= CurrentTime))
		{
			AttackAvailableTime = CurrentTime + AttackCooldown;
			ZombieAttack(DeltaSeconds);
			if (DotToTarget >= FMath::Cos(FMath::DegreesToRadians(AttackAngle)))
			{
				const float DistSqXY{ FVector::DistSquaredXY(Target->GetActorLocation(), OurLocation) };
				if (DistSqXY <= (AttackDistance * AttackDistance))
				{
					if (ATank* const TankTarget{ GetTargetAsTank() })
					{
						TankTarget->DamageHealth(10.f);
						if (APlayerController* const PC{ Cast<APlayerController>(TankTarget->GetController()) })
						{
							PC->ClientStartCameraShake(HitShake, 1.f);
						}
					}
					else
					{
						SetTarget(nullptr);
					}
				}
			}
		}
	}
}

SUB AZombie::ZombieAIShouldAttack_Implementation() const -> bool
{
	if (const AActor* const Target = GetTarget())
	{
		// Attack our target if we're in range (distance and angle). For now, we'll use our unmodified ...
		const FVector OurLocation{ GetActorLocation() };
		const FVector DirectionToTarget{ (Target->GetActorLocation() - OurLocation).GetSafeNormal2D() };
		const float DotToTarget{ FVector::DotProduct(DirectionToTarget, GetActorForwardVector()) };
		if (DotToTarget >= FMath::Cos(FMath::DegreesToRadians(AttackAngle)))
		{
			const float DistSqXY{ FVector::DistSquaredXY(Target->GetActorLocation(), OurLocation) };
			if (DistSqXY <= (AttackDistance * AttackDistance))
			{
				// Note that attacking cool-down isn't checked. We don't want this kind of zombie to move ...
				return true;
			}
		}
	}

	return false;
}

SETTER AZombie::SetTarget(AActor* const NewTarget)
{
	TargetActor = NewTarget;
	TargetTank = Cast<ATank>(NewTarget);
}

GETTER AZombie::GetTarget() const -> const AActor* const
{
	return TargetActor;
}

GETTER AZombie::GetTargetAsTank() const -> ATank* const
{
	return TargetTank;
}

//~
//~ New Zombie Input
//~

PROC AZombie::AddRotationInput(const float DeltaYawDegrees)
{
	YawInput += DeltaYawDegrees;
}

PROC AZombie::AddAttackInput()
{
	bAttackInput = true;
}

SUB AZombie::ConsumeRotationInput() -> float
{
	const float RetVal{ YawInput };
	YawInput = 0.f;
	return RetVal;
}

SUB AZombie::ConsumeAttackInput() -> bool
{
	const bool bRetVal{ static_cast<bool>(bAttackInput) };
	bAttackInput = false;
	return bRetVal;
}

GETTER AZombie::GetRotationInput() const -> float
{
	return YawInput;
}

GETTER AZombie::GetAttackInput() const -> bool
{
	return bAttackInput;
}

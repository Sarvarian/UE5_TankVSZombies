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
PROC AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
PROC AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

GETTER AZombie::GetTargetAsTank() const -> const ATank* const
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
	const float RetVal {YawInput};
	YawInput = 0.f;
	return RetVal;
}

SUB AZombie::ConsumeAttackInput() -> bool
{
	const bool bRetVal{ bAttackInput };
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

// © 2021 Mohammad Jamal Sarvarian No rights reserved


#include "Zombie.h"

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
void AZombie::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AZombie::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AZombie::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


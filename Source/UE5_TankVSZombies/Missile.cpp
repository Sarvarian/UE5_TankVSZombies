// © 2021 Mohammad Jamal Sarvarian No rights reserved


#include "Missile.h"

#define proc void

// Sets default values
AMissile::AMissile()
	: Speed{ 200.f }
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
proc AMissile::BeginPlay()
{
	Super::BeginPlay();

	FTimerHandle DummyTimerHandle;
	GetWorldTimerManager().SetTimer(DummyTimerHandle, this, &AMissile::Explode, 1.f);
}

// Called every frame
proc AMissile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector Loc = GetActorLocation();
	Loc += (DeltaTime * Speed) * GetActorForwardVector();
	SetActorLocation(Loc);
}

proc AMissile::Explode_Implementation()
{
	Destroy();
}

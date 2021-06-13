// © 2021 Mohammad Jamal Sarvarian No rights reserved


#include "ZombieBrains.h"
#include "Zombie.h"

#define PROC void

PROC AZombieBrains::OnPossess(APawn* const InPawn)
{
	Super::Possess(InPawn);
	PawnAsZombie = Cast<AZombie>(GetPawn());
}
// © 2021 Mohammad Jamal Sarvarian No rights reserved


#include "Turret.h"
#include "Components/ArrowComponent.h"
#include "PaperSpriteComponent.h"
#include "Tank.h"
#include "Kismet/GameplayStatics.h"
#include "TankStatics.h"

#define proc void

// Sets default values
ATurret::ATurret()
	:
	TurretDirection{ CreateDefaultSubobject<UArrowComponent>(TEXT("TurretDirection")) },
	TurretSprite{ CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TurretSprite")) }
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = TurretDirection;
	TurretSprite->SetupAttachment(TurretDirection);
}

// Called when the game starts or when spawned
proc ATurret::BeginPlay()
{
	Super::BeginPlay();

	check(GetParentActor())
	Tank = Cast<ATank>(GetParentActor());
	check(Tank)
}

// Called every frame
proc ATurret::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	check(TurretDirection);

	if (Tank)
	{
		if (APlayerController * PC{ Cast<APlayerController>(Tank->GetController()) })
		{
			FVector2D AimLocation;
			if (PC->GetMousePosition(AimLocation.X, AimLocation.Y))
			{
				FVector2D TurretLocation{ FVector2D::ZeroVector };
				UGameplayStatics::ProjectWorldToScreen(PC, TurretDirection->GetComponentLocation(), TurretLocation);

				float DesiredYaw{ 0.f };

				if (UTankStatics::FindLookAtAngle2D(TurretLocation, AimLocation, DesiredYaw))
				{
					DesiredYaw += 90.f;
					FRotator CurrentRotation{ TurretDirection->GetComponentRotation() };
					const float DeltaYaw{ UTankStatics::FindDeltaAngleDegrees(CurrentRotation.Yaw, DesiredYaw) };
					const float MaxDeltaYawThisFrame{ YawSpeed * DeltaTime };
					if (MaxDeltaYawThisFrame >= FMath::Abs(DeltaYaw))
					{
						// We can get there on this frame, so just set it.
						CurrentRotation.Yaw = DesiredYaw;
					}
					else
					{
						CurrentRotation.Yaw += FMath::Sign(DeltaYaw) * MaxDeltaYawThisFrame;
					}

					TurretDirection->SetWorldRotation(CurrentRotation);
				}
			}
		}

		// Handle Input.
		const FTankInput& CurrentInput{ Tank->GetCurrentInput() };
		if (CurrentInput.bFire1 && ProjectileType && Fire1Timer > Fire1MaxTime)
		{
			if (UWorld* const World{ GetWorld() })
			{
				if (AMissile * NewProjectile{ Cast<AMissile>(World->SpawnActor(ProjectileType)) })
				{
					FVector Loc = TurretSprite->GetSocketLocation("Muzzle");
					FRotator Rot = TurretSprite->GetComponentRotation();
					NewProjectile->SetActorLocation(Loc);
					NewProjectile->SetActorRotation(Rot);
					Fire1Timer = 0;
				}
			}
		}
		else if (Fire1Timer < (Fire1MaxTime + 1.f))
		{
			Fire1Timer += DeltaTime;
		}
	}
}


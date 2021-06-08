// © 2021 Mohammad Jamal Sarvarian No rights reserved


#include "Tank.h"
#include "Components/ArrowComponent.h"
#include "PaperSpriteComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "TankStatics.h"

#define proc void

proc FTankInput::Sanitize()
{
	MovementInput = RawMovementInput.ClampAxes(-1.f, 1.f);
	if (MovementInput.SizeSquared() > 1.f)
	{
		MovementInput = MovementInput.GetSafeNormal();
	}
	RawMovementInput.Set(0.f, 0.f);
}

proc FTankInput::MoveX(const float AxisValue)
{
	RawMovementInput.X += AxisValue;
}

proc FTankInput::MoveY(const float AxisValue)
{
	RawMovementInput.Y += AxisValue;
}

// Sets default values
ATank::ATank()
	:
	TankDirection{ CreateDefaultSubobject<UArrowComponent>(TEXT("TankDirection")) },
	TankSprite{ CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("TankSprite")) },
	ChildTurret{ CreateDefaultSubobject<UChildActorComponent>(TEXT("Turret")) },
	CameraComponent{ CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent")) },
	YawSpeed{ 180.f }, MoveSpeed{ 100.f }
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	if (!RootComponent)
	{
		RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("TankBase"));
	}

	TankDirection->SetupAttachment(RootComponent);
	TankDirection->SetArrowColor(FLinearColor(0.f, 1.f, 0.f));

	TankSprite->SetupAttachment(TankDirection);

	USpringArmComponent* SpringArm{ CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm")) };
	SpringArm->TargetArmLength = 500.f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->bEnableCameraRotationLag = false;
	SpringArm->bUsePawnControlRotation = false;
	SpringArm->CameraLagSpeed = 2.f;
	SpringArm->bDoCollisionTest = false;
	SpringArm->SetupAttachment(RootComponent);
	SpringArm->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));

	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	CameraComponent->OrthoWidth = 1024.f;
	CameraComponent->AspectRatio = 3.f / 4.f;
	CameraComponent->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	//CameraComponent->SetWorldRotation(FRotator(-90.f, 0.f, 0.f));

	ChildTurret->SetupAttachment(TankDirection);
}

// Called when the game starts or when spawned
proc ATank::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
proc ATank::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TankInput.Sanitize();

	if (!TankInput.MovementInput.IsNearlyZero()) // Move the tank!
	{
		// Rotate the tank! Note that we rotate the TankDirection component,
		// not the RootComponent.
		const FRotator MovementAngle = FVector(TankInput.MovementInput.X, TankInput.MovementInput.Y, 0.f).Rotation();
		const float DeltaYaw = UTankStatics::FindDeltaAngleDegrees(TankDirection->GetComponentRotation().Yaw, MovementAngle.Yaw);
		bool bReverse = false;
		if (DeltaYaw != 0.f)
		{
			float AdjustedDeltaYaw = DeltaYaw;
			if (AdjustedDeltaYaw < -90.f)
			{
				AdjustedDeltaYaw += 180.f;
				bReverse = true;
			}
			else if (AdjustedDeltaYaw > 90.f)
			{
				AdjustedDeltaYaw -= 180.f;
				bReverse = true;
			}

			// Turn toward the desired angle. Stop if we can get there in one frame.
			const float MaxYawThisFrame = YawSpeed * DeltaTime;
			if (MaxYawThisFrame >= FMath::Abs(AdjustedDeltaYaw))
			{
				if (bReverse)
				{
					// Move backward
					TankDirection->SetWorldRotation({
					MovementAngle.Pitch,
					MovementAngle.Yaw + 180.f,
					MovementAngle.Roll
						});
				}
				else
				{
					TankDirection->SetWorldRotation(MovementAngle);
				}
			}
			else
			{
				// Can't reach our desired angle this frame, rotate part way.
				TankDirection->AddLocalRotation(FRotator(0.f, FMath::Sign(AdjustedDeltaYaw) * MaxYawThisFrame, 0.f));
			}
		}

		// Move the tank
		const FVector MovementDirection = TankDirection->GetForwardVector() * (bReverse ? -1.f : 1.f);
		const FVector ActorLocation = GetActorLocation();
		SetActorLocation({
			ActorLocation.X + MovementDirection.X * MoveSpeed * DeltaTime,
			ActorLocation.Y + MovementDirection.Y * MoveSpeed * DeltaTime,
			ActorLocation.Z
			});
	}
}

// Called to bind functionality to input
proc ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	InputComponent->BindAxis(TEXT("MoveX"), this, &ATank::MoveX);
	InputComponent->BindAxis(TEXT("MoveY"), this, &ATank::MoveY);
	InputComponent->BindAction(TEXT("Fire1"), IE_Pressed, this, &ATank::Fire1Pressed);
	InputComponent->BindAction(TEXT("Fire1"), IE_Released, this, &ATank::Fire1Released);
	InputComponent->BindAction(TEXT("Fire2"), IE_Pressed, this, &ATank::Fire2Pressed);
	InputComponent->BindAction(TEXT("Fire2"), IE_Released, this, &ATank::Fire2Released);
}

#define getter auto
getter ATank::GetCurrentInput() const -> const FTankInput&
{
	return TankInput;
}

proc ATank::MoveX(float AxisValue)
{
	TankInput.MoveX(AxisValue);
}

proc ATank::MoveY(float AxisValue)
{
	TankInput.MoveY(AxisValue);
}

proc ATank::Fire1Pressed()
{
	TankInput.bFire1 = true;
}

proc ATank::Fire1Released()
{
	TankInput.bFire1 = false;
}

proc ATank::Fire2Pressed()
{
	TankInput.bFire2 = true;
}

proc ATank::Fire2Released()
{
	TankInput.bFire2 = false;
}

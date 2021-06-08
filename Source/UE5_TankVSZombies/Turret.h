// © 2021 Mohammad Jamal Sarvarian No rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Missile.h"
#include "Turret.generated.h"

UCLASS()
class UE5_TANKVSZOMBIES_API ATurret : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ATurret();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Max turn rate is degrees/second for the turret.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Turret")
	float YawSpeed{ 180.f };

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	class ATank* Tank;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret")
	TSubclassOf<AMissile> ProjectileType;

private:
	// Which way is the turret facing?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* TurretDirection;

	// Sprite for the turret.
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Turret", meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent* TurretSprite;

	const float Fire1MaxTime{ 1 };
	float Fire1Timer;

};

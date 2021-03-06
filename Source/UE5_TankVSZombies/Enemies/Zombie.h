// © 2021 Mohammad Jamal Sarvarian No rights reserved

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraShakeBase.h"
#include "Zombie.generated.h"

UCLASS()
class UE5_TANKVSZOMBIES_API AZombie : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AZombie();

	// Called every frame
	virtual void Tick(const float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	// Set this zombie's target. The base version of this function will handle updating TargetActor and Tank ...
	UFUNCTION(BlueprintCallable, Category = "AI")
	void SetTarget(AActor* const NewTarget);

	// Return the target Actor. Returning NULL indicates no target.
	UFUNCTION(BlueprintCallable, Category = "AI")
	const AActor* const GetTarget() const;

	// Return the target Actor as a Tank, if possible. Returning NULL indicates no target, or that ...
	UFUNCTION(BlueprintCallable, Category = "AI")
	class ATank* const GetTargetAsTank() const;

	// Zombies will call this on Tick.
	UFUNCTION(BlueprintNativeEvent, Category = "AI")
	void ZombieAI(const float DeltaSeconds);
	virtual void ZombieAI_Implementation(const float DeltaSeconds);
	
	// This subroutine asks the zombie if it is in position to attack its current target. It does not actual ...
	UFUNCTION(BlueprintNativeEvent, Category = "AI")
	bool ZombieAIShouldAttack() const;
	virtual bool ZombieAIShouldAttack_Implementation() const;

	// This can be used to animate walking.
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void ZombieWalk(const float DeltaSeconds, const FVector DistanceWalkedThisFrame);

	// This can be used to start attack animations.
	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void ZombieAttack(const float DeltaSeconds);
	
	//~
	//~ New Zombie Input
	//~

	/** Add rotation equal to the yaw (in degrees) provided. */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "AddInput"))
	virtual void AddRotationInput(const float DeltaYawDesired);

	/** Get rotation input. Return pre-clear value. */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
	virtual float GetRotationInput() const ;

	/** Get (and clear) rotation input. Returns pre-clear value. */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
	virtual float ConsumeRotationInput();

	/** Press the attack button. The pawn will know what to do with this. No arguments because it's a bool ... */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "AddInput"))
	void AddAttackInput();

	/** Get the status of the attack button. */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
	virtual bool GetAttackInput() const;

	/** Get (and clear) the status of the attack button */
	UFUNCTION(BlueprintCallable, Category = "Pawn|Input", meta = (Keywords = "ConsumeInput"))
	virtual bool ConsumeAttackInput();


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Camera effect, if any, to play when the player is hit by this Zombie. */
	UPROPERTY(EditAnywhere)
	TSubclassOf<UCameraShakeBase> HitShake;

	/** Current health value. Might be fun to have different values for different attack types. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	float Health;

	/** Sight distance (when no target is present) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	float SightDistance;

	/** Sight half-angle in degrees (when no target is present) */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	float SightAngle;

	/** Max yaw speed per second in degrees. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	float YawSpeed;

	/** Speed when wondering around, or when facing away from target. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	float WalkSpeed;

	/** Speed when rushing toward target. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	float RunSpeed;

	/** Attack distance. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	float AttackDistance;

	/** Half-angle (in degrees) for our attack. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	float AttackAngle;

	/** Minimum time between attacks. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zombie", meta = (ClampMin = "0.0"))
	float AttackCooldown;

	/** Game time, is seconds, when the Zombie will be allowed to attack again. */
	UPROPERTY(Transient, VisibleInstanceOnly, BlueprintReadWrite, Category = "Zombie")
	float AttackAvailableTime;

private:
	/* The actor we're targeting. Will be NULL if there is no target. */
	UPROPERTY(VisibleInstanceOnly, Category = "AI")
	AActor* TargetActor;

	/* The actor we're targeting, cast to a Tank and cached. Will be Null if no target or if the target a ... */
	UPROPERTY(VisibleInstanceOnly, Category = "AI")
	class ATank* TargetTank;

	// Current rotation input.
	float YawInput;

	// Current attack button status.
	uint32 bAttackInput : 1;
};

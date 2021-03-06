// © 2021 Mohammad Jamal Sarvarian No rights reserved

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "ZombieBrains.generated.h"

/**
 * 
 */
UCLASS()
class UE5_TANKVSZOMBIES_API AZombieBrains : public AAIController
{
	GENERATED_BODY()
public:
	virtual void Tick(const float DeltaTime) override;
	virtual void OnPossess(APawn* const InPawn) override;
	virtual void OnUnPossess() override;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Zombie")
	class AZombie* PawnAsZombie;
	

private:
};

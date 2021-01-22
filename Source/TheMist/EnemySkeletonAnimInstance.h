// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemySkeletonAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class THEMIST_API UEnemySkeletonAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:

protected:

public:
	virtual void NativeInitializeAnimation() override;

	UFUNCTION(BlueprintCallable, Category = AnimProp)
	void UpdateCharacterAnimationProperties();

	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = CharacterMovement)
	float characterSpeed;
	
	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = CharacterMovement)
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = CharacterMovement)
	class AEnemySkeleton* EnemySkeleton;
};

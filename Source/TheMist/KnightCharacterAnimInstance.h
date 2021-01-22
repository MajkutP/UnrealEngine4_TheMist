#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "KnightCharacterAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class THEMIST_API UKnightCharacterAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

private:

protected:

public:
	virtual void NativeInitializeAnimation() override;

	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = CharacterMovement)
	float characterSpeed;
	
	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = CharacterMovement)
	class APawn* Pawn;

	UPROPERTY(EditAnywhere, BluePrintReadOnly, Category = CharacterMovement)
	class AKnightCharacter* KnightCharacter;

	UFUNCTION(BlueprintCallable, Category = AnimProp)
	void UpdateCharacterAnimationProperties();
};

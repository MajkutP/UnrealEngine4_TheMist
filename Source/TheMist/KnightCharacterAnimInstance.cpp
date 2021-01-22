#include "KnightCharacterAnimInstance.h"
#include "KnightCharacter.h"

void UKnightCharacterAnimInstance::NativeInitializeAnimation() {
	if (Pawn == nullptr) 
		Pawn = TryGetPawnOwner();
		
		if (Pawn) {
			KnightCharacter = Cast<AKnightCharacter>(Pawn);
		}
}

void UKnightCharacterAnimInstance::UpdateCharacterAnimationProperties() {
	if (Pawn == nullptr)
		Pawn = TryGetPawnOwner();

	if (Pawn != nullptr) {
		FVector Speed = Pawn->GetVelocity();
		FVector HorizontalSpeed = FVector(Speed.X, Speed.Y, 0);
		characterSpeed = HorizontalSpeed.Size();

		if (KnightCharacter == nullptr) {
			KnightCharacter = Cast<AKnightCharacter>(Pawn);
		}
	}
}
// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySkeletonAnimInstance.h"
#include "EnemySkeleton.h"

void UEnemySkeletonAnimInstance::NativeInitializeAnimation(){
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			EnemySkeleton = Cast <AEnemySkeleton>(Pawn);
		}
	}
}

void UEnemySkeletonAnimInstance::UpdateCharacterAnimationProperties() {
	if (Pawn == nullptr) {
		Pawn = TryGetPawnOwner();
		if (Pawn) {
			EnemySkeleton = Cast <AEnemySkeleton>(Pawn);
		}
	}

	if (Pawn) {
		FVector Speed = Pawn->GetVelocity();
		FVector HorizontalSpeed = FVector(Speed.X, Speed.Y, 0);
		characterSpeed = HorizontalSpeed.Size();
	}
}
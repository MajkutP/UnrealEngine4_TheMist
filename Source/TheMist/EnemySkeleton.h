// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemySkeleton.generated.h"

UENUM(BlueprintType)
enum class ESkeletonStatus : uint8 {
	E_IDLE UMETA(DisplayName = "IDLE"),
	E_RUN UMETA(DisplayName = "RUN"),
	E_COMBAT UMETA(DisplayName = "COMBAT"),
	E_DEATH UMETA(DisplayName = "DEATH")
};

UCLASS()
class THEMIST_API AEnemySkeleton : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemySkeleton();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Status)
	ESkeletonStatus Status;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	class USphereComponent* FollowSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	class USphereComponent* AttackSphere;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = AI)
	class AAIController* SkeletonController;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	AKnightCharacter* Target;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	class UParticleSystem* HitParticle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	class UBoxComponent* ArmCollision;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat)
	class UAnimMontage* Montage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	FTimerHandle AttackDelayer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	TSubclassOf<UDamageType> TypeOfDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	FTimerHandle DestroySkeletonDelayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = AI)
	bool bStopAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float EnemyHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float EnemyMaximumHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AI)
	float EnemyDamageToPlayer;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	bool bCombat;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float minTimeToNextAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float maxTimeToNextAttack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float DelaySkeletonDestruction;

	bool bIsTargetAlive;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void SetSkeletonStatus(ESkeletonStatus S);

	ESkeletonStatus GetSkeletonStatus();

	UFUNCTION()
	virtual void FollowSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void FollowSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	virtual void AttackSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void AttackSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void Die();

	UFUNCTION(BlueprintCallable)
	void FollowKnightCharacter(class AKnightCharacter* Knight);

	UFUNCTION()
	void SwordOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SwordOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void EnableWeaponCollision();

	UFUNCTION(BlueprintCallable)
	void DisableWeaponCollision();

	void AttackKnight();

	void StartCombatMontage();

	UFUNCTION(BlueprintCallable)
	void StopCombatMontage();

	UFUNCTION(BlueprintCallable)
	void EndMontageAtDeathPose();

	bool IsSkeletonAlive();

	void ClearActor();
};

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KnightCharacter.generated.h"

UENUM(BlueprintType)
enum class EKnightLocomotionStatus : uint8 {
	E_WALK UMETA(DisplayName = "WALK"),
	E_RUN UMETA(DisplayName = "Sprinting"),
	E_DEATH UMETA(DisplayName = "Death")
};

UENUM(BlueprintType)
enum class EKnightStaminaStatus : uint8 {
	ES_NORMAL UMETA(DisplayName = "ES_NORMAL"),
	ES_UNDERMIMIMUM UMETA(DisplayName = "ES_UNDERMIMIMUM"),
	ES_ZERO UMETA(DisplayName = "ES_ZERO"),
	ES_RECOVER UMETA(DisplayName = "ES_RECOVER")
};

UCLASS()
class THEMIST_API AKnightCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKnightCharacter();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enum)
	EKnightLocomotionStatus KnightLocomotionStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Enum)
	EKnightStaminaStatus KnightStaminaStatus;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerCamera, meta = (AllowPricateAccess = "true"))
	class USpringArmComponent* CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = PlayerCamera, meta = (AllowPricateAccess = "true"))
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	class UBoxComponent* SwordCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	class UParticleSystem* HitParticle;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float CharacterTurnRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float MaxHealth;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float MaxStamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float MaxMana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float Stamina;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stats)
	float Maana;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	float WalkSpeed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Enum)
	float RunSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
	float StaimnaUsage;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
	float MinimumRunStamina;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Enum)
	bool bRunKeyPressed;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Anim)
	bool bCombat;

	bool bLMB;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	float SwordDamage;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Anim)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat)
	TSubclassOf<UDamageType> TypeOfDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat)
	AController* SwordInstigator;

	bool bMoveForwardBackward;

	bool bMoverightLeft;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void MoveCharacterForwardOrBackward(float value);

	void MoveCharacterRightOrLeft(float value);

	void TurnCharacterRightOrLeftAtRate(float rate);

	void LookCharacterUpOrDownAtRate(float rate);

	class USpringArmComponent* getCameraArm() const;

	class UCameraComponent* getCamera() const;

	void DecreaseHealth(float amount);

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

	void IncreaseHealth(float amount);

	void DecreaseMana(float amount);

	void IncreaseMana(float amount);

	void Die();

	void SetKnightLocomotionStatus(EKnightLocomotionStatus Status);

	void SetKnightStaminaStatus(EKnightStaminaStatus Status);

	void RunKeyPressed();

	void RunKeyNotPressed();

	void LeftMouseButtonActive();

	void LeftMouseButtonNonActive();

	void StartCombatMontage();

	UFUNCTION(BlueprintCallable)
	void StopCombatMontage();

	UFUNCTION()
	void SwordOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void SwordOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION(BlueprintCallable)
	void EnableWeaponCollision();

	UFUNCTION(BlueprintCallable)
	void DisableWeaponCollision();

	void SetInstigator(AController* Instigator);

	UFUNCTION(BlueprintCallable)
	void EndAtDeath();

	UFUNCTION(BlueprintCallable)
	void SaveGame();

	UFUNCTION(BlueprintCallable)
	void LoadGame();
};

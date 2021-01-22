#include "KnightCharacter.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/BoxComponent.h"
#include "EnemySkeleton.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "TheMistSaveGame.h"

// Sets default values
AKnightCharacter::AKnightCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessPlayer = EAutoReceiveInput::Player0;

	CameraArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraArm"));
	CameraArm->SetupAttachment(GetRootComponent());
	CameraArm->bUsePawnControlRotation = true;
	CameraArm->TargetArmLength = 300.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(CameraArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	SwordCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Sword"));
	SwordCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("WeaponSocket"));

	CharacterTurnRate = 60.f;

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationYaw = false;

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 600.f, 0.f);

	MaxHealth = 100.0f;
	MaxStamina = 100.0f;
	MaxMana = 60.f;

	Health = 100.0f;
	Stamina = 100.0f;
	Maana = 0.0f;

	WalkSpeed = 200.0f;
	RunSpeed = 500.0f;

	KnightLocomotionStatus = EKnightLocomotionStatus::E_WALK;
	bRunKeyPressed = false;
	bMoveForwardBackward = false;
	bMoverightLeft = false;

	KnightStaminaStatus = EKnightStaminaStatus::ES_NORMAL;
	StaimnaUsage = 15.f;
	MinimumRunStamina = 30.f;

	bLMB = false;
	bCombat = false;

	SwordDamage = 40.0f;

}

// Called when the game starts or when spawned
void AKnightCharacter::BeginPlay()
{
	Super::BeginPlay();

	SwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SwordCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	SwordCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SwordCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	SwordCollision->OnComponentBeginOverlap.AddDynamic(this, &AKnightCharacter::SwordOnOverlapBegin);
	SwordCollision->OnComponentEndOverlap.AddDynamic(this, &AKnightCharacter::SwordOnOverlapEnd);

	SetInstigator(AKnightCharacter::GetController());
}

// Called every frame
void AKnightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (KnightLocomotionStatus == EKnightLocomotionStatus::E_DEATH) {
		return;
	}

	float DeltaStamina = StaimnaUsage * DeltaTime;
	switch (KnightStaminaStatus) {
	case EKnightStaminaStatus::ES_NORMAL:
		if (bRunKeyPressed) {
			if (Stamina - DeltaStamina <= MinimumRunStamina) {
				SetKnightStaminaStatus(EKnightStaminaStatus::ES_UNDERMIMIMUM);
				Stamina -= DeltaStamina;
			}
			else {
				Stamina -= DeltaStamina;
			}
			if (bMoveForwardBackward || bMoverightLeft) {
				SetKnightLocomotionStatus(EKnightLocomotionStatus::E_RUN);
			}
			else {
				SetKnightLocomotionStatus(EKnightLocomotionStatus::E_WALK);
			}
		}
		else {
			if (Stamina + DeltaStamina >= MaxStamina) {
				Stamina = MaxStamina;
			}
			else {
				Stamina += DeltaStamina;
			}
			SetKnightLocomotionStatus(EKnightLocomotionStatus::E_WALK);
		}
		break;
	case EKnightStaminaStatus::ES_UNDERMIMIMUM:
		if (bRunKeyPressed) {
			if (Stamina - DeltaStamina <= 0.0f) {
				SetKnightStaminaStatus(EKnightStaminaStatus::ES_ZERO);
				Stamina = 0.0f;
				SetKnightLocomotionStatus(EKnightLocomotionStatus::E_WALK);
			}
			else {
				Stamina -= DeltaStamina;
				if (bMoveForwardBackward || bMoverightLeft) {
					SetKnightLocomotionStatus(EKnightLocomotionStatus::E_RUN);
				}
				else {
					SetKnightLocomotionStatus(EKnightLocomotionStatus::E_WALK);
				}
			}
		}
		else {
			if (Stamina + DeltaStamina >= MinimumRunStamina) {
				SetKnightStaminaStatus(EKnightStaminaStatus::ES_NORMAL);
				Stamina += DeltaStamina;
			}
			else{
				Stamina += DeltaStamina;
			}
			SetKnightLocomotionStatus(EKnightLocomotionStatus::E_WALK);
		}
		break;
	case EKnightStaminaStatus::ES_ZERO:
		if (bRunKeyPressed) {
			Stamina = 0.f;
		}
		else {
			SetKnightStaminaStatus(EKnightStaminaStatus::ES_RECOVER);
			Stamina += DeltaStamina;
		}
		SetKnightLocomotionStatus(EKnightLocomotionStatus::E_WALK);
		break;
	case EKnightStaminaStatus::ES_RECOVER:
		if (Stamina + DeltaStamina >= MinimumRunStamina) {
			SetKnightStaminaStatus(EKnightStaminaStatus::ES_NORMAL);
			Stamina += DeltaStamina;
		}
		else {
			Stamina += DeltaStamina;
		}
		SetKnightLocomotionStatus(EKnightLocomotionStatus::E_WALK);
		break;
	default:
		break;
	}
}

// Called to bind functionality to input
void AKnightCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);
	PlayerInputComponent->BindAxis("MoveForwardBackward", this, &AKnightCharacter::MoveCharacterForwardOrBackward);
	PlayerInputComponent->BindAxis("MoveRightLeft", this, &AKnightCharacter::MoveCharacterRightOrLeft);

	PlayerInputComponent->BindAxis("TurnRightLeft", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUpDown", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAxis("TurnRightLeftAtRate", this, &AKnightCharacter::TurnCharacterRightOrLeftAtRate);
	PlayerInputComponent->BindAxis("LookUpDownAtRate", this, &AKnightCharacter::LookCharacterUpOrDownAtRate);

	PlayerInputComponent->BindAction("Run", IE_Pressed, this, &AKnightCharacter::RunKeyPressed);
	PlayerInputComponent->BindAction("Run", IE_Released, this, &AKnightCharacter::RunKeyNotPressed);

	PlayerInputComponent->BindAction("LeftMouseButton", IE_Pressed, this, &AKnightCharacter::LeftMouseButtonActive);
	PlayerInputComponent->BindAction("LeftMouseButton", IE_Released, this, &AKnightCharacter::LeftMouseButtonNonActive);
}


void AKnightCharacter::MoveCharacterForwardOrBackward(float value) {
	bMoveForwardBackward = false;
	if (Controller != nullptr && value != 0.f && !bCombat && KnightLocomotionStatus != EKnightLocomotionStatus::E_DEATH) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, value * 0.2f);

		bMoveForwardBackward = true;
	}
}

void AKnightCharacter::MoveCharacterRightOrLeft(float value) {
	bMoverightLeft = false;
	if (Controller != nullptr && value != 0.f && !bCombat && KnightLocomotionStatus != EKnightLocomotionStatus::E_DEATH) {
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, value * 0.2f);

		bMoverightLeft = true;
	}
}

void AKnightCharacter::TurnCharacterRightOrLeftAtRate(float rate) {
	AddControllerYawInput(GetWorld()->GetDeltaSeconds() * CharacterTurnRate * rate);
}

void AKnightCharacter::LookCharacterUpOrDownAtRate(float rate) {
	AddControllerPitchInput(GetWorld()->GetDeltaSeconds() * CharacterTurnRate * rate);
}

class USpringArmComponent* AKnightCharacter::getCameraArm() const {
	return CameraArm;
}

class UCameraComponent* AKnightCharacter::getCamera() const {
	return Camera;
}

void AKnightCharacter::DecreaseHealth(float amount) {
	if (Health - amount <= 0.0f) {
		Health -= amount;
		Die();
	}
	else {
		Health -= amount;
	}
}

void AKnightCharacter::IncreaseHealth(float amount) {
	if (Health + amount > MaxHealth) {
		Health = MaxHealth;
	}
	else {
		Health += amount;
	}
}

void AKnightCharacter::DecreaseMana(float amount){
	if (Maana - amount <= 0.0f) {
		Maana = 0;
	}
	else {
		Maana -= amount;
	}
}

void AKnightCharacter::IncreaseMana(float amount) {
	if (Maana + amount > MaxMana) {
		Maana = MaxMana;
	}
	else {
		Maana += amount;
	}
}

void AKnightCharacter::Die() {
	UAnimInstance* Animation = GetMesh()->GetAnimInstance();
	if (Animation) {
		Animation->Montage_Play(Montage, 1.f);
		Animation->Montage_JumpToSection(FName("Death"));
	}
	SetKnightLocomotionStatus(EKnightLocomotionStatus::E_DEATH);
}

void AKnightCharacter::EndAtDeath() {
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;
}

void AKnightCharacter::SetKnightLocomotionStatus(EKnightLocomotionStatus Status) {
	KnightLocomotionStatus = Status;
	if (KnightLocomotionStatus == EKnightLocomotionStatus::E_RUN) {
		GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
	else {
		GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}
}

void AKnightCharacter::RunKeyPressed() {
	bRunKeyPressed = true;
}

void AKnightCharacter::RunKeyNotPressed() {
	bRunKeyPressed = false;
}

void AKnightCharacter::SetKnightStaminaStatus(EKnightStaminaStatus Status) {
	KnightStaminaStatus = Status;
}

void AKnightCharacter::LeftMouseButtonActive() {
	bLMB = true;
	if (KnightLocomotionStatus != EKnightLocomotionStatus::E_DEATH) {
		StartCombatMontage();
	}
}

void AKnightCharacter::LeftMouseButtonNonActive() {
	bLMB = false;
}

void AKnightCharacter::StartCombatMontage() {
	if (!bCombat && KnightLocomotionStatus != EKnightLocomotionStatus::E_DEATH) {
	bCombat = true;
		UAnimInstance* Animation = GetMesh()->GetAnimInstance();
		if (Animation) {
			Animation->Montage_Play(Montage, 1.4f);
			Animation->Montage_JumpToSection(FName("Attack1"), Montage);
		}
	}
}

void AKnightCharacter::StopCombatMontage() {
	bCombat = false;

	UAnimInstance* Animation = GetMesh()->GetAnimInstance();
	if (Animation) {
		Animation->Montage_Stop(0.f);
	}
}

void AKnightCharacter::SwordOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		AEnemySkeleton* Skeleton = Cast<AEnemySkeleton>(OtherActor);
		if (Skeleton) {
			if (Skeleton->HitParticle) {
				const USkeletalMeshSocket* SkeletonHitSocket = GetMesh()->GetSocketByName("ParticleSocket");
				if (SkeletonHitSocket) {
					FVector SocketLocation = SkeletonHitSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Skeleton->HitParticle, SocketLocation, FRotator(0.f), false);
				}
			}
			if (TypeOfDamage) {
				UGameplayStatics::ApplyDamage(Skeleton, SwordDamage, SwordInstigator, this, TypeOfDamage);
			}
		}
	}
}

void AKnightCharacter::SwordOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

}

void AKnightCharacter::EnableWeaponCollision() {
	SwordCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AKnightCharacter::DisableWeaponCollision() {
	SwordCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

float AKnightCharacter::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	if (Health - DamageAmount <= 0.0f) {
		Health -= DamageAmount;
		Die();
		if (DamageCauser) {
			AEnemySkeleton* Skeleton = Cast<AEnemySkeleton>(DamageCauser);
			if (Skeleton) {
				Skeleton->bIsTargetAlive = false;
			}
		}
	}
	else {
		Health -= DamageAmount;
	}
	return DamageAmount;
}

void AKnightCharacter::SetInstigator(AController* Instigator) {
	SwordInstigator = Instigator;
}

void AKnightCharacter::SaveGame() {
	UTheMistSaveGame* SavedGame = Cast<UTheMistSaveGame>(UGameplayStatics::CreateSaveGameObject(UTheMistSaveGame::StaticClass()));
	SavedGame->CharStats.Health = Health;
	SavedGame->CharStats.MaxHealth = MaxHealth;
	SavedGame->CharStats.Stamina = Stamina;
	SavedGame->CharStats.MaxStamina = MaxStamina;
	SavedGame->CharStats.Mana = Maana;
	SavedGame->CharStats.MaxMana = MaxMana;
	SavedGame->CharStats.Rotation = GetActorRotation();
	SavedGame->CharStats.Location = GetActorLocation();

	UGameplayStatics::SaveGameToSlot(SavedGame, SavedGame->CharacterName, SavedGame->UserIndex);
}

void AKnightCharacter::LoadGame() {
	UTheMistSaveGame* LoadGame = Cast<UTheMistSaveGame>(UGameplayStatics::CreateSaveGameObject(UTheMistSaveGame::StaticClass()));
	LoadGame = Cast<UTheMistSaveGame>(UGameplayStatics::LoadGameFromSlot(LoadGame->CharacterName, LoadGame->UserIndex));
	Health = LoadGame->CharStats.Health;
	MaxHealth = LoadGame->CharStats.MaxHealth;
	Stamina = LoadGame->CharStats.Stamina;
	MaxStamina = LoadGame->CharStats.MaxStamina;
	Maana = LoadGame->CharStats.Mana;
	MaxMana = LoadGame->CharStats.MaxMana;
	SetActorRotation(LoadGame->CharStats.Rotation);
	SetActorLocation(LoadGame->CharStats.Location);

}
#include "EnemySkeleton.h"
#include "Components/SphereComponent.h"
#include "AIController.h"
#include "KnightCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Sound/SoundCue.h"
#include "Animation/AnimInstance.h"
#include "TimerManager.h"
#include "Components/CapsuleComponent.h"

// Sets default values
AEnemySkeleton::AEnemySkeleton()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FollowSphere = CreateDefaultSubobject<USphereComponent>(TEXT("FollowSphere"));
	FollowSphere->SetupAttachment(GetRootComponent());
	AttackSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AttackSphere"));
	AttackSphere->SetupAttachment(GetRootComponent());

	ArmCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("Arm"));
	ArmCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("EnemySocket"));

	FollowSphere->InitSphereRadius(488.f);
	AttackSphere->InitSphereRadius(83.f);

	Status = ESkeletonStatus::E_IDLE;

	bStopAttacking = false;
	bCombat = false;
	bIsTargetAlive = false;

	EnemyHealth = 100.0f;
	EnemyMaximumHealth = 100.0f;
	EnemyDamageToPlayer = 10.f;

	minTimeToNextAttack = 0.4f;
	maxTimeToNextAttack = 1.8f;

	DelaySkeletonDestruction = 50.f;
}

// Called when the game starts or when spawned
void AEnemySkeleton::BeginPlay()
{
	Super::BeginPlay();
	SkeletonController = Cast<AAIController>(GetController());
	FollowSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemySkeleton::FollowSphereOnOverlapBegin);
	FollowSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemySkeleton::FollowSphereOnOverlapEnd);
	AttackSphere->OnComponentBeginOverlap.AddDynamic(this, &AEnemySkeleton::AttackSphereOnOverlapBegin);
	AttackSphere->OnComponentEndOverlap.AddDynamic(this, &AEnemySkeleton::AttackSphereOnOverlapEnd);

	ArmCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	ArmCollision->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	ArmCollision->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	ArmCollision->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);

	ArmCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemySkeleton::SwordOnOverlapBegin);
	ArmCollision->OnComponentEndOverlap.AddDynamic(this, &AEnemySkeleton::SwordOnOverlapEnd);

	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
}

// Called every frame
void AEnemySkeleton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AEnemySkeleton::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemySkeleton::SetSkeletonStatus(ESkeletonStatus S) {
	Status = S;
}

ESkeletonStatus AEnemySkeleton::GetSkeletonStatus() {
	return Status;
}

void AEnemySkeleton::FollowSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor && IsSkeletonAlive()) {
		AKnightCharacter* Knight = Cast<AKnightCharacter>(OtherActor);
		if (Knight) {
			FollowKnightCharacter(Knight);
		}
	}
}
 
void AEnemySkeleton::FollowSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor) {
		AKnightCharacter* Knight = Cast<AKnightCharacter>(OtherActor);
		if (Knight) {
			bIsTargetAlive = false;
			SetSkeletonStatus(ESkeletonStatus::E_IDLE);
			if (SkeletonController) {
				SkeletonController->StopMovement();
			}
		}
	}
}
 
void AEnemySkeleton::AttackSphereOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor && IsSkeletonAlive()) {
		AKnightCharacter* Knight = Cast<AKnightCharacter>(OtherActor);
		if(Knight) {
			bIsTargetAlive = true;
			Target = Knight;
			bStopAttacking = true;
			AttackKnight();
		}
	}
}
 
void AEnemySkeleton::AttackSphereOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {
	if (OtherActor) {
		AKnightCharacter* Knight = Cast<AKnightCharacter>(OtherActor);
		if (Knight) {
			bStopAttacking = false;
			FollowKnightCharacter(Knight);
			Target = nullptr;
			GetWorldTimerManager().ClearTimer(AttackDelayer);
		}
	}
}

void AEnemySkeleton::FollowKnightCharacter(AKnightCharacter* Knight) {
	SetSkeletonStatus(ESkeletonStatus::E_RUN);
	if (SkeletonController) {
		FAIMoveRequest Request;
		Request.SetGoalActor(Knight);
		Request.SetAcceptanceRadius(12.0f);
		FNavPathSharedPtr NavPathSharedPtr;
		SkeletonController->MoveTo(Request, &NavPathSharedPtr);
	}
}

void AEnemySkeleton::SwordOnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) {
	if (OtherActor) {
		AKnightCharacter * Knight = Cast<AKnightCharacter>(OtherActor);
		if (Knight) {
			if (Knight->HitParticle) {
				const USkeletalMeshSocket* HitSocket = GetMesh()->GetSocketByName("HitSocket");
				if (HitSocket) {
					FVector SocketLocation = HitSocket->GetSocketLocation(GetMesh());
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), Knight->HitParticle, SocketLocation, FRotator(0.f), false);
				}
			}
			if (TypeOfDamage) {
				UGameplayStatics::ApplyDamage(Knight, EnemyDamageToPlayer, SkeletonController, this, TypeOfDamage);
			}
		}
	}
}

void AEnemySkeleton::SwordOnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) {

}

void AEnemySkeleton::EnableWeaponCollision() {
	ArmCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AEnemySkeleton::DisableWeaponCollision() {
	ArmCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemySkeleton::AttackKnight() {
	if (IsSkeletonAlive() && bIsTargetAlive) {
		if (SkeletonController) {
			SkeletonController->StopMovement();
			SetSkeletonStatus(ESkeletonStatus::E_COMBAT);
		}
		if (!bCombat) {
			bCombat = true;
			UAnimInstance* Animation = GetMesh()->GetAnimInstance();
			if (Animation) {
				Animation->Montage_Play(Montage, 1.3f);
				Animation->Montage_JumpToSection(FName("Attack1"), Montage);
			}
		}
	}
}

void AEnemySkeleton::StopCombatMontage() {
	bCombat = false;
	if (bStopAttacking) {
		float nextAttack = FMath::FRandRange(minTimeToNextAttack, maxTimeToNextAttack);
		GetWorldTimerManager().SetTimer(AttackDelayer, this, &AEnemySkeleton::AttackKnight, nextAttack);

		AttackKnight();
	}
}

float AEnemySkeleton::TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) {
	if (EnemyHealth - DamageAmount <= 0) {
		EnemyHealth -= DamageAmount;
		Die();
	}
	else {
		EnemyHealth -= DamageAmount;
	}
	return DamageAmount;
}

void AEnemySkeleton::Die() {
	UAnimInstance* Animation = GetMesh()->GetAnimInstance();
	if (Animation) {
		Animation->Montage_Play(Montage, 1.f);
		Animation->Montage_JumpToSection(FName("Death"), Montage);
	}
	SetSkeletonStatus(ESkeletonStatus::E_DEATH);
	ArmCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	AttackSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	FollowSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AEnemySkeleton::EndMontageAtDeathPose() {
	GetMesh()->bPauseAnims = true;
	GetMesh()->bNoSkeletonUpdate = true;

	GetWorldTimerManager().SetTimer(DestroySkeletonDelayer, this, &AEnemySkeleton::ClearActor, DelaySkeletonDestruction);
}

bool AEnemySkeleton::IsSkeletonAlive() {
	return GetSkeletonStatus() != ESkeletonStatus::E_DEATH;
}

void AEnemySkeleton::ClearActor() {
	Destroy();
}
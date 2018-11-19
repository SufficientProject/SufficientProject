// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "Engine.h"
#include "SufficientProject.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

// Define collision
#define COLLISION_ENEMY ECollisionChannel::ECC_GameTraceChannel1
#define COLLISION_PLAYER ECollisionChannel::ECC_GameTraceChannel2
#define COLLISION_PROJECTILE ECollisionChannel::ECC_GameTraceChannel3

// Sets default values
ABullet::ABullet()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(8.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("projectile");

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	CollisionComp->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(COLLISION_PLAYER, ECollisionResponse::ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(COLLISION_ENEMY, ECollisionResponse::ECR_Block);
	CollisionComp->SetCollisionResponseToChannel(COLLISION_PROJECTILE, ECollisionResponse::ECR_Ignore);

	CollisionComp->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	CollisionComp->SetNotifyRigidBodyCollision(true);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 1500.f;
	ProjectileMovement->MaxSpeed = 1500.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->Bounciness = 0.0f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 5.0f;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();

	if (GetInstigator() == UGameplayStatics::GetPlayerCharacter(GetWorld(), 0))
		CollisionComp->SetCollisionResponseToChannel(COLLISION_PLAYER, ECollisionResponse::ECR_Ignore);
	else
		CollisionComp->SetCollisionResponseToChannel(COLLISION_ENEMY, ECollisionResponse::ECR_Ignore);
}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 10.0f, GetActorLocation());
		Destroy();

		//UE_LOG(LogTemp, Warning, TEXT("hit!"));
	}
	
	if (GetInstigator() != OtherActor)
	{
		TSubclassOf<UDamageType> const ValidDamageTypeClass = TSubclassOf<UDamageType>(UDamageType::StaticClass());
		FDamageEvent DamageEvent(ValidDamageTypeClass);
		APlayerController* PlayerController = Cast<APlayerController>(GetInstigatorController());
		OtherActor->TakeDamage(damage, DamageEvent, PlayerController, this);
		Destroy();

		//UE_LOG(LogTemp, Warning, TEXT("hit!"));
	}
}
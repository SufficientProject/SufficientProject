// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"
#include "Engine.h"
#include "SufficientProject.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/SphereComponent.h"

/*
// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	speed = 600.0f;
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FVector loc = GetActorLocation();
	loc += (DeltaTime * speed) * GetTransform().GetUnitAxis(EAxis::X);
	SetActorLocation(loc);
}*/

ABullet::ABullet()
{
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(8.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");

	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	RootComponent = CollisionComp;

	CollisionComp->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	CollisionComp->SetCollisionResponseToAllChannels(ECR_Block);

	CollisionComp->OnComponentHit.AddDynamic(this, &ABullet::OnHit);

	CollisionComp->SetNotifyRigidBodyCollision(true);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 600.f;
	ProjectileMovement->MaxSpeed = 600.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = false;
	ProjectileMovement->Bounciness = 0.0f;
	ProjectileMovement->ProjectileGravityScale = 0.f;

	InitialLifeSpan = 5.0f;
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();


}

void ABullet::OnHit(UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	if ((OtherActor != NULL) && (OtherActor != this) && (OtherComp != NULL) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * 10.0f, GetActorLocation());
		Destroy();

		UE_LOG(LogTemp, Warning, TEXT("hit!"));
	}
	
	if (GetInstigator() != OtherActor)
	{
		//...//
		Destroy();

		UE_LOG(LogTemp, Warning, TEXT("hit!"));
	}
}
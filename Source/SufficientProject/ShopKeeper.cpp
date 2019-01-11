// Fill out your copyright notice in the Description page of Project Settings.

#include "ShopKeeper.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"

#define COLLISION_ENEMY ECollisionChannel::ECC_GameTraceChannel1
#define COLLISION_PLAYER ECollisionChannel::ECC_GameTraceChannel2

AShopKeeper::AShopKeeper()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;

	// Use only Yaw in controll of the character
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	// Set the size of our collision capsule
	GetCapsuleComponent()->SetCapsuleHalfHeight(96.0f);
	GetCapsuleComponent()->SetCapsuleRadius(40.0f);

	GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("enemy");
	GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics, true);

	GetCapsuleComponent()->SetCollisionObjectType(COLLISION_PLAYER);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_ENEMY, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PLAYER, ECollisionResponse::ECR_Block);

	// Prevent all automatic rotation behavior on the camera, character and camera component
	GetCharacterMovement()->bOrientRotationToMovement = false;

	// Configure character movement
	GetCharacterMovement()->GravityScale = 2.0f;
	GetCharacterMovement()->AirControl = 0.10f;
	GetCharacterMovement()->JumpZVelocity = 1000.0f;
	GetCharacterMovement()->GroundFriction = 10.0f;
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
	GetCharacterMovement()->MaxFlySpeed = 600.0f;

	// Lock character motion onto the XZ plane
	GetCharacterMovement()->bConstrainToPlane = true;
	GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.0f, -1.0f, 0.0f));

	// Set behavior like 2D platformet character with flat bottom
	GetCharacterMovement()->bUseFlatBaseForFloorChecks = true;
}

void AShopKeeper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateAnimation();
}

void AShopKeeper::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();
	UPaperFlipbook* DesiredAnimation;
	float updateTime = 1.0f / 15.0f;

	DesiredAnimation = IdleAnimation;

	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}
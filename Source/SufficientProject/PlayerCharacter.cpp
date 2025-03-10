// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacter.h"
#include "Bullet.h"
#include "PaperFlipbookComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Camera/CameraComponent.h"

// Define collision
#define COLLISION_ENEMY ECollisionChannel::ECC_GameTraceChannel1
#define COLLISION_PLAYER ECollisionChannel::ECC_GameTraceChannel2

// Sets default values
APlayerCharacter::APlayerCharacter()
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

	GetCapsuleComponent()->BodyInstance.SetCollisionProfileName("player");
	GetCapsuleComponent()->BodyInstance.SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics, true);

	GetCapsuleComponent()->SetCollisionObjectType(COLLISION_PLAYER);
	GetCapsuleComponent()->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_ENEMY, ECollisionResponse::ECR_Block);
	GetCapsuleComponent()->SetCollisionResponseToChannel(COLLISION_PLAYER, ECollisionResponse::ECR_Block);

	// Create a camera bom attached to the root
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 500.0f;
	CameraBoom->SocketOffset = FVector(0.0f, 0.0f, 75.0f);
	CameraBoom->bAbsoluteRotation = true;
	CameraBoom->bDoCollisionTest = false;
	CameraBoom->RelativeRotation = FRotator(0.0f, -90.0f, 0.0f);

	// Create an orthograpic camera (no perspective) and attach it to the cameraboom
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->ProjectionMode = ECameraProjectionMode::Perspective;
	CameraComponent->FieldOfView = 90.f;
	//CameraComponent->OrthoWidth = 2048.0f;
	CameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	// Prevent all automatic rotation behavior on the camera, character and camera component
	CameraBoom->bAbsoluteRotation = true;
	CameraComponent->bUsePawnControlRotation = false;
	CameraComponent->bAutoActivate = true;
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

	// Set default values for player character
	currentHealth = 10;
	maxHealth = 10;

	turnedRight = true;
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateCharacter();
}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &APlayerCharacter::Fire);

	PlayerInputComponent->BindAxis("MoveRight", this, &APlayerCharacter::MoveRight);
}

void APlayerCharacter::UpdateAnimation()
{
	const FVector PlayerVelocity = GetVelocity();
	const float PlayerSpeedSqr = PlayerVelocity.SizeSquared();

	// Check if moving
	UPaperFlipbook* DesiredAnimation = (PlayerSpeedSqr > 0.0f) ? RunningAnimation : IdleAnimation;
	if (GetSprite()->GetFlipbook() != DesiredAnimation)
	{
		GetSprite()->SetFlipbook(DesiredAnimation);
	}
}

void APlayerCharacter::MoveRight(float value)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), value);
}

void APlayerCharacter::UpdateCharacter()
{
	// Update animation to match the motion
	UpdateAnimation();

	// Setup rotation of the controller based on the direction we are travelling
	const FVector PlayerVelocity = GetVelocity();
	float TravelDirection = PlayerVelocity.X;

	// Set the rotation so that the character faces his direction of travel
	if (Controller != nullptr)
	{
		if (TravelDirection < 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 180.0f, 0.0f));
			SetTurnedRight(false);
		}
		else if (TravelDirection > 0.0f)
		{
			Controller->SetControlRotation(FRotator(0.0f, 0.0f, 0.0f));
			SetTurnedRight(true);
		}
	}
}

float APlayerCharacter::GetCurrentHealth()
{
	return currentHealth;
}

void APlayerCharacter::SetCurrentHealth(float health)
{
	if (health <= 0.f)
		currentHealth = 0.f;
	else if (health > GetMaxHealth())
		currentHealth = GetMaxHealth();
	else
		currentHealth = health;

}

void APlayerCharacter::ChangeCurrentHealth(float value)
{
	SetCurrentHealth(GetCurrentHealth() + value);
	UE_LOG(LogTemp, Warning, TEXT("%f"), GetCurrentHealth());
}

float APlayerCharacter::GetMaxHealth()
{
	return maxHealth;
}

void APlayerCharacter::SetMaxHealth(float health)
{
	if (health > 0.f)
		maxHealth = health;
}

void APlayerCharacter::Fire()
{
	if (bullet)
	{
		FVector loc = CameraBoom->GetComponentLocation();
		FRotator rot;

		if (GetTurnedRight())
			rot = FRotator(0.0f, 0.0f, 0.0f);
		else
			rot = FRotator(0.0f, 180.0f, 0.0f);
		
		FActorSpawnParameters SpawnParams;

		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = Instigator;

		FVector SpawnLocation = loc + rot.RotateVector(FVector(70, 0, 0));
		GetWorld()->SpawnActor<ABullet>(bullet, SpawnLocation, rot, SpawnParams);
	}
}

void APlayerCharacter::SetTurnedRight(bool value)
{
	turnedRight = value;
}

bool APlayerCharacter::GetTurnedRight()
{
	return turnedRight;
}

float APlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Call the base class - this will tell us how much damage to apply  
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);
	if (ActualDamage > 0.f)
	{
		ChangeCurrentHealth(-ActualDamage);

		// If the damage depletes our health set our lifespan to zero - which will destroy the actor  
		if (currentHealth <= 0.f)
		{
			UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetViewTargetWithBlend(this, 2.0f, VTBlend_Linear, 0.0f, false);
			ClearComponentOverlaps();
			SetLifeSpan(0.001f);
		}
	}

	return ActualDamage;
}
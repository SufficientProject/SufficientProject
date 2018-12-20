// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy.h"
#include "Bullet.h"
#include "PaperFlipbookComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Controller.h"

// Define collision
#define COLLISION_ENEMY ECollisionChannel::ECC_GameTraceChannel1
#define COLLISION_PLAYER ECollisionChannel::ECC_GameTraceChannel2

AEnemy::AEnemy()
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

	// Set default values for player character
	currentHealth = 10;
	maxHealth = 10;

	turnedRight = true;
	inCombat = false;

	BackgroundMusicAudioComp = CreateDefaultSubobject<UAudioComponent>(TEXT("BackgroundMusicComponent"));

	if (BackgroundMusicAudioComp)
	{
		BackgroundMusicAudioComp->bAutoActivate = false;
	}
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (inCombat)
	{
		if ( !(BackgroundMusicAudioComp->IsPlaying()) )
		{
			BackgroundMusicAudioComp->Play(0.0f);
		}
	}

	UpdateCharacter();
	//Fire();
}

void AEnemy::UpdateAnimation()
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

void AEnemy::MoveRight(float value)
{
	AddMovementInput(FVector(1.0f, 0.0f, 0.0f), value);
}

void AEnemy::UpdateCharacter()
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

	Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	
	if (Player)
	{
		if (Player->GetActorLocation().X < this->GetActorLocation().X)
		{
			SetActorRotation(FRotator(0.0f, 180.0f, 0.0f));
			SetTurnedRight(false);
		}
		else if (Player->GetActorLocation().X > this->GetActorLocation().X)
		{
			SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
			SetTurnedRight(true);
		}
	}
}

float AEnemy::GetCurrentHealth()
{
	return currentHealth;
}

void AEnemy::SetCurrentHealth(float health)
{
	if (health < 0)
		currentHealth = 0;
	else if (health > GetMaxHealth())
		currentHealth = GetMaxHealth();
	else
		currentHealth = health;

}

void AEnemy::ChangeCurrentHealth(float value)
{
	SetCurrentHealth(GetCurrentHealth() + value);
	UE_LOG(LogTemp, Warning, TEXT("%f"), GetCurrentHealth());
}

float AEnemy::GetMaxHealth()
{
	return maxHealth;
}

void AEnemy::SetMaxHealth(float health)
{
	if (health > 0)
		maxHealth = health;
}

void AEnemy::Fire()
{
	if (bullet)
	{
		UGameplayStatics::PlaySoundAtLocation(this, AttackNormal, GetActorLocation());

		Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (Player)
		{
			FVector loc = GetCapsuleComponent()->RelativeLocation;
			FRotator rot = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), Player->GetActorLocation());

			FActorSpawnParameters SpawnParams;

			SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			FVector SpawnLocation = loc + rot.RotateVector(FVector(70, 0, 0));
			GetWorld()->SpawnActor<ABullet>(bullet, SpawnLocation, rot, SpawnParams);
		}
	}
}

void AEnemy::FireSpecial()
{
	if (bullet)
	{
		Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));

		if (Player)
		{
			int randmax = 1000000;
			int random = FMath::RandRange(0, randmax);

			if (GetCurrentHealth() >= 70)
			{
				if (random > randmax * 0.9)
					specialAttack = true;
			}
			else if (GetCurrentHealth() >= 40)
			{
				if (random > randmax * 0.75)
					specialAttack = true;
			}
			else if (GetCurrentHealth() > 0)
				if (random > randmax * 0.5)
					specialAttack = true;

			if (specialAttack)
			{
				int randomVal = FMath::RandRange(0, 1);

				if (randomVal == 0)
				{
					UGameplayStatics::PlaySound2D(this, AttackSpecial1);
				}
				else if (randomVal == 1)
				{
					UGameplayStatics::PlaySound2D(this, AttackSpecial2);
				}
				else
				{
					UGameplayStatics::PlaySound2D(this, AttackSpecial3);
				}

				FVector loc = GetCapsuleComponent()->RelativeLocation;
				FRotator rot = UKismetMathLibrary::FindLookAtRotation(this->GetActorLocation(), Player->GetActorLocation());
				
				FActorSpawnParameters SpawnParams;

				SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;

				FVector SpawnLocation = loc + rot.RotateVector(FVector(70, 0, 0));
				GetWorld()->SpawnActor<ABullet>(bullet, SpawnLocation, rot, SpawnParams);

				specialAttack = false;
			}
		}
	}
}


void AEnemy::SetTurnedRight(bool value)
{
	turnedRight = value;
}

bool AEnemy::GetTurnedRight()
{
	return turnedRight;
}

float AEnemy::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	// Call the base class - this will tell us how much damage to apply  
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage > 0.f)
	{
		if (TauntDamageTaken)
		{
			UGameplayStatics::PlaySound2D(this, TauntDamageTaken);
		}
		
		ChangeCurrentHealth(-ActualDamage);

		if (currentHealth <= 0.f)
		{
			BackgroundMusicAudioComp->Stop();

			SetLifeSpan(0.001f);
		}

		int randmax = 1000000;
		int random = FMath::RandRange(0, randmax);

		if (GetCurrentHealth() >= 70)
		{
			StartMedium();
		}
		else if (GetCurrentHealth() >= 40)
		{
			StartFast();
		}			
		// If the damage depletes our health set our lifespan to zero - which will destroy the actor  
	}

	return ActualDamage;
}

void AEnemy::StartSlow()
{

	const float QuarterNoteTime = 60.0f / FMath::Max(1.0f, bpm);
	const float QuarterNoteTime2 = 60.0f / FMath::Max(1.0f, bpmSpecial);

	GetWorld()->GetTimerManager().SetTimer(timer, this, &AEnemy::Fire, QuarterNoteTime, true);
	GetWorld()->GetTimerManager().SetTimer(timerHandle2, this, &AEnemy::FireSpecial, QuarterNoteTime2, true);

	if (!inCombat)
	{
		inCombat = true;

		if (TauntBegin)
		{
			UGameplayStatics::PlaySound2D(this, TauntBegin);

			if (MusicSlow)
			{
				BackgroundMusicAudioComp->SetSound(MusicSlow);

				BackgroundMusicAudioComp->bIgnoreForFlushing = 1;
			}
		}
		return;
	}

	if (!(BackgroundMusicAudioComp->IsPlaying()))
	{
		BackgroundMusicAudioComp->Stop();
	}

	if (MusicSlow)
	{
		BackgroundMusicAudioComp->SetSound(MusicSlow);

		BackgroundMusicAudioComp->bIgnoreForFlushing = 1;

		BackgroundMusicAudioComp->Play();
	}
	
}

void AEnemy::StartMedium()
{
	inCombat = true;

	int randomVal = FMath::RandRange(0, 1);
	
	if (randomVal == 1)
	{
		if (TauntCocky1)
		{
			UGameplayStatics::PlaySound2D(this, TauntCocky1);
		}	
	}
	else
	{
		if (TauntCocky2)
		{
			UGameplayStatics::PlaySound2D(this, TauntCocky2);
		}	
	}

	if (!(BackgroundMusicAudioComp->IsPlaying()))
	{
		BackgroundMusicAudioComp->Stop();
	}

	if (MusicMedium)
	{
		BackgroundMusicAudioComp->SetSound(MusicMedium);

		BackgroundMusicAudioComp->bIgnoreForFlushing = 1;

		BackgroundMusicAudioComp->Play();
	}

	const float QuarterNoteTime = 60.0f / FMath::Max(1.0f, bpm);
	const float QuarterNoteTime2 = 60.0f / FMath::Max(1.0f, bpmSpecial);

	GetWorld()->GetTimerManager().SetTimer(timer, this, &AEnemy::Fire, QuarterNoteTime, true);
	GetWorld()->GetTimerManager().SetTimer(timerHandle2, this, &AEnemy::FireSpecial, QuarterNoteTime2, true);
}

void AEnemy::StartFast()
{

	inCombat = true;

	int randomVal = FMath::RandRange(0, 1);

	if (randomVal == 1)
	{
		if (TauntAngry1)
		{
			UGameplayStatics::PlaySound2D(this, TauntAngry1);
		}
		
	}
	else
	{
		if (TauntAngry2)
		{
			UGameplayStatics::PlaySound2D(this, TauntAngry2);
		}
		
	}

	if (!(BackgroundMusicAudioComp->IsPlaying()))
	{
		BackgroundMusicAudioComp->Stop();
	}

	if (MusicFast)
	{
		BackgroundMusicAudioComp->SetSound(MusicFast);

		BackgroundMusicAudioComp->bIgnoreForFlushing = 1;

		BackgroundMusicAudioComp->Play();
	}

	const float QuarterNoteTime = 60.0f / FMath::Max(1.0f, bpm);
	const float QuarterNoteTime2 = 60.0f / FMath::Max(1.0f, bpmSpecial);

	GetWorld()->GetTimerManager().SetTimer(timer, this, &AEnemy::Fire, QuarterNoteTime, true);
	GetWorld()->GetTimerManager().SetTimer(timerHandle2, this, &AEnemy::FireSpecial, QuarterNoteTime2, true);
}


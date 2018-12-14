// Fill out your copyright notice in the Description page of Project Settings.

#include "CombatCameraTrigger.h"
#include "PlayerCharacter.h"
#include "SufficientProject.h"
#include "Engine.h"
#include "Camera/CameraComponent.h"


// Sets default values
ACombatCameraTrigger::ACombatCameraTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->InitBoxExtent(FVector(366.25f, 198.75f, 201.25f));
	CollisionBox->SetCollisionProfileName("Trigger");
	RootComponent = CollisionBox;

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
	//CameraComponent->FieldOfView = 90.f;
	CameraComponent->OrthoWidth = 2048.0f;
	CameraComponent->SetRelativeLocation(FVector(-50.0f, 1024.0f, -10.0f));
	CameraComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	CameraComponent->SetupAttachment(RootComponent);

	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ACombatCameraTrigger::BeginOverlap);
	CollisionBox->OnComponentEndOverlap.AddDynamic(this, &ACombatCameraTrigger::EndOverlap);
}

// Called when the game starts or when spawned
void ACombatCameraTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	DrawDebugBox(GetWorld(), GetActorLocation(), CollisionBox->GetScaledBoxExtent(), FQuat(GetActorRotation()), FColor::Turquoise, true, -1, 0, 2);
}

// Called every frame
void ACombatCameraTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ACombatCameraTrigger::BeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherActor != NULL && OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		APlayerController* Controller = Cast<APlayerController>(Player->GetController());

		Controller->SetViewTargetWithBlend(this, 2.0f, VTBlend_Linear, 0.0f, false);
	}
}

void ACombatCameraTrigger::EndOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor != NULL && OtherActor->IsA(APlayerCharacter::StaticClass()))
	{
		APlayerCharacter* Player = Cast<APlayerCharacter>(OtherActor);
		APlayerController* Controller = Cast<APlayerController>(Player->GetController());

		Controller->SetViewTargetWithBlend(Player, 2.0f, VTBlend_Linear, 0.0f, false);
	}
}
// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "PaperCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class SUFFICIENTPROJECT_API APlayerCharacter : public APaperCharacter
{
	GENERATED_BODY()
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// The animation to play while in running state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		class UPaperFlipbook* RunningAnimation;

	// The animation to play while idle (standing still)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		class UPaperFlipbook* IdleAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		class UPaperFlipbook* CombatAnimation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bullet)
		TSubclassOf<AActor> bullet;

	/* Called to choose correct animation to play based on movement state */
	void UpdateAnimation();

	/* Called for movement */
	void MoveRight(float value);

	void UpdateCharacter();

	void Fire();

	void Suicide();

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

public:
	// Sets default values for this character's properties
	APlayerCharacter();

	/* Returns CameraComponent subobject */
	FORCEINLINE class UCameraComponent* GetCameraComponent() const { return CameraComponent; }

	/* Returns CameraBoom subobject */
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float currentHealth;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float maxHealth;

public:
	UFUNCTION()
		float GetCurrentHealth();

	UFUNCTION()
		void SetCurrentHealth(float health);

	UFUNCTION()
		void ChangeCurrentHealth(float value);

	UFUNCTION()
		float GetMaxHealth();

	UFUNCTION()
		void SetMaxHealth(float health);

private:
	bool turnedRight;

public:
	void SetTurnedRight(bool value);
	bool GetTurnedRight();

protected:
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};

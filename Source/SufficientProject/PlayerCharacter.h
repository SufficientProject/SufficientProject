// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "PaperCharacter.h"
#include "SoundCombo.h"
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
	// The animation to play while in jumping state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
		class UPaperFlipbook* JumpingAnimation;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bullet)
		TSubclassOf<AActor> bulletLow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bullet)
		TSubclassOf<AActor> bulletMed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bullet)
		TSubclassOf<AActor> bulletHigh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Bullet)
		TSubclassOf<AActor> bulletHighest;

	/* Called to choose correct animation to play based on movement state */
	void UpdateAnimation();

	/* Called for movement */
	void MoveRight(float value);

	void UpdateCharacter();

	void FireLow();
	void FireMed();
	void FireHigh();

	void FireHighest();
	void FireDefault();

	void Fire(TSubclassOf<AActor> b);

	void Squeak();

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		float currentStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		float maxStamina;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		float staminaRegenerationValue;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stamina)
		float staminaRegenerationRate;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* Squeaking;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* DamageTaken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* Dying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* ShotDefault;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* ShotLow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* ShotMed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* ShotHigh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* ShotHighest;

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

	UFUNCTION()
		float GetCurrentStamina();

	UFUNCTION()
		void SetCurrentStamina(float stamina);

	UFUNCTION()
		void ChangeCurrentStamina(float value);

	UFUNCTION()
		float GetMaxStamina();

	UFUNCTION()
		void SetMaxStamina(float stamina);

	UFUNCTION()
		void SetStaminaRegenerationValue(float stamina);

	UFUNCTION()
		float GetStaminaRegenerationValue();

	UFUNCTION()
		void SetStaminaRegenerationRate(float stamina);

	UFUNCTION()
		float GetStaminaRegenerationRate();

private:

	sound_shot genericCombos[5][5] =
	{
		{sound_shot::LOW, sound_shot::LOW, sound_shot::LOW, sound_shot::HIGHEST, sound_shot::HIGH},

		{sound_shot::LOW, sound_shot::HIGH, sound_shot::LOW, sound_shot::HIGH, sound_shot::HIGHEST},

		{sound_shot::MED, sound_shot::MED, sound_shot::HIGH, sound_shot::HIGHEST, sound_shot::LOW},

		{sound_shot::LOW, sound_shot::MED, sound_shot::MED, sound_shot::HIGH, sound_shot::HIGHEST},

		{sound_shot::HIGHEST, sound_shot::LOW, sound_shot::LOW, sound_shot::MED, sound_shot::HIGH}

	};

	size_t shotsFired, comboList;

	SoundCombo *comboList;
	bool *possibleCombos;

	FTimerHandle staminaTimer;

	float lastStaminaShot;

	bool staminaReplenishing;
	bool turnedRight;

	void DestroyOverlapped();

public:
	void SetTurnedRight(bool value);
	bool GetTurnedRight();
	bool IsJumping();

	bool CheckStamina();

	void StartReplenishingStamina();
	void StoptReplenishingStamina();

	void ReplenishStaminaPortion();

	void checkCombo(sound_shot);
	void usePowerUp(sound_combo_effect effect);
	void resetComboCheck();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
		void Death();

protected:
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};

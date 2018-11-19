// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "PlayerCharacter.h"
#include "PaperCharacter.h"
#include "Enemy.generated.h"

/**
 * 
 */
UCLASS()
class SUFFICIENTPROJECT_API AEnemy : public APaperCharacter
{
	GENERATED_BODY()
	
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

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		int currentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		int maxHealth;

public:
	UFUNCTION()
		int GetCurrentHealth();

	UFUNCTION()
		void SetCurrentHealth(int health);

	UFUNCTION()
		void ChangeCurrentHealth(int value);

	UFUNCTION()
		int GetMaxHealth();

	UFUNCTION()
		void SetMaxHealth(int health);

private:
	bool turnedRight;
	APlayerCharacter* Player;
	bool test = false;

	FTimerHandle timer;

public:
	void SetTurnedRight(bool value);
	bool GetTurnedRight();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float bpm = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float bpmSpecial = 0;

	void StartCombat();

protected:
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};

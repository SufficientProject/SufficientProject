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
	void FireSpecial();

public:
	// Sets default values for this character's properties
	AEnemy();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float currentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Health)
		float maxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* TauntBegin;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* TauntCocky1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* TauntCocky2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* TauntAngry1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* TauntAngry2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* TauntDying;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* TauntDamageTaken;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* AttackNormal;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* AttackSpecial1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* AttackSpecial2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* AttackSpecial3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* MusicSlow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* MusicMedium;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		class USoundBase* MusicFast;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Sound)
		UAudioComponent* BackgroundMusicAudioComp;

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
	APlayerCharacter* Player;
	
	bool test = false;
	bool specialAttack = false;
	bool inCombat = false;

	FTimerHandle timer;
	FTimerHandle timerHandle2;

public:
	void SetTurnedRight(bool value);
	bool GetTurnedRight();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float bpm = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float bpmSpecial = 0;

		void StartSlow();
		void StartMedium();
		void StartFast();

protected:
	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;
};

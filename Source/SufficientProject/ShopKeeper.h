// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "PlayerCharacter.h"
#include "PaperCharacter.h"
#include "ShopKeeper.generated.h"

/**
 * 
 */
UCLASS()
class SUFFICIENTPROJECT_API AShopKeeper : public APaperCharacter
{
	GENERATED_BODY()

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UPaperFlipbook* IdleAnimation;

	/* Called to choose correct animation to play based on movement state */
	void UpdateAnimation();
	
public:
	// Sets default values for this character's properties
	AShopKeeper();
};

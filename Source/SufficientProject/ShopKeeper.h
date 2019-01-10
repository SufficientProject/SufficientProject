// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperCharacter.h"
#include "ShopKeeper.generated.h"

/**
 * 
 */
UCLASS()
class SUFFICIENTPROJECT_API AShopKeeper : public APaperCharacter
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	class UPaperFlipbook* IdleAnimation;
	
};

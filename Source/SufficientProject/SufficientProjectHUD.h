// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "SufficientProjectHUD.generated.h"

/**
 * 
 */
UCLASS()
class SUFFICIENTPROJECT_API ASufficientProjectHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	ASufficientProjectHUD();

	virtual void BeginPlay() override;

private:

	UPROPERTY(EditAnywhere, Category = "Health")
		TSubclassOf<class UUserWidget> HUDWidgetClass;

	UPROPERTY(EditAnywhere, Category = "Health")
		class UUserWidget* CurrentWidget;
};

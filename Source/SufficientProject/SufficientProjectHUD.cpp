// Fill out your copyright notice in the Description page of Project Settings.

#include "SufficientProjectHUD.h"
#include "UObject/ConstructorHelpers.h"
#include "Blueprint/UserWidget.h"

ASufficientProjectHUD::ASufficientProjectHUD() {

	//Find Health UI Widget
	static ConstructorHelpers::FClassFinder<UUserWidget> HealthBarOBJ(TEXT("/Game/Blueprints/PlayerHud"));

	//Create and set health ui to WidgetClass
	HUDWidgetClass = HealthBarOBJ.Class;
}

void ASufficientProjectHUD::BeginPlay() {

	Super::BeginPlay();

	//Set current widget and add to viewport
	if (HUDWidgetClass != nullptr) {
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);

		if (CurrentWidget)
		{
			CurrentWidget->AddToViewport();
		}
	}
}



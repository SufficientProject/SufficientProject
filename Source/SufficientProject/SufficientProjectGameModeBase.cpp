// Fill out your copyright notice in the Description page of Project Settings.

#include "SufficientProjectGameModeBase.h"
#include "UObject/ConstructorHelpers.h"
#include "SufficientProjectHUD.h"
#include "UObject/ConstructorHelpers.h"

ASufficientProjectGameModeBase::ASufficientProjectGameModeBase() : Super() {
	
	//Use custom hud class
	HUDClass = ASufficientProjectHUD::StaticClass();
}
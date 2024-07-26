// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMouseController.h"

void AInGameMouseController::BeginPlay()
{
	bEnableClickEvents = true;
	bShowMouseCursor = true;
}

void AInGameMouseController::SetupInputComponent()
{
	InputComponent->BindAction("LeftClick", IE_Pressed, this, &AInGameMouseController::OnLeftMouseClick);
	InputComponent->BindAction("RightClick", IE_Pressed, this, &AInGameMouseController::OnRightMouseClick);
}

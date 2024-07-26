// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InGameMouseController.generated.h"

/**
 * 
 */
UCLASS()
class PIPES_API AInGameMouseController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	// setup custom click bindings
	virtual void SetupInputComponent() override;
	
	// handle left and right mouse clicks
	UFUNCTION()
	void OnLeftMouseClick();
	void OnRightMouseClick();
};

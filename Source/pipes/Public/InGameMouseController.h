// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "InGameMouseController.generated.h"

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

/**
 * 
 */
UCLASS()
class PIPES_API AInGameMouseController : public APlayerController
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

	
public:
	// handle left and right mouse clicks
	UFUNCTION()
	void OnLeftMouseClick(const FInputActionValue& Value);
	//void OnRightMouseClick();

};

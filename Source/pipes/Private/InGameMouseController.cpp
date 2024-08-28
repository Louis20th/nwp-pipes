// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMouseController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "InputMappingContext.h" 
#include "GameBoardActor.h"
#include "Engine/AssetManager.h"

AInGameMouseController::AInGameMouseController()
{
	mIMContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/InputActions/InputContext.InputContext"));
	mIAMouseClick = LoadObject<UInputAction>(nullptr, TEXT("/Game/InputActions/IA_LeftClick.IA_LeftClick"));

	if (mIMContext && mIAMouseClick) {
		UE_LOG(LogTemp, Error, TEXT("Missing enhanced input mappings"));
	}
}

void AInGameMouseController::BeginPlay()
{
	Super::BeginPlay();
	AutoReceiveInput = EAutoReceiveInput::Player0;

	bEnableClickEvents = true;
	bShowMouseCursor = true;

	if (mIMContext) {
		// Get the Enhanced Input subsystem and add the mapping context
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()) {
			subsystem->AddMappingContext(mIMContext, 0);
		}
	}
	auto inputMode = FInputModeGameAndUI();
	inputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	inputMode.SetHideCursorDuringCapture(false);
	SetInputMode(inputMode);
}

void AInGameMouseController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* enhancedInputComponent = Cast<UEnhancedInputComponent>(InputComponent)) {
		enhancedInputComponent->BindAction(mIAMouseClick, ETriggerEvent::Completed, this, &AInGameMouseController::onClick);
	}
}

void AInGameMouseController::onClick(const FInputActionValue& Value)
{
	FHitResult hitResult;
	GetHitResultUnderCursorByChannel(UEngineTypes::ConvertToTraceType(ECC_Visibility), true, hitResult);

	if (hitResult.GetActor()) {
		if (AGameBoardActor* tileMapActor = Cast<AGameBoardActor>(hitResult.GetActor())) {
			tileMapActor->onTileClick(hitResult.Location);
		}
	}
}

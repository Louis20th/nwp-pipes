// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLevelMode.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Blueprint/UserWidget.h"

#include <Kismet/GameplayStatics.h>
#include "Kismet/KismetSystemLibrary.h"

AGameLevelMode::AGameLevelMode()
	: mInitilized(false)
	, mCurrentState(GameState::gameStart)
{
	/// Suppress spawning default pawn
	DefaultPawnClass = nullptr;
}

bool AGameLevelMode::handleNewState() {
	bool status(false);
	UE_LOG(LogTemp, Error, TEXT("Handling game state"));
	switch (mCurrentState)
	{
	case GameState::mainMenu: {
		/// reset game session
		/// level generator generates buffer if necessary
		/// display mainMenu
		/// 
		UE_LOG(LogTemp, Error, TEXT("Case(GameState::mainMenu"));
		if (IsValid(mMainMenuWidgetClass))
		{
			UE_LOG(LogTemp, Error, TEXT("mMainMenuWidgetClass not nullptr"));
			mMainMenuWidget = CreateWidget<UMainMenuWidget>(GetWorld(), mMainMenuWidgetClass);
			if (mMainMenuWidget)
			{
				mMainMenuWidget->setOnStartClickedCallback([this]() {
					setGameState(GameState::inGame);
					});
				mMainMenuWidget->setOnQuitClickedCallback([this]() {
					UKismetSystemLibrary::QuitGame(GetWorld(), UGameplayStatics::GetPlayerController(GetWorld(), 0), EQuitPreference::Type::Quit, false);
					});

				UE_LOG(LogTemp, Error, TEXT("mMainMenuWidget not nullptr"));
				mMainMenuWidget->AddToViewport(1);

				APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
				if (PlayerController != nullptr)
				{
					UE_LOG(LogTemp, Error, TEXT("PlayerController not nullptr"));
					FInputModeUIOnly InputModeData;
					InputModeData.SetWidgetToFocus(mMainMenuWidget->TakeWidget());
					PlayerController->SetInputMode(InputModeData);
					PlayerController->bShowMouseCursor = true;
				}
			}
			else {
				UE_LOG(LogTemp, Error, TEXT("Main menu widget not created"));
			}
		}
		else {
			UE_LOG(LogTemp, Error, TEXT("mMainMenuWidgetClass is nullptr"));
		}

		status = true;
		break;
	}
	case GameState::inGame: {
		UE_LOG(LogTemp, Error, TEXT("case core::GameState::inGame:"));
		/// start session
		/// hide mainMenu
		/// spawn gameBoard
		/// set level

		if (mMainMenuWidget) {
			mMainMenuWidget->RemoveFromParent();
		}
		if (!mSpawnedBoard) {
			UE_LOG(LogTemp, Error, TEXT("Spawned board is nullptr"));
			return;
		}

		FVector Location(0.0f, 0.0f, 0.0f);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		auto boardActor = GetWorld()->SpawnActor<AGameBoardActor>(mSpawnedBoard->StaticClass(), Location, Rotation);
		if (!boardActor) {
			UE_LOG(LogTemp, Error, TEXT("Tilemap not loaded"));
		}

		boardActor->init(2, 2);
		boardActor->setDemoState();

		FVector CameraLocation(15.0f, 50.0f, -15.0f);
		FRotator CameraRotation(0.0f, 270.0f, 0.0f); // Y, Z, X for some reason
		auto camera = GetWorld()->SpawnActor<ACameraActor>(CameraLocation, CameraRotation);
		auto cameraComp = camera->GetCameraComponent();
		cameraComp->ProjectionMode = ECameraProjectionMode::Orthographic;
		cameraComp->SetOrthoWidth(250);

		auto controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
		if (controller) {
			if (camera->HasActiveCameraComponent()) {
				UE_LOG(LogTemp, Error, TEXT("BOARD HAS ACTIVE CAMERA COMP"));
				controller->SetViewTarget(camera);
			}
		}
		status = true;
		break;
	}
	case GameState::pauseMenu: {
		/// stop timers
		/// display pauseMenu
		status = true;
		break;
	}
	case GameState::scoreBoard: {
		/// hide mainMenu
		/// display scoreBoard
		status = true;
		break;
	}
	default:
		break;
	}
	return status;
}

void AGameLevelMode::setGameState(GameState const gameState) {
	UE_LOG(LogTemp, Error, TEXT("Setting game state"));
	mCurrentState = gameState;

	if (!handleNewState()) {
		/// log and handle error
		return;
	}
}

bool AGameLevelMode::initAllObjects() {
	if (mInitilized) {
		/// log this, it shoudn't happen
		return false;
	}

	mMainMenuWidgetClass = LoadClass<UMainMenuWidget>(this, TEXT("/Game/Menus/MainMenuWidgetBP.MainMenuWidgetBP_C"));
	mSpawnedBoard = NewObject<AGameBoardActor>(this);

	/// init level generator
	/// init score manager
	/// init game board (pop one layout from level generator)
	/// log and return after each step
	/// 
	mInitilized = true;
	return true;
}

void AGameLevelMode::BeginPlay()
{
	/*if (!initAllObjects() && !mInitilized) {
		/// log and exit
		exit(-1);
	}*/

	setGameState(GameState::mainMenu);

	//if (mInitilized) {
	//	FVector Location(0.0f, 0.0f, 0.0f);
	//	FRotator Rotation(0.0f, 0.0f, 0.0f);
	//	FActorSpawnParameters SpawnInfo;
	//	auto boardActor = GetWorld()->SpawnActor<AGameBoardActor>(mSpawnedBoard->StaticClass(), Location, Rotation);
	//	if (!boardActor) {
	//		UE_LOG(LogTemp, Error, TEXT("Tilemap not loaded"));
	//	}

	//	boardActor->init(tileSet, 2, 2);
	//	boardActor->setDemoState();

	//	FVector CameraLocation(15.0f, 50.0f, -15.0f);
	//	FRotator CameraRotation(0.0f, 270.0f, 0.0f); // Y, Z, X for some reason
	//	auto camera = GetWorld()->SpawnActor<ACameraActor>(CameraLocation, CameraRotation);
	//	auto cameraComp = camera->GetCameraComponent();
	//	cameraComp->ProjectionMode = ECameraProjectionMode::Orthographic;
	//	cameraComp->SetOrthoWidth(250);

	//	auto controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	//	if (controller) {
	//		if (camera->HasActiveCameraComponent()) {
	//			UE_LOG(LogTemp, Error, TEXT("BOARD HAS ACTIVE CAMERA COMP"));
	//			controller->SetViewTarget(camera);
	//		}
	//	}
	//	else {
	//		UE_LOG(LogTemp, Error, TEXT("PLAYER CONTROLLER NOT INITI"));
	//	}
	//}
	//else {
	//	UE_LOG(LogTemp, Error, TEXT("GameBoard not initialized"));
	//}
}

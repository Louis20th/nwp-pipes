// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameBoardActor.h"
#include <memory>

#include "GameLevelMode.generated.h"

/**
 * 
 */
UCLASS()
class PIPES_API AGameLevelMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AGameLevelMode();

	void BeginPlay() override;

private:
	AGameBoardActor* mGameBoard;
	bool mInitilized;
};

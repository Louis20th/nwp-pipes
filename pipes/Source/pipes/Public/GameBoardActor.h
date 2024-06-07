// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileMapActor.h"
#include "PaperTileSet.h"
#include <memory>

#include "GameBoardActor.generated.h"

/**
 * Board actor holds the TileMap. It's created in the GameInstance.
 * 
 */
UCLASS()
class PIPES_API AGameBoardActor : public APaperTileMapActor
{
	GENERATED_BODY()

public:
	AGameBoardActor();
	~AGameBoardActor() = default;
	
	bool init(UPaperTileSet* tileSet, int32 cols, int32 rows);
	void setDemoState();
private:
	UPROPERTY()
	UPaperTileMapComponent* mTileMapComponent;
	UPaperTileSet* mTileSet;
};

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileMapActor.h"
#include "PaperTileSet.h"
#include "PaperTileMap.h"
#include "BoardLayout.h"
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

protected:
	virtual void BeginPlay() override;

public:
	~AGameBoardActor() = default;

	void onTileClick(const FVector& ClickLocation);

	bool init(int32 cols, int32 rows);

	// temp funct to test click handling
	void setDemoState();

	void spawnBoard(BoardLayout const& layout);

private:
	UPaperTileSet* mTileSet;
	UPaperTileMap* mTileMap;
};

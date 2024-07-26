// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileMapActor.h"
#include "PaperTileSet.h"
#include "PaperTileMap.h"
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

	UFUNCTION()
	void onCLicked();

public:
	AGameBoardActor();
	~AGameBoardActor() = default;

	UFUNCTION()
	bool init(int32 cols, int32 rows);

	UFUNCTION()
	void setDemoState();

protected:

private:
	UPaperTileSet* mTileSet;
	UPaperTileMap* mTileMap; 
};

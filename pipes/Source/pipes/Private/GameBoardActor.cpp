// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoardActor.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "PaperTileLayer.h"

AGameBoardActor::AGameBoardActor()
	: mTileMapComponent(nullptr)
	, mTileSet(nullptr)
{
}

bool AGameBoardActor::init(UPaperTileSet* tileSet, int32 cols, int32 rows)
{
	if (tileSet == nullptr) {
		return false;
	}

	mTileSet = tileSet;

	mTileMapComponent = FindComponentByClass<UPaperTileMapComponent>();
	RootComponent = mTileMapComponent;
	if (mTileMapComponent == nullptr) {
		return false;
	}
	
	auto tileMap = NewObject<UPaperTileMap>(this);
	tileMap->InitializeNewEmptyTileMap(mTileSet);
	tileMap->ResizeMap(cols, rows);


	mTileMapComponent->SetTileMap(tileMap);
	return true;
}

void AGameBoardActor::setDemoState()
{
	auto tileMap = mTileMapComponent->TileMap;
	FPaperTileInfo tileInfo;
	tileInfo.TileSet = mTileSet;

	for (int32 i = 0; i < 4; ++i) {
		tileInfo.PackedTileIndex = i + 5;
		mTileMapComponent->SetTile(i, i + 1, 0, tileInfo);
	}
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoardActor.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "PaperTileLayer.h"
#include "Math/Color.h"

AGameBoardActor::AGameBoardActor()
{
}

bool AGameBoardActor::init(int32 cols, int32 rows)
{
	mTileSet = LoadObject<UPaperTileSet>(nullptr, TEXT("/Game/Tileset/pipeTilesSet.pipeTilesSet"));
	if (mTileSet == nullptr) {
		return false;
	}

	// ATileMapActor wraps tileMap as an render component
	auto tileMap = GetRenderComponent();

	tileMap->CreateNewTileMap(cols, rows);
	tileMap->SetTileMapColor(FLinearColor::White);

	return true;
}

void AGameBoardActor::setDemoState()
{
	FPaperTileInfo tileInfo;
	tileInfo.TileSet = mTileSet;
	int32 w, h, layers;
	auto tileMap = GetRenderComponent();

	tileMap->GetMapSize(w, h, layers);

	for (int32 i = 0; i < 2; ++i) {
		tileInfo.PackedTileIndex = i + 37;
		tileMap->SetTile(i, i, 0, tileInfo);
	}

}

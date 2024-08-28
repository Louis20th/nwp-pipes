// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoardActor.h"
#include "GameLevelMode.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "PaperTileLayer.h"
#include "InGameMouseController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "Math/Color.h"

void AGameBoardActor::BeginPlay()
{
	Super::BeginPlay();
}

void AGameBoardActor::onTileClick(const FVector& ClickLocation)
{
	UPaperTileMapComponent* tileMap = GetRenderComponent();
	if (!tileMap) {
		return;
	}

	// Convert the world click location to tile coordinates
	FVector clickLocation = tileMap->GetComponentTransform().InverseTransformPosition(ClickLocation);
	const float tileWidth = tileMap->TileMap->TileWidth;
	const float tileHeight = tileMap->TileMap->TileHeight;

	FIntPoint tileCoords;
	// GameBoard actor is placed on 0, 0, 0 but top left is -16,0,16 (half a tile??)
	auto offset = tileWidth / 2;
	tileCoords.X = FMath::FloorToInt((clickLocation.X + offset) / tileWidth); // it starts negative -16
	tileCoords.Y = FMath::FloorToInt((clickLocation.Z - offset) / tileHeight * -1); // it goes to negative

	FPaperTileInfo newTileInfo;
	newTileInfo.TileSet = mTileSet;

	auto tile = tileMap->GetTile(tileCoords.X, tileCoords.Y, 0);
	if (tile.HasFlag(EPaperTileFlags::FlipVertical)) {
		newTileInfo.PackedTileIndex = 89;
		newTileInfo.SetFlagValue(EPaperTileFlags::FlipHorizontal, true);
	}
	else {
		newTileInfo.PackedTileIndex = 64;
		newTileInfo.SetFlagValue(EPaperTileFlags::FlipVertical, true);
	}

	tileMap->SetTile(tileCoords.X, tileCoords.Y, 0, newTileInfo);

	// Update the tile map component
	tileMap->MarkRenderStateDirty();
}

bool AGameBoardActor::init(int32 const cols, int32 const rows)
{
	mTileSet = LoadObject<UPaperTileSet>(nullptr, TEXT("/Game/Tileset/pipeTilesSet.pipeTilesSet"));
	if (!mTileSet) {
		return false;
	}

	// ATileMapActor has tileMap as a render component
	auto tileMap = GetRenderComponent();
	tileMap->CreateNewTileMap(cols, rows);
	tileMap->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	tileMap->SetCollisionResponseToAllChannels(ECR_Ignore);
	tileMap->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

	return true;
}

void AGameBoardActor::setDemoState()
{
	FPaperTileInfo tileInfo;
	tileInfo.TileSet = mTileSet;
	int32 w, h, layers;
	auto tileMap = GetRenderComponent();

	tileMap->GetMapSize(w, h, layers);

	for (int i = 0; i < h; ++i) {
		for (int j = 0; j < w; ++j) {
			tileInfo.PackedTileIndex = 64;
			tileInfo.SetFlagValue(EPaperTileFlags::FlipVertical, true);
			tileMap->SetTile(i, j, 0, tileInfo);
		}
	}

	tileMap->RebuildCollision();
}

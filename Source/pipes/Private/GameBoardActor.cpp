// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoardActor.h"
#include "GameLevelMode.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "PaperTileLayer.h"

#include "Math/Color.h"
#include <Kismet/GameplayStatics.h>

void AGameBoardActor::BeginPlay()
{
	Super::OnClicked.AddDynamic(this, &AGameBoardActor::onCLicked);
}

void AGameBoardActor::onCLicked()
{
	auto controller = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	float x, y;
	controller->GetMousePosition(x, y);
	GetRenderComponent()->TileMap->GetTileCoordinatesFromLocalSpacePosition() GetTile(x, y, 0);

}

AGameBoardActor::AGameBoardActor()
{
}

bool AGameBoardActor::init(int32 const cols, int32 const rows)
{
	mTileSet = LoadObject<UPaperTileSet>(nullptr, TEXT("/Game/Tileset/pipeTilesSet.pipeTilesSet"));
	if (!mTileSet) {
		return false;
	}

	// ATileMapActor wraps tileMap as an render component
	auto tileMap = GetRenderComponent();

	tileMap->CreateNewTileMap(cols, rows);
	tileMap->SetLayerColor(FLinearColor::White, 0);

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
			tileInfo.PackedTileIndex = 37;
			tileMap->SetTile(i,j, 0, tileInfo);
		}
	}
}

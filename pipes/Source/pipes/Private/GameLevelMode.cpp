// Fill out your copyright notice in the Description page of Project Settings.


#include "GameLevelMode.h"

AGameLevelMode::AGameLevelMode()
	: mInitilized(false)
{
	if (!mGameBoard) {
		mGameBoard = NewObject<AGameBoardActor>();
	}

	static ConstructorHelpers::FObjectFinder<UPaperTileSet> TileSetAsset(TEXT("/pipes/Content/Tileset/pipeTilesSet.uasset"));
	if (TileSetAsset.Succeeded()) {
		mInitilized = mGameBoard->init(TileSetAsset.Object, 2, 2);

	}
}

void AGameLevelMode::BeginPlay()
{
	if (mInitilized) {
		FVector Location(0.0f, 0.0f, 0.0f);
		FRotator Rotation(0.0f, 0.0f, 0.0f);
		FActorSpawnParameters SpawnInfo;
		auto tileMapActor = GetWorld()->SpawnActor<AGameBoardActor>(mGameBoard->StaticClass(), Location, Rotation, SpawnInfo);
		tileMapActor->setDemoState();
	}

}

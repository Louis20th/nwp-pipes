// Fill out your copyright notice in the Description page of Project Settings.


#include "GameBoardActor.h"
#include "GameLevelMode.h"
#include "PaperTileMapComponent.h"
#include "PaperTileMap.h"
#include "PaperTileLayer.h"
#include "InGameMouseController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Components//InputComponent.h"


#include "Math/Color.h"
#include <Kismet/GameplayStatics.h>

void AGameBoardActor::BeginPlay()
{
	Super::BeginPlay();
	// OnClicked.AddDynamic(this, &AGameBoardActor::onCLicked);
	auto tileMapComp = GetRenderComponent();
	tileMapComp->SetCollisionEnabled(ECollisionEnabled::QueryAndProbe);
	tileMapComp->SetCollisionResponseToAllChannels(ECR_Block);
	tileMapComp->SetCollisionObjectType(ECC_WorldStatic);
}

void AGameBoardActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	APawn::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* inputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent) {
		inputComponent->BindAction(IA_LeftClick, ETriggerEvent::Triggered, this, )
	}
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

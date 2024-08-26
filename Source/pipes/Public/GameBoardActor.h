// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileMapActor.h"
#include "PaperTileSet.h"
#include "PaperTileMap.h"
#include <memory>

#include "GameBoardActor.generated.h"

class UInputMappingContext;
class UInputAction;

/**
* Board actor holds the TileMap. It's created in the GameInstance.
*
*/
UCLASS()
class PIPES_API AGameBoardActor : public APaperTileMapActor, public APawn
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputMappingContext* InputContext;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	UInputAction* IA_LeftClick;

	void

public:
	~AGameBoardActor() = default;

	UFUNCTION()
	bool init(int32 cols, int32 rows);

	UFUNCTION()
	void setDemoState();

private:
	UPaperTileSet* mTileSet;
	UPaperTileMap* mTileMap; 
};

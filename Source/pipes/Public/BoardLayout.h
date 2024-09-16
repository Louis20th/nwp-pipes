// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileLayer.h"

// utils
struct TileData;
using TileLayout = std::vector <TileData>;
using TilePosition = std::pair<size_t, size_t>;
TileLayout& operator<<(TileLayout& layout, TileData& tileData);
bool operator==(TilePosition const& lhs, TilePosition const& rhs);

enum TileType
{
	Start,
	Stop,
	Frame,
	Field
};

struct TileData {
	FPaperTileInfo mTileInfo;
	TilePosition mPosition;
	TileType mType;
	uint32 mWeight;
	uint32 f, g, h;
};

/**
 * This class handles layout generation.
 * Layout is generated using "landscape" creation which is then used for path creation using A* algo.
 * Finally, path tiles types are designated and randomly oriented, and the rest of the tiles are randomized.
 */

class PIPES_API BoardLayout
{
public:
	BoardLayout();
	~BoardLayout() = default;
	TileLayout createLayout(size_t const rows, size_t const cols);

private:
	uint32 manhattanScore(TilePosition const& curr, TilePosition const& end);
	std::vector<TileData*> getNeighbours(TileData& current);
	void generateLandscape();
	void generatePath(TilePosition const& start, TilePosition const& end);


	TileLayout mLayout;
	size_t mMaxRows;
	size_t mMaxCols;
};


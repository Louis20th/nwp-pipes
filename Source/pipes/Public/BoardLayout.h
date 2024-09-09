// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PaperTileLayer.h"

struct TileData {
	FPaperTileInfo mTileInfo;
	double mWeight;
	double mPotential;
	// use additional info if necesary
};

std::vector<TileData>& operator<<(std::vector<TileData>& layout, TileData& tileData);

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

	std::vector<TileData> const& createLayout(size_t const rows, size_t const cols);

private:
	void generateLandscape();



	std::vector<TileData> mLayout;

};


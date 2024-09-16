// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <functional>
#include <set>

#include "BoardLayout.h"
#include "CoreMinimal.h"

enum ChaseStatus {
	Chasing,
	Won,
	Lost
};

/**
 *
 */
class PIPES_API Chaser
{
public:
	Chaser();
	~Chaser() = default;

	void init(TileLayout* layout);
	void StartChasing();
	ChaseStatus nextMove();
	TilePosition getCurrPosition() const;

private:
	TileData& getNext();
	bool isNextConnected();

	TileLayout* mLayout;
	std::set<TilePosition> mVisited;
	TileData mCurrent;
	TileData mNext;
	TilePosition mPrev;
	size_t mMaxWidth;
};

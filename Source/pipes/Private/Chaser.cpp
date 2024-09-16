// Fill out your copyright notice in the Description page of Project Settings.

#include <cmath>
#include "Chaser.h"

Chaser::Chaser()
	: mLayout(nullptr)
	, mVisited()
	, mCurrent()
	, mNext()
	, mPrev()
	, mMaxWidth(0U)
{
}

void Chaser::init(TileLayout* layout)
{
	mLayout = layout;
}

void Chaser::StartChasing()
{
	TileData start, stop;

	for (auto& tile : *mLayout) {
		if (tile.mType == TileType::Start) {
			mCurrent = tile;
		}
		if (tile.mType == TileType::Stop) {
			mMaxWidth = tile.mPosition.first + 1U;
		}
	}
}

ChaseStatus Chaser::nextMove()
{
	mVisited.insert(mCurrent.mPosition);
	mNext = getNext();
	if (isNextConnected()) {
		if (mNext.mType == TileType::Stop) {
			return ChaseStatus::Won;
		}
		mPrev = mCurrent.mPosition;
		mCurrent = mNext;
		return ChaseStatus::Chasing;
	}
	return ChaseStatus::Lost;
}

TilePosition Chaser::getCurrPosition() const
{
	return mCurrent.mPosition;
}


TileData& Chaser::getNext()
{
	if (mCurrent.mType == TileType::Start) {
		// on start just move to the one on the right
		return mLayout->at(mCurrent.mPosition.second * mMaxWidth + (mCurrent.mPosition.first + 1U));

	}
	else {
		switch (mCurrent.mTileInfo.PackedTileIndex)
		{
		case 62: {
			// this one can go down and right
			auto& r = mLayout->at(mCurrent.mPosition.second * mMaxWidth + (mCurrent.mPosition.first + 1U));
			auto& d = mLayout->at((mCurrent.mPosition.second + 1U) * mMaxWidth + mCurrent.mPosition.first);
			return mVisited.contains(r.mPosition) ? d : r;
		}
		case 63: {
			// this one can go left and down
			auto& l = mLayout->at(mCurrent.mPosition.second * mMaxWidth + (mCurrent.mPosition.first - 1U));
			auto& d = mLayout->at((mCurrent.mPosition.second + 1U) * mMaxWidth + mCurrent.mPosition.first);
			return mVisited.contains(l.mPosition) ? d : l;
		}
		case 64: {
			// this one can go left and right
			auto& l = mLayout->at(mCurrent.mPosition.second * mMaxWidth + (mCurrent.mPosition.first - 1U));
			auto& r = mLayout->at(mCurrent.mPosition.second * mMaxWidth + (mCurrent.mPosition.first + 1U));
			return mVisited.contains(l.mPosition) ? r : l;
		}
		case 87: {
			// this one can go up and right
			auto& u = mLayout->at((mCurrent.mPosition.second - 1U) * mMaxWidth + mCurrent.mPosition.first);
			auto& r = mLayout->at(mCurrent.mPosition.second * mMaxWidth + (mCurrent.mPosition.first + 1U));
			return mVisited.contains(u.mPosition) ? r : u;
		}
		case 88: {
			// this one can go left and up
			auto& u = mLayout->at((mCurrent.mPosition.second - 1U) * mMaxWidth + mCurrent.mPosition.first);
			auto& l = mLayout->at(mCurrent.mPosition.second * mMaxWidth + (mCurrent.mPosition.first - 1U));
			return mVisited.contains(u.mPosition) ? l : u;
		}
		case 89: {
			// this one can go down and up
			auto& u = mLayout->at((mCurrent.mPosition.second - 1U) * mMaxWidth + mCurrent.mPosition.first);
			auto& d = mLayout->at((mCurrent.mPosition.second + 1U) * mMaxWidth + mCurrent.mPosition.first);
			return mVisited.contains(u.mPosition) ? d : u;
		}
		}
		return mCurrent;
	}
}

bool Chaser::isNextConnected()
{
	std::set<int> possible({ 0 });
	if (mCurrent.mType != TileType::Start)
	{
		switch (mCurrent.mTileInfo.PackedTileIndex)
		{
		case 62:
			possible = { 63, 64, 87, 88, 89, 364 };
			break;
		case 63:
			possible = { 62, 64, 87, 88, 89 };
			break;
		case 64:
			possible = { 62, 63, 64, 87, 88, 364 };
			break;
		case 87:
			possible = { 62, 63, 64, 88, 89, 364 };
			break;
		case 88:
			possible = { 62, 63, 64, 87, 89 };
			break;
		case 89:
			possible = { 62, 63, 87, 88, 89 };
			break;
		}
	}
	else {
		possible = { 63, 64, 88 };
	}

	return possible.contains(mNext.mTileInfo.PackedTileIndex);
}

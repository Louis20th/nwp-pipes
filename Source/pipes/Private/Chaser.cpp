// Fill out your copyright notice in the Description page of Project Settings.

#include "Chaser.h"

#include <cmath>

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
            mPrev = tile.mPosition;
        }
        if (tile.mType == TileType::Stop) {
            mMaxWidth = tile.mPosition.first + 1U;
        }
    }
}

ChaseStatus Chaser::nextMove()
{
    mNext = getNext();
    if (isNextConnected()) {
        if (mNext.mType == TileType::Stop) {
            mVisited.clear();
            return ChaseStatus::Won;
        }
        mVisited.insert(mCurrent.mPosition);
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

size_t Chaser::getPathLenght() const
{
    return mVisited.size();
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
            return r.mPosition == mPrev ? d : r;
        }
        case 63: {
            // this one can go left and down
            auto& l = mLayout->at(mCurrent.mPosition.second * mMaxWidth + (mCurrent.mPosition.first - 1U));
            auto& d = mLayout->at((mCurrent.mPosition.second + 1U) * mMaxWidth + mCurrent.mPosition.first);
            return l.mPosition == mPrev ? d : l;
        }
        case 64: {
            // this one can go left and right
            auto& l = mLayout->at(mCurrent.mPosition.second * mMaxWidth + (mCurrent.mPosition.first - 1U));
            auto& r = mLayout->at(mCurrent.mPosition.second * mMaxWidth + (mCurrent.mPosition.first + 1U));
            return l.mPosition == mPrev ? r : l;
        }
        case 87: {
            // this one can go up and right
            auto& u = mLayout->at((mCurrent.mPosition.second - 1U) * mMaxWidth + mCurrent.mPosition.first);
            auto& r = mLayout->at(mCurrent.mPosition.second * mMaxWidth + (mCurrent.mPosition.first + 1U));
            return r.mPosition == mPrev ? u : r;
        }
        case 88: {
            // this one can go left and up
            auto& u = mLayout->at((mCurrent.mPosition.second - 1U) * mMaxWidth + mCurrent.mPosition.first);
            auto& l = mLayout->at(mCurrent.mPosition.second * mMaxWidth + (mCurrent.mPosition.first - 1U));
            return l.mPosition == mPrev ? u : l;
        }
        case 89: {
            // this one can go down and up
            auto& u = mLayout->at((mCurrent.mPosition.second - 1U) * mMaxWidth + mCurrent.mPosition.first);
            auto& d = mLayout->at((mCurrent.mPosition.second + 1U) * mMaxWidth + mCurrent.mPosition.first);
            return u.mPosition == mPrev ? d : u;
        }
        }
        return mCurrent;
    }
}

bool Chaser::isNextConnected()
{
    UE_LOG(LogTemp, Error, TEXT("Current: %u"), mCurrent.mTileInfo.PackedTileIndex);
    UE_LOG(LogTemp, Error, TEXT("Next: %u"), mNext.mTileInfo.PackedTileIndex);
    bool toRight = mCurrent.mPosition.first < mNext.mPosition.first;
    bool toUp = mCurrent.mPosition.second > mNext.mPosition.second;
    if (toUp) {
        UE_LOG(LogTemp, Error, TEXT("Go up"));
    } else { UE_LOG(LogTemp, Error, TEXT("Go down")); }
    if (toRight) {
        UE_LOG(LogTemp, Error, TEXT("Go right"));
    }
    else { UE_LOG(LogTemp, Error, TEXT("Go left")); }
    std::set<int> possible({ 0 });
    if (mCurrent.mType != TileType::Start)
    {
        switch (mCurrent.mTileInfo.PackedTileIndex)
        {
        case 62:
            if (toRight) {
                // right
                possible = { 63, 64, 88, 364 };
            }
            else {
                // down
                possible = { 87, 88, 89 };
            }
            break;
        case 63:
            if (!toUp) {
                // down
                possible = { 87, 88, 89 };
            }
            else {
                // left
                possible = { 62, 64, 87};
            }
            break;
        case 64:
            //left
            if (!toRight) {
                possible = { 62, 64, 87 };
            }
            else {
                possible = { 63, 64, 88, 364 };
            }
            break;
        case 87:
            if (toRight) {
                possible = { 63, 64, 88, 364 };
            } // up
            else {
                possible = { 62, 63, 89 };
            }
            break;
        case 88:
            if (toUp) {
                possible = { 62, 63, 89 };
            }
            else {
                possible = { 62, 64, 87 };
            }
            break;
        case 89:
            if (toUp) {
                possible = { 62, 63, 89 };
            }
            else {
                possible = { 87, 88, 89 };
            }
            break;
        }
    }
    else {
        possible = { 63, 64, 88 };
    }

    return possible.contains(mNext.mTileInfo.PackedTileIndex);
}

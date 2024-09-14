// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardLayout.h"

#include <cstdlib>
#include <climits>
#include <map>
#include <cmath>
#include <queue>
#include <set>

TileLayout& operator<<(TileLayout& layout, TileData& tileData)
{
	// TODO: insert return statement here
	layout.push_back(std::move(tileData));
	return layout;
}

bool operator==(TilePosition const& lhs, TilePosition const& rhs)
{
	return (lhs.first == rhs.first) && (lhs.second == rhs.second);
}

uint32 BoardLayout::manhattanScore(TilePosition const& curr, TilePosition const& end)
{
	return std::abs(static_cast<int>(curr.first) - static_cast<int>(end.first)) + std::abs(static_cast<int>(curr.first) - static_cast<int>(end.second));
}

std::vector<TileData*> BoardLayout::getNeighbours(TileData& current)
{
	auto top = 1U;
	auto left = 1U;
	auto bot = mMaxRows - 1U;
	auto right = mMaxCols - 1U;

	std::vector<TileData*> neighbours;
	if (current.mPosition.second > top) {
		//push upper
		neighbours.push_back(&mLayout[(current.mPosition.second - 1U) * mMaxCols + current.mPosition.first]);
	}
	if (current.mPosition.second < bot) {
		// push lower
		neighbours.push_back(&mLayout[(current.mPosition.second + 1U) * mMaxCols + current.mPosition.first]);
	}
	if (current.mPosition.first > left) {
		//push left
		neighbours.push_back(&mLayout[current.mPosition.second * mMaxCols + (current.mPosition.first - 1U)]);
	}
	if (current.mPosition.first < right) {
		//push right
		neighbours.push_back(&mLayout[current.mPosition.second * mMaxCols + (current.mPosition.first + 1U)]);
	}

	return neighbours;
}

void BoardLayout::generateLandscape()
{
	UE_LOG(LogTemp, Error, TEXT("-----------------> generateLandscape"));
	int allowed[] = { 62, 63, 64, 87, 88, 89 };
	for (size_t y = 0; y < mMaxRows; ++y) {
		for (size_t x = 0; x < mMaxCols; ++x) {
			TileData newTile;
			if ((x > 0 && y > 0) && (x < (mMaxCols - 1U) && (y < (mMaxRows - 1U))))
			{
				newTile.mTileInfo.PackedTileIndex = allowed[std::rand() % 6];
				newTile.mWeight = std::rand() % 1000;

				// give it a bit of flair
				if (newTile.mWeight % 6 == 0) {
					newTile.mWeight *= 4U;
				}
				if (newTile.mWeight % 11 == 0) {
					newTile.mWeight *= 8U;
				}
				newTile.g = UINT32_MAX;
				newTile.f = UINT32_MAX;
				newTile.mFrame = false;
			}
			else {
				newTile.mFrame = true;
				newTile.mTileInfo.PackedTileIndex = 3;
			}
			newTile.mPosition = TilePosition(x, y);
			mLayout << newTile;
		}
	}

}

void BoardLayout::generatePath(TilePosition const& start, TilePosition const& end)
{
	UE_LOG(LogTemp, Error, TEXT("-----------------> generatePath"));
	struct Node {
		TileData mTileData;
		Node(TileData& tileData) : mTileData(tileData) {
		};
	};

	struct NodeCompare {
		bool operator()(const Node l, const Node r) const { return l.mTileData.f > r.mTileData.f; }
	};

	std::priority_queue<Node, std::vector<Node>, NodeCompare> openList;
	std::map<TilePosition, TilePosition> parentMap;
	std::set<TilePosition> openListSet;

	auto startTile = mLayout[start.second * mMaxCols + (start.first + 1U)];
	auto endTile = mLayout[end.second * mMaxCols + (end.first - 1U)];
	Node startNode(startTile);
	startNode.mTileData.g = 0U;
	startNode.mTileData.f = manhattanScore(start, end);
	openList.push(std::move(startNode));
	openListSet.insert(startNode.mTileData.mPosition);

	while (!openList.empty()) {
		Node current = openList.top();

		if (current.mTileData.mPosition == endTile.mPosition) {
			std::vector<TileData*> res;
			auto pos = current.mTileData.mPosition;
			while (parentMap.contains(pos)) {
				auto next = parentMap[pos];
				res.insert(res.begin(), &mLayout[pos.second * mMaxCols + pos.first]);
				parentMap.erase(pos);
				pos = next;
			}

			for (size_t i = 0U; i < res.size(); i++) {
				if ((i > 0U) && (i < (res.size() - 1U))) {
					if ((res[i - 1U]->mPosition.first != res[i + 1U]->mPosition.first) && (res[i - 1U]->mPosition.second != res[i + 1U]->mPosition.second)) {
						res[i]->mTileInfo.PackedTileIndex = 63;
					}
					else {
						res[i]->mTileInfo.PackedTileIndex = 89;
					}
				}
			}
			mLayout[start.second * mMaxCols].mTileInfo.PackedTileIndex = 64;
			mLayout[start.second * mMaxCols].mStartEnd = true;
			mLayout[start.second * mMaxCols].mFrame = false;
			mLayout[end.second * mMaxCols + end.first].mTileInfo.PackedTileIndex = 64;
			mLayout[end.second * mMaxCols + end.first].mStartEnd = true;
			mLayout[end.second * mMaxCols + end.first].mFrame = false;

			return;
		}

		openList.pop();
		openListSet.erase(current.mTileData.mPosition);

		auto neighbours = getNeighbours(current.mTileData);

		for (auto& it : neighbours) {

			Node neighbour(*it);
			uint32 temp_g = current.mTileData.g + neighbour.mTileData.mWeight;

			if (temp_g < neighbour.mTileData.g) {
				parentMap[neighbour.mTileData.mPosition] = current.mTileData.mPosition;
				neighbour.mTileData.g = temp_g;
				neighbour.mTileData.f = temp_g + manhattanScore(neighbour.mTileData.mPosition, end);
				mLayout[neighbour.mTileData.mPosition.second * mMaxCols + neighbour.mTileData.mPosition.first].g = temp_g;
				mLayout[neighbour.mTileData.mPosition.second * mMaxCols + neighbour.mTileData.mPosition.first].f = temp_g + manhattanScore(neighbour.mTileData.mPosition, end);
				// if neighbour not in openList add to openList
				if (!openListSet.contains(current.mTileData.mPosition)) {
					openList.push(neighbour);
					openListSet.insert(neighbour.mTileData.mPosition);
				}
			}
		}
	}


}

BoardLayout::BoardLayout()
	: mLayout(TileLayout())
	, mMaxRows(0U)
	, mMaxCols(0U)
{
}

TileLayout BoardLayout::createLayout(size_t const rows, size_t const cols)
{
	// we need size of atleast 2x2
	if ((rows < 2U) || (cols < 2U)) {
		return TileLayout();
	}
	mMaxRows = rows;
	mMaxCols = cols;

	// mLayout.resize(mMaxRows * mMaxCols);

	// TODO: insert return statement here
	TilePosition start(0U, (std::rand() % (mMaxRows - 2U)) + 1U);
	TilePosition end(mMaxCols - 1U, (std::rand() % (mMaxRows - 2U)) + 1U);

	generateLandscape();
	generatePath(start, end);


	return mLayout;

}



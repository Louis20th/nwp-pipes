// Fill out your copyright notice in the Description page of Project Settings.


#include "BoardLayout.h"
#include "PathGenerator"

std::vector<TileData>& operator<<(std::vector<TileData>& layout, TileData& tileData)
{
	// TODO: insert return statement here
	layout.push_back(tileData);
}

BoardLayout::BoardLayout()
{
}


std::vector<TileData> const& BoardLayout::createLayout(size_t const rows, size_t const cols)
{
	// TODO: insert return statement here

	mLayout.clear();
	mLayout.resize(rows * cols);

	generateLandscape();


	PathGenerator generator;
	auto winPath = generator.


}


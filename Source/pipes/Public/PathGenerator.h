// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

using TilePosition = std::pair<uint8, uint8>;
using 

/**
 * 
 */
class PIPES_API PathGenerator
{
public:
	PathGenerator();
	~PathGenerator() = default;

	void generatePath();
};

//1. create empty list -> input size.x and size.y
//2. generate start and stop points -> input size.x and size.y
//3. generate landscape (don't block start/stop) -> input start/stop
//4. find path -> input start/stop
//5. populate not set tiles to random type and orientation

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class PIPES_API GameDifficulty
{
public:
	GameDifficulty();
	~GameDifficulty() = default;

private:
	// countdown timer value in ms
	uint32 mCountdown;
};

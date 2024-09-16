// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDifficulty.h"

/**
 * 
 */
class PIPES_API PlayerSession
{
public:
	PlayerSession();
	~PlayerSession() = default;

	void startSession();
	void onWin();

	uint32 getScore() const;
	void updateScore(size_t const pathLenght);
	float getOverflowSpeed() const;
	float getCountdownAmount() const;

private:

	struct Difficulty {
		float mCoundown;
		float mOverflowSpeed;
		uint32 mStreak;

		void onSuccess();
		void reset();
	};

	Difficulty mDifficulty;
	uint32 mScore;

};

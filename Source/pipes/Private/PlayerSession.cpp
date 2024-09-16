// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSession.h"

// for now start every session from the start
// TODO: handle paused session continuation
PlayerSession::PlayerSession()
	: mDifficulty{ }
	, mScore(0U)
{
}

void PlayerSession::startSession()
{
	mScore = 0;
	mDifficulty.reset();
}

void PlayerSession::onWin()
{
	mDifficulty.onSuccess();
}

uint32 PlayerSession::getScore() const
{
	return mScore;
}

void PlayerSession::updateScore(size_t const pathLenght)
{
	mScore += pathLenght;
}

float PlayerSession::getOverflowSpeed() const
{
	return mDifficulty.mOverflowSpeed;
}

float PlayerSession::getCountdownAmount() const
{
	return mDifficulty.mCoundown;
}

void PlayerSession::Difficulty::onSuccess()
{
	mStreak += 1U;
	if (mStreak > 5) {
		mCoundown -= 10;
		mOverflowSpeed *= 0.5;
	} if (mStreak > 10) {
		mCoundown -= 10;
		mOverflowSpeed *= 0.5;
	}
}

void PlayerSession::Difficulty::reset()
{
	mStreak = 0U;
	mCoundown = 30;
	mOverflowSpeed = 1.5;
}

// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameDifficulty.h"

enum class SessionState
{
	NotStarted,
	Started,
	InProgress,
	Ended,
	Paused // not used yet
};

/**
 * 
 */
class PIPES_API PlayerSession
{
public:
	PlayerSession();
	~PlayerSession() = default;

	void setState(const SessionState newwState);
	const SessionState& getState() const;

	void updateScore(/*void for now, but should get level state to calculate*/);
	uint32 getScore() const;


private:
	SessionState mSessionState;
	uint32 mScore;

};

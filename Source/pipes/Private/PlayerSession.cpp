// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSession.h"

// for now start every session from the start
// TODO: handle paused session continuation
PlayerSession::PlayerSession()
	: mSessionState(SessionState::NotStarted)
	, mScore(0U)
{
}

void PlayerSession::setState(const SessionState newwState)
{
	// pass for now
}

const SessionState& PlayerSession::getState() const
{
	return mSessionState;
}

void PlayerSession::updateScore()
{
	// pass for now
}

uint32 PlayerSession::getScore() const
{
	return mScore;
}

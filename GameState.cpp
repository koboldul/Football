#include "GameState.h"

namespace StateManagement
{

void GameState::pause()
{
	_currentState |= gsGamePaused;
}

void GameState::resume()
{
	_currentState &= ~gsGamePaused;
}

void GameState::setGameState(GameStates state)
{
	_currentState = state;
}

void GameState::startGame()
{
	_currentState |= gsGameStarted;
}

TurnPhase GameState::getTurnPhase()
{
	return _turnPhase;
}

void GameState::next()
{
	int t = (int)_turnPhase;
	t++;
	if (t == 5)
	{
		t = 1;
	}
	_turnPhase = (TurnPhase)t;
}

bool GameState::allowUpdates()
{
	return !(_currentState & gsGamePaused);
}

bool GameState::allowTouch()
{
	return !(_currentState & gsGamePaused) && isGameStarted();
}

bool GameState::isGameStarted()
{
	return  _currentState & gsGameStarted;
}

}
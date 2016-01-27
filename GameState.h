#ifndef __GAME_STATE_H__
#define __GAME_STATE_H__

#define sgl_GameState StateManagement::GameState::getInstance()

namespace StateManagement
{

typedef enum _GameStates
{
    gsGameNone = 0,
	gsGameStarted = 1,
    gsGamePlaying = 2,
	gsGamePaused = 4,
	gsGameOver = 8,
	gsGameWon = 16
} GameStates;

typedef enum _TurnPhase
{
	tNotStarted = 0,
    tAiming = 1,
    tPass = 2
} TurnPhase;

class GameState 
{
public:

	static GameState& getInstance()
	{
		static GameState instance; 

		return instance;
	}

	void pause();
	void resume();
	void startGame();

	void setGameState(GameStates state);
	
	TurnPhase getTurnPhase();
	void next();
	
	bool allowUpdates();
	bool allowTouch();
	bool isGameStarted();

private:
	GameState() {}; 
	GameState(GameState const&); 
	void operator=(GameState const&); 
	
	int _currentState;
	TurnPhase _turnPhase;
};
}
#endif // __GAME_STATE_H__
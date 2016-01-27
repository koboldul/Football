/*
 * Defines the actual player - computer, human.	
 */

#ifndef __Player__
#define __Player__

#include "Footballer.h"
#include "SoccerGlobal.h"
#include "StadionLayer.h"
#include "Gate.h"
class Gate;

using namespace SoccerGlobal;

class Player : public CCObject
{

public:
    ~Player();
	Player();
	void init(StadionLayer * game, TerrainPosition side);

	CC_SYNTHESIZE(int, _score, Score);
	CC_SYNTHESIZE(int, _teamSize, TeamSize);
	CC_SYNTHESIZE(StadionLayer *, _game, Game);
	CC_SYNTHESIZE(Gate*, _gate, Gate);
	CC_SYNTHESIZE(CCArray *, _team, Team);
	CC_SYNTHESIZE(TerrainPosition, _side, Side);


private:
    //void initPlayer();
	void createTeam();
	CCPoint getFootballerPosition(int playerNumber);

};

#endif /* defined(__Soccer__Player__) */

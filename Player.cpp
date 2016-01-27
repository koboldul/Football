#include "pch.h"

#include "Player.h"

using namespace SoccerGlobal;

Player::Player() {
	setScore(0);
	setTeamSize(11);
}

Player::~Player() {  
    CC_SAFE_RELEASE(_team);
    
    CCLOG("destroy ~Player player");
}

void Player::init(StadionLayer * game, TerrainPosition side)
{
	_game = game;
	_side = side;

	_gate = Gate::create(_game, _side);
	_game->addChild(_gate, 3);

	createTeam();
}

//Private 
void Player::createTeam()
{
	_team = CCArray::createWithCapacity(2);
	_team->retain();

	for(int i=0; i < getTeamSize(); i++)
	{
		Footballer * player = Footballer::create(_game, _side, getFootballerPosition(i));
        
		_team->addObject(player);
		_game->addChild(player, 1);
	}
}

CCPoint Player::getFootballerPosition(int playerNumber)
{
    int x = 0;
    int y = 0;

    // [ Goalkeeper ]
    if (playerNumber == 0)
    {
        y = Settings::getInstance().getYOffset() + MARGIN_TICKNESS + _game->getTerrainHeight() / 2;

        if (_side == kLeft)
        {
            x = Settings::getInstance().getXOffset() + MARGIN_TICKNESS + 2;
        }
        else
        {
            x = Settings::getInstance().getXOffset() +_game->getTerrainWidth() + MARGIN_TICKNESS / 2 - 2;
        }
    } 

	float relativeX = 0;
    // [ Defenders ]
    if (playerNumber == 1 || playerNumber == 2)
    {
		y = (Settings::getInstance().getYOffset() + MARGIN_TICKNESS + _game->getTerrainHeight() / 2) - (playerNumber == 2 ? -1 : 1) * _gate->getContentSize().height * 0.25f ;
		relativeX = (_game->getTerrainWidth() / 2) * SoccerGlobal::firstRowRatio;
    }

    // [ Middle ]
    if (playerNumber == 3 || playerNumber == 4 || playerNumber == 5)
    {
		float middle = Settings::getInstance().getYOffset() + MARGIN_TICKNESS + _game->getTerrainHeight() / 2;
        relativeX = (_game->getTerrainWidth() / 2) * SoccerGlobal::secondRowRatio;
		y = middle + (playerNumber - 4) * (15 + _gate->getContentSize().height/2);
    }

    // [ Attack ]
    if (playerNumber == 6|| playerNumber == 7 || playerNumber == 8|| playerNumber == 9 || playerNumber == 10)
    {
		float middle = Settings::getInstance().getYOffset() + MARGIN_TICKNESS + _game->getTerrainHeight() / 2;
        y = middle + (playerNumber - 8) * (30 + _gate->getContentSize().height/2);
		relativeX = (_game->getTerrainWidth() / 2) * SoccerGlobal::thirdRowRatio;
    }

    if (_side == kLeft)
    {
        x = Settings::getInstance().getXOffset() + MARGIN_TICKNESS + relativeX;
    }
    else
    {
        x = Settings::getInstance().getXOffset() + MARGIN_TICKNESS + _game->getTerrainWidth() - relativeX;
    }
     
    return ccp(x, y);
}


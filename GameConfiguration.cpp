//
//  GameConfiguration.cpp
//  ZombieSoccer
//
//  Created by Bogdan Costea on 9/22/13.
//
//

#include "GameConfiguration.h"

GameConfiguration::GameConfiguration() :
    _activePlayerID(DEFAULT_ID)
{
}

GameConfiguration::~GameConfiguration()
{
}

void GameConfiguration::switchActivePlayer()
{
    _lPlayerC->setIsActive(!_lPlayerC->getIsActive());
    _rPlayerC->setIsActive(!_rPlayerC->getIsActive());
    
    if (_lPlayerC->getIsActive())
    {
        setActivePlayerID(_lPlayerC->getID());
    }
    else if (_rPlayerC->getIsActive())
    {
        setActivePlayerID(_rPlayerC->getID());
    }
    else
    {
        assignActivePlayer();
    }
}

void GameConfiguration::assignActivePlayer()
{
    if (getActivePlayerID() != DEFAULT_ID)
    {
        if (_lPlayerC->getID() == getActivePlayerID())
        {
            _lPlayerC->setIsActive(true);
        }
        else
        {
            _rPlayerC->setIsActive(true);
        }
    }
    else
    {
        int flipResult = randomFlip();
    
        _rPlayerC->setIsActive(flipResult == kRight);
        _lPlayerC->setIsActive(flipResult == kLeft);
        
        if (_rPlayerC->getPlayerType() == kHuman &&
            _lPlayerC->getPlayerType() == kHumanRemote &&
            !_rPlayerC->getIsActive())
        {
            setLeftPlayerC(_rPlayerC);
            setRightPlayerC(_lPlayerC);
            
            _rPlayerC->setSide(kLeft);
            _lPlayerC->setSide(kRight);
        }
    }
}
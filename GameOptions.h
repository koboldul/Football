//
//  GameOptions.h
//  ZombieSoccer
//
//  Created by Bogdan Costea on 9/8/13.
//
//

#ifndef __ZombieSoccer__GameOptions__
#define __ZombieSoccer__GameOptions__

#include <string>

enum TerrainType
{
    ttGrass = 0,
    ttSand = 1,
    ttAsphalt = 2
};

enum BallSkin
{
    bskClassic = 0
};

enum WinCondition
{
  wcTime = 0,
  wcScore = 1
};

struct GameOptions
{
    TerrainType terrainType;
    BallSkin ballSkin;
    
    std::string PlayerName;
    std::string Room;
    
    WinCondition winHow;
    
    int Score;
    int Time;
};

#endif /* defined(__ZombieSoccer__GameOptions__) */

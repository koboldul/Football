#ifndef __b2Sprite__
#define __b2Sprite__

#include "cocos2d.h"
#if(CC_TARGET_PLATFORM == CC_PLATFORM_WP8 || CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) 
#include "Box2D/Box2D.h"
#else
#include "Box2D/Box2D.h"
#endif

class StadionLayer;

using namespace cocos2d;


enum {
    kSpriteFootballer,
    kSpriteBall,
    kSpriteGate
};


class b2Sprite : public CCSprite
{
    
public:
    
    b2Sprite (StadionLayer * game, int type);
    ~b2Sprite();

    CC_SYNTHESIZE(b2Body *, _body, Body);
    CC_SYNTHESIZE(StadionLayer *, _game, Game);
    CC_SYNTHESIZE(int, _spriteType, SpriteType);
    
    virtual void setSpritePosition (CCPoint position);
    virtual void update(float dt);
    virtual void hide(void);
    virtual void reset(void);
    virtual float mag();
};


#endif /* defined(__ZSoccer__b2Sprite__) */

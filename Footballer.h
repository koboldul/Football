#ifndef __Footballer__
#define __Footballer__

#include "b2Sprite.h"

class Footballer : public b2Sprite {

public:
    ~Footballer();
	Footballer(StadionLayer * game, int type, CCPoint position);

	CC_SYNTHESIZE(int, _color, Color);

    static Footballer* create(StadionLayer * game, int type, CCPoint position);
    
private:
    void initFootballer(StadionLayer * game);
    CCPoint _startPosition;
	int _type;
};

#endif /* defined(__Footballer__) */

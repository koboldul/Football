#include "pch.h"
#include "SoccerGlobal.h"
#include "cocos2d.h"


namespace SoccerGlobal
{
	void Settings::scaleSprite(CCSprite* sprite)
	{
		//CCSize size = CCEGLView::sharedOpenGLView()->getFrameSize();
        ///CCSize design = CCDirector::sharedDirector()->getWinSize();
        
		//sprite->setScaleX(size.width / design.width);
		//sprite->setScaleY(size.height/ design.height);
	}
    
    void Settings::scaleSprite(CCSprite* sprite,  float constantScale)
	{
		//CCSize size = CCEGLView::sharedOpenGLView()->getFrameSize();
        //CCSize design = CCDirector::sharedDirector()->getWinSize();
		
        sprite->setScaleX(constantScale);
		sprite->setScaleY(constantScale);
	}
}



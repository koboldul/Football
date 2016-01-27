#ifndef __OptionsLayer__
#define __OptionsLayer__

#include "OverlayBase.h"
#include "cocos2d.h"
#include <cocos-ext.h>
#include <SpriteFactory.h>
#include "SoccerGlobal.h"
#include "ControlFactory.h"

USING_NS_CC_EXT;

class OptionsLayer : public OverlayBase, public CCEditBoxDelegate
{
public:
	OptionsLayer();
	~OptionsLayer();
    
    bool init();

	void editBoxEditingDidBegin(CCEditBox* editBox);
    void editBoxEditingDidEnd(CCEditBox* editBox);
    void editBoxTextChanged(CCEditBox* editBox, const std::string& text);
    void editBoxReturn(CCEditBox* editBox);

private:
	CCLabelTTF* _lblGoalSelection;

	void initControls();
	void sliderEvent(CCObject *pSender, CCControlEvent controlEvent); 

	void onApply(CCObject* pSender);
    
    CCControlSlider* _goalNoSlider;
    CCControlSlider* _timeSlider;
	CCEditBox* _txtName;
};

#endif //__OptionsLayer__
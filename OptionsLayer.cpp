#include "pch.h"

#include "OptionsLayer.h"
#include "cocos2d.h"

int _nameTag = 1234;
int goalTag = 111;
int timerTag = 222;

using namespace SoccerGlobal;

OptionsLayer::OptionsLayer()
{
    init();
}

OptionsLayer::~OptionsLayer()
{
   
}

bool OptionsLayer::init()
{
	bool bRet = false;
	
	do 
	{
		CC_BREAK_IF(! OverlayBase::init());
        
		CCSize size = CCDirector::sharedDirector()->getWinSize();
		int leftSpace = 100;

		setTouchEnabled(true);
	
		CCSprite* bg = CCSprite::create(SETTINGS_BG_NAME.c_str());
		
		bg->setAnchorPoint(ccp(0,0));
        bg->setPosition(ccp(0, 0));
		addChild(bg, -1);

		//Cancel - Go Menu
        CCMenuItemSprite* mRightArrow = sgl_SpriteFactory.createMenuItemSprite("btnArrow_Back128x128.png", "btnArrow_Back128x128Pressed.png", menu_selector(OverlayBase::onQuit), this);
        CCMenu* menuReturn = CCMenu::create(mRightArrow, NULL);
        
        CCMenuItemSprite* mLeftArrow = 
			sgl_SpriteFactory.createMenuItemSprite("btnArrow128x128.png", "btnArrow128x128.png", menu_selector(OptionsLayer::onApply), this);
        CCMenu* mStartGame = CCMenu::create(mLeftArrow, NULL);
        
		menuReturn->setPosition(leftSpace, 150);
		mStartGame->setPosition(size.width - leftSpace - mLeftArrow->boundingBox().size.width * 0.5f, 150);
        
        addChild(mStartGame);
        addChild(menuReturn);

		initControls();

		bRet = true;
	} while (0);

	return bRet;
}

void OptionsLayer::onApply(CCObject* pSender)
{
	GameOptions opt;

	int goals = 0;

	goals = _goalNoSlider->getValue();
	opt.Score = goals;
    
	CCUserDefault::sharedUserDefault()->setIntegerForKey(GOALS_DATA_NAME, goals);

	std::string name = _txtName->getText();
	if (!name.empty())
	{
		CCUserDefault::sharedUserDefault()->setStringForKey(USER_DATA_NAME, name);
	}
	
	sgl_Settings.setOptions(opt);
    
    OverlayBase::onQuit(pSender);
}

/*
 * Controls Events
 */
void OptionsLayer::sliderEvent(CCObject *pSender, CCControlEvent controlEvent)
{
	CCControlSlider* pSlider = (CCControlSlider*)pSender;

	int percent = pSlider->getValue();
	std::string s = "g:";
	s += percent;

	_lblGoalSelection->setString(s.c_str());
}

/*
 * Control initialization
 */
void OptionsLayer::initControls()
{
	int xPos = 100;
	int fontSize = 35;

	CCSize visibleSize = CCEGLView::sharedOpenGLView()->getVisibleSize();
    
	//Labels
	CCLabelTTF* lblName = sgl_ControlFactory.createTTFLabel("Name:", ccp(xPos, 500));
    this->addChild(lblName);

	CCLabelTTF* lblGoal = sgl_ControlFactory.createTTFLabel("Goals:", ccp(xPos, 400));
	this->addChild(lblGoal);

	CCLabelTTF* lblColor = sgl_ControlFactory.createTTFLabel("Time:", ccp(xPos, 300));
	this->addChild(lblColor);

	//Controls
	int controlLimit = xPos + lblGoal->boundingBox().size.width + 20;

    std::string userName = CCUserDefault::sharedUserDefault()->getStringForKey(USER_DATA_NAME);
    if (userName.empty())
    {
        userName = "anonymus";
    }
    
	CCSize editBoxSize = CCSizeMake(visibleSize.width * 0.2f, 50);
	CCScale9Sprite* sp = CCScale9Sprite::create("textbg.png");
	
	_txtName = CCEditBox::create(editBoxSize, sp);
    _txtName->setPosition(ccp(controlLimit+10, 480));
	_txtName->setFontName(S_F_TYPE.c_str());
	_txtName->setFontSize(fontSize);
    _txtName->setFontColor(ccRED);
	_txtName->setPlaceHolder(userName.c_str());
    _txtName->setPlaceholderFontColor(ccWHITE);
    _txtName->setMaxLength(15);
    _txtName->setReturnType(kKeyboardReturnTypeDone);
    _txtName->setDelegate(this);
	_txtName->setZOrder(1);
    _txtName->setAnchorPoint(ccp(0,0));
	
	addChild(_txtName);

	int goals = CCUserDefault::sharedUserDefault()->getIntegerForKey(GOALS_DATA_NAME);
	if (goals < 1) goals = 10;

	_goalNoSlider = CCControlSlider::create("sliderTrack.png", "sliderProgress.png", "sliderThumb.png");
    
    _goalNoSlider->setTouchEnabled(true);
    _goalNoSlider->setMinimumValue(1.0f); // Sets the min value of range
    _goalNoSlider->setMaximumValue(goals); // Sets the max value of range
    _goalNoSlider->setPosition(ccp(controlLimit+100, 400));
	_goalNoSlider->setAnchorPoint(ccp(0,0));
	_goalNoSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(OptionsLayer::sliderEvent), CCControlEventValueChanged);
	_goalNoSlider->setTag(goalTag);

	_lblGoalSelection = sgl_ControlFactory.createTTFLabel("", ccp(controlLimit+_goalNoSlider->boundingBox().size.width+100, 400));
    this->addChild(_lblGoalSelection);

	_timeSlider = CCControlSlider::create("sliderTrack.png", "sliderProgress.png", "sliderThumb.png");
    
    _timeSlider->setTouchEnabled(true);
    _timeSlider->setPosition(ccp(controlLimit+100, 300));
	_timeSlider->setAnchorPoint(ccp(0,0));
	_timeSlider->addTargetWithActionForControlEvents(this, cccontrol_selector(OptionsLayer::sliderEvent), CCControlEventValueChanged);
	_timeSlider->setTag(timerTag);

    addChild(_goalNoSlider);
	addChild(_timeSlider);
}

/* Delegate thinggy */

void OptionsLayer::editBoxEditingDidBegin(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidBegin !", editBox);
}

void OptionsLayer::editBoxEditingDidEnd(cocos2d::extension::CCEditBox* editBox)
{
    CCLog("editBox %p DidEnd !", editBox);
}

void OptionsLayer::editBoxTextChanged(cocos2d::extension::CCEditBox* editBox, const std::string& text)
{
    CCLog("editBox %p TextChanged, text: %s ", editBox, text.c_str());
}

void OptionsLayer::editBoxReturn(CCEditBox* editBox)
{
    CCLog("editBox %p was returned !",editBox);
    
    /*if (m_pEditName == editBox)
    {
        m_pTTFShowEditReturn->setString("Name EditBox return !");
    }
    else if (m_pEditPassword == editBox)
    {
        m_pTTFShowEditReturn->setString("Password EditBox return !"); 
    }
    else if (m_pEditEmail == editBox)
    {
        m_pTTFShowEditReturn->setString("Email EditBox return !");
    }*/
}
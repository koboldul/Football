//
//  ControlFactory.cpp
//  ZombieSoccer
//
//  Created by Bogdan Costea on 9/2/13.
//
//

#include "ControlFactory.h"

CCLabelTTF* ControlFactory::createTTFLabel(std::string text, CCPoint pos)
{
    CCLabelTTF* label = CCLabelTTF::create();
	label->setString(text.c_str());
	label->setPosition(pos);
	label->setFontName(S_F_TYPE.c_str());
	label->setFontSize(35);
	
	return label;
}

CCLabelBMFont* ControlFactory::createBMFontLabel(std::string text, CCPoint pos)
{
    CCLabelBMFont* label = CCLabelBMFont::create(text.c_str(), "fonts/ballFont.fnt", 30);
	
	label->setPosition(pos);
		
	return label;
}


/*
* cocos2d-x   http://www.cocos2d-x.org
*
* Copyright (c) 2010-2011 - cocos2d-x community
* 
* Portions Copyright (c) Microsoft Open Technologies, Inc.
* All Rights Reserved
* 
* Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at 
* 
* http://www.apache.org/licenses/LICENSE-2.0 
* 
* Unless required by applicable law or agreed to in writing, software distributed under the License is distributed on an 
* "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and limitations under the License.
*/

#include "pch.h"
#include "MyContactListener.h"
#include "Gate.h"
#include "SimpleAudioEngine.h"

using namespace CocosDenshion;

MyContactListener::MyContactListener()
{
}

MyContactListener::~MyContactListener()
{
}

void MyContactListener::BeginContact(b2Contact* contact) {
    b2Body * bodyA = contact->GetFixtureA()->GetBody();
    b2Body * bodyB = contact->GetFixtureB()->GetBody();
    
    b2Sprite * spriteA = (b2Sprite *) bodyA->GetUserData();
    b2Sprite * spriteB = (b2Sprite *) bodyB->GetUserData();
    
    if (spriteA && spriteB) 
	{
		Gate *gate = NULL;
        Ball *b = NULL;
        
		if (kSpriteFootballer == spriteA->getSpriteType() ||
			kSpriteFootballer == spriteB->getSpriteType())
		{
			SimpleAudioEngine::sharedEngine()->playEffect(BALL_SOUND);

			return;
		}

		if (kSpriteGate == spriteA->getSpriteType()) 
		{
			gate = (Gate*)spriteA;
			b = (Ball*)spriteB;
		} 
		else if (kSpriteGate == spriteB->getSpriteType()) 
		{
            gate = (Gate*)spriteB;
            b = (Ball*)spriteA;
		}
        
		if(NULL != gate){
			
            if (NULL != b)
            {
				if (gate->getType() == kRight && b->getPosition().x > (gate->getPositionX() - gate->boundingBox().size.width / 2))
				{
	                gate->setHoldsBall(true);
				}
				else if (gate->getType() == kLeft && b->getPosition().x < (gate->getPositionX() + gate->boundingBox().size.width / 2))
				{
					gate->setHoldsBall(true);
				}
            }
        }
	}
}

void MyContactListener::PreSolve(b2Contact* contact, const b2Manifold* oldManifold)  {
    
   // b2Body * bodyA = contact->GetFixtureA()->GetBody();
   // b2Body * bodyB = contact->GetFixtureB()->GetBody();
    
    //b2Sprite * spriteA = (b2Sprite *) bodyA->GetUserData();
    //b2Sprite * spriteB = (b2Sprite *) bodyB->GetUserData();
    //
    //    
    //if (spriteA && spriteB) {
    //    
    //    //track collision between player and cue ball
    //    if (spriteA->getType() == kSpriteCue && spriteA->mag() > 2) {
    //        if (spriteB->getType() == kSpritePlayer && spriteA->isVisible()) {
    //            SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
    //            spriteA->setVisible(false);
    //            spriteB->getGame()->setCanShoot(false);
    //        }
    //    } else if (spriteB->getType() == kSpriteCue && spriteA->mag() > 2) {
    //        if (spriteA->getType() == kSpritePlayer && spriteB->isVisible()) {
    //            SimpleAudioEngine::sharedEngine()->playEffect("hit.wav");
    //            spriteB->setVisible(false);
    //            spriteA->getGame()->setCanShoot(false);
    //        }
    //    } 
    //
    //}
    
}

void MyContactListener::EndContact(b2Contact* contact)
{
}

void MyContactListener::PostSolve(b2Contact* contact,const b2ContactImpulse* impulse)
{
	
}
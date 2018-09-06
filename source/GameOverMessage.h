//*********************************************************************//
//	File:		GameOverMessage.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Sends a Message to Trigger GameOver
//*********************************************************************//

#pragma once
#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"
#include "Hero.h"
#include "Orc.h"

class GameOverMessage : public SGD::Message
{
public:
	//GameOverMessage() = default;
	GameOverMessage(Character* theHero);
	virtual ~GameOverMessage();

	//Accessor
	Character* GetHero() const{ return m_tOwner; }


private:
	Character* m_tOwner = nullptr;
};
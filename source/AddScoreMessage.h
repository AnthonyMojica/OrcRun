//*********************************************************************//
//	File:		AddScoreMessage.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Adds Score
//*********************************************************************//

#pragma once
#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"
#include "Hero.h"
#include "Orc.h"

class AddScoreMessage : public SGD::Message
{
public:
	//AddScoreMessage() = default;
	AddScoreMessage(Character* theHero);
	virtual ~AddScoreMessage();

	//Accessor
	Character* GetHero() const{ return m_tOwner; }


private:
	Character* m_tOwner = nullptr;
};
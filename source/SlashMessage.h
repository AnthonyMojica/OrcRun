//*********************************************************************//
//	File:		SlashMessage.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Sends a Message when the player Slashes to the Orcs
//*********************************************************************//

#pragma once
#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"
#include "Hero.h"
#include "Orc.h"

class SlashMessage : public SGD::Message
{
public:
	//SlashMessage() = default;
	SlashMessage(Hero* theHero);
	virtual ~SlashMessage();

	//Accessor
	Hero* GetHero() const{ return m_tOwner; }


private:
	Hero* m_tOwner = nullptr;
};


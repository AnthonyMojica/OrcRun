//*********************************************************************//
//	File:		CreateArrowMessage.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Sends a Message to Create an Arrow When Fired.
//*********************************************************************//

#pragma once

#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"
#include "Hero.h"
#include "Orc.h"

class CreateArrowMessage : public SGD::Message
{
public:
	//CreateArrowMessage() = default;
	CreateArrowMessage(Hero* theHero);
	virtual ~CreateArrowMessage();

	//Accessor
	Hero* GetHero() const{ return m_tOwner; }


private:
	Hero* m_tOwner = nullptr;
};


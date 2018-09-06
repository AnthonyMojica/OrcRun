//*********************************************************************//
//	File:		CreateOrcMessage.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Creates Orcs
//*********************************************************************//

#pragma once
#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"
#include "Hero.h"
#include "Orc.h"

class CreateOrcMessage : public SGD::Message
{
public:
	//CreateOrcMessage() = default;
	CreateOrcMessage(Hero* theHero);
	virtual ~CreateOrcMessage();

	//Accessor
	Hero* GetHero() const{ return m_tOwner; }


private:
	Hero* m_tOwner = nullptr;
};


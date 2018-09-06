//*********************************************************************//
//	File:		CreateOrcArrowMsg.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Sends a Message to Create an Arrow When Orcs Fire.
//*********************************************************************//

#pragma once

#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"
#include "OrcBow.h"

class CreateOrcArrowMsg : public SGD::Message
{
public:
	//CreateOrcArrowMsg() = default;
	CreateOrcArrowMsg(OrcBow* theHero);
	virtual ~CreateOrcArrowMsg();

	//Accessor
	OrcBow* GetOrc() const{ return m_tOwner; }


private:
	OrcBow* m_tOwner = nullptr;
};

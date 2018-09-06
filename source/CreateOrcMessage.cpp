//*********************************************************************//
//	File:		CreateOrcMessage.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Creates Orcs
//*********************************************************************//

#include "CreateOrcMessage.h"


CreateOrcMessage::CreateOrcMessage(Hero* theHero) : Message(MessageID::MSG_CREATE_ORC)
{
	m_tOwner = theHero;
	if (m_tOwner != nullptr)
		m_tOwner->AddRef();
}


CreateOrcMessage::~CreateOrcMessage()
{
	if (m_tOwner != nullptr)
		m_tOwner->Release();
}

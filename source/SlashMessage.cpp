//*********************************************************************//
//	File:		SlashMessage.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Sends a Message when the player Slashes to the Orcs
//*********************************************************************//

#include "SlashMessage.h"


SlashMessage::SlashMessage(Hero* theHero) : Message(MessageID::MSG_SLASH)
{
	m_tOwner = theHero;
	if (m_tOwner != nullptr)
		m_tOwner->AddRef();
}


SlashMessage::~SlashMessage()
{
	if (m_tOwner != nullptr)
		m_tOwner->Release();
}

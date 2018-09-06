//*********************************************************************//
//	File:		AddScoreMessage.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Adds Score
//*********************************************************************//

#include "AddScoreMessage.h"


AddScoreMessage::AddScoreMessage(Character* theHero) : Message(MessageID::MSG_ADD_SCORE)
{
	m_tOwner = theHero;
	if (m_tOwner != nullptr)
		m_tOwner->AddRef();
}


AddScoreMessage::~AddScoreMessage()
{
	if (m_tOwner != nullptr)
		m_tOwner->Release();
}

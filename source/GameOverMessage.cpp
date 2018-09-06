//*********************************************************************//
//	File:		GameOverMessage.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Sends a Message to Trigger GameOver
//*********************************************************************//

#include "GameOverMessage.h"


GameOverMessage::GameOverMessage(Character* theHero) : Message(MessageID::MSG_GAME_OVER)
{
	m_tOwner = theHero;
	if (m_tOwner != nullptr)
		m_tOwner->AddRef();
}


GameOverMessage::~GameOverMessage()
{
	if (m_tOwner != nullptr)
		m_tOwner->Release();
}

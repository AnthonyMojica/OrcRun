//*********************************************************************//
//	File:		CreateArrowMessage.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Sends a Message to Create an Arrow When Fired.
//*********************************************************************//

#include "CreateArrowMessage.h"


CreateArrowMessage::CreateArrowMessage(Hero* theHero) : Message(MessageID::MSG_CREATE_ARROW)
{
	m_tOwner = theHero;
	if (m_tOwner != nullptr)
		m_tOwner->AddRef();
}


CreateArrowMessage::~CreateArrowMessage()
{
	if (m_tOwner != nullptr)
		m_tOwner->Release();
}

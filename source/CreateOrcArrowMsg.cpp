//*********************************************************************//
//	File:		CreateOrcArrowMsg.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Sends a Message to Create an Arrow When Orcs Fire.
//*********************************************************************//

#include "CreateOrcArrowMsg.h"


CreateOrcArrowMsg::CreateOrcArrowMsg(OrcBow* _TheOrc) : Message(MessageID::MSG_CREATE_ARROW_ORC)
{
	m_tOwner = _TheOrc;
	if (m_tOwner != nullptr)
		m_tOwner->AddRef();
}


CreateOrcArrowMsg::~CreateOrcArrowMsg()
{
	if (m_tOwner != nullptr)
		m_tOwner->Release();
}

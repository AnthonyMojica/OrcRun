//*********************************************************************//
//	File:		MessageID.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	MessageID enum class defines the message types
//*********************************************************************//

#pragma once


//*********************************************************************//
// MessageID enum class
//	- unique identifiers for messages sent to Game::MessageProc
enum class MessageID	
{
	MSG_UNKNOWN = 0,
	MSG_DESTROY_ENTITY = 1,
	MSG_CREATE_ARROW = 2,
	MSG_SLASH = 3,
	MSG_ADD_SCORE = 4,
	MSG_CREATE_ORC = 5,
	MSG_CREATE_ARROW_ORC = 6,
	MSG_GAME_OVER,
};

//*********************************************************************//
//	File:		PauseMenu.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Pauses the game
//*********************************************************************//

#pragma once

#include "IGameState.h"

class PauseMenu : public IGameState
{
public:
	//*****************************************************************//
	// Singleton Accessor:
	static PauseMenu* GetInstance(void);


	//*****************************************************************//
	// IGameState Interface:
	virtual void Enter(void)				override;	// load resources / reset data
	virtual void Exit(void)				override;	// unload resources

	virtual int Update(float elapsedTime)	override;	// handle input / update entities
	virtual void Render(float elapsedTime)	override;	// draw entities / menu

private:
	//*****************************************************************//
	// SINGLETON (not-dynamically allocated)
	//	- Hide the "Quadrilogy-of-Evil" so they cannot be called
	//	  by outside functions
	PauseMenu(void) = default;	// default constructor
	virtual ~PauseMenu(void) = default;	// destructor

	PauseMenu(const PauseMenu&) = delete;	// copy constructor
	PauseMenu& operator= (const PauseMenu&) = delete;	// assignment operator


	//*****************************************************************//
	// cursor index / position
	int m_nCursor = 0;
	float m_fEscapeTimer = 0;
};


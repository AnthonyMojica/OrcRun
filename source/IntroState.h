//*********************************************************************//
//	File:		IntroState.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Intro Screen
//*********************************************************************//

#pragma once
#include "IGameState.h"
#include "CellAnimation.h"

#include <string>

class IntroState: public IGameState
{
public:
	//*****************************************************************//
	// Singleton Accessor:
	static IntroState* GetInstance(void);

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
	IntroState(void) = default;	// default constructor
	virtual ~IntroState(void) = default;	// destructor

	IntroState(const IntroState&) = delete;	// copy constructor
	IntroState& operator= (const IntroState&) = delete;	// assignment operator


	//*****************************************************************//
	// cursor index / position
	int m_nCursor = 0;
	float m_fAttractTimer = 0;
	float HeroPOSX = 0;
	float m_fTextheight = 0;
	bool m_fSwap = false;

	CellAnimation* m_cWalkAnim = nullptr;
};


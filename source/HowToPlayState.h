//*********************************************************************//
//	File:		HowToPlayState.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Displays how to play
//*********************************************************************//

#pragma once

#include "IGameState.h"
#include "CellAnimation.h"


class HowToPlayState : public IGameState
{
public:
	//*****************************************************************//
	// Singleton Accessor:
	static HowToPlayState* GetInstance(void);


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
	HowToPlayState(void) = default;	// default constructor
	virtual ~HowToPlayState(void) = default;	// destructor

	HowToPlayState(const HowToPlayState&) = delete;	// copy constructor
	HowToPlayState& operator= (const HowToPlayState&) = delete;	// assignment operator

	SGD::HTexture m_hOrc = SGD::INVALID_HANDLE;
	SGD::HTexture m_hPauseBack = SGD::INVALID_HANDLE;

	//*****************************************************************//
	// cursor index / position
	int m_nCursor = 0;
	int m_nHowtoState = 0;
	float m_fAnimtimer = 0;
	float m_aAttractTimer = 0;

	CellAnimation* m_cWalkAnim = nullptr;
	CellAnimation* m_nSlash = nullptr;
	CellAnimation* m_cFire = nullptr;
};


//*********************************************************************//
//	File:		CreditsState.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Scrolls the Credits
//*********************************************************************//

#pragma once

#include "IGameState.h"
#include "CellAnimation.h"

class CreditsState : public IGameState
{
public:
	//*****************************************************************//
	// Singleton Accessor:
	static CreditsState* GetInstance(void);

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
	CreditsState(void) = default;	// default constructor
	virtual ~CreditsState(void) = default;	// destructor

	CreditsState(const CreditsState&) = delete;	// copy constructor
	CreditsState& operator= (const CreditsState&) = delete;	// assignment operator

	SGD::HTexture m_hOrc = SGD::INVALID_HANDLE;
	SGD::HTexture m_hPauseBack = SGD::INVALID_HANDLE;

	float m_fScroll = 0;
	float heroPOSX = 0; 
	float OrcPOSX = 0;
	float smallOrcPOSX = 0;
	float smallOrcPace = -150;
	bool _runaway = false;

	float m_aAttractTimer = 0;
	//*****************************************************************//
	// cursor index / position
	int m_nCursor = 0;

	CellAnimation* m_cWalkAnim = nullptr;
	CellAnimation* m_cOrcWalkAnim = nullptr;
	CellAnimation* m_cOrcWalkAnim2 = nullptr;

};


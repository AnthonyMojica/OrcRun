//*********************************************************************//
//	File:		MainMenuState.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	MainMenuState class handles the main menu
//*********************************************************************//

#pragma once

#include "IGameState.h"


//*********************************************************************//
// MainMenuState class
//	- runs the main menu
//	- SINGLETON! (Static allocation, not dynamic)
class MainMenuState : public IGameState
{
public:
	//*****************************************************************//
	// Singleton Accessor:
	static MainMenuState* GetInstance( void );

	
	//*****************************************************************//
	// IGameState Interface:
	virtual void Enter	( void )				override;	// load resources / reset data
	virtual void Exit	( void )				override;	// unload resources

	virtual int Update	( float elapsedTime )	override;	// handle input / update entities
	virtual void Render	( float elapsedTime )	override;	// draw entities / menu

	bool GetAttractState(){ return m_bAttractState; }
	void SetAttractState(bool _state) { m_bAttractState = _state; }


private:
	//*****************************************************************//
	// SINGLETON (not-dynamically allocated)
	//	- Hide the "Quadrilogy-of-Evil" so they cannot be called
	//	  by outside functions
	MainMenuState( void )			= default;	// default constructor
	virtual ~MainMenuState( void )	= default;	// destructor

	MainMenuState( const MainMenuState& )				= delete;	// copy constructor
	MainMenuState& operator= ( const MainMenuState& )	= delete;	// assignment operator

	void LoadSettings();

	//*****************************************************************//
	// cursor index / position
	int m_nCursor = 0;
	float m_fAttractTimer = 0;
	bool m_bAttractState = false;
};


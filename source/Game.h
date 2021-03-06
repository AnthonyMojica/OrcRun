//*********************************************************************//
//	File:		Game.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Game class initializes the SGD Wrappers
//				and runs the game state machine
//*********************************************************************//

#pragma once

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "EntityManager.h"
#include "Entity.h"

#include <stack>

//*********************************************************************//
// Forward class declarations
class BitmapFont;
class IGameState;



//*********************************************************************//
// Game class
//	- handles the SGD wrappers
//	- runs the game state machine
//	- SINGLETON
//		- only ONE instance can be created
//		- global access to THE instance
class Game
{
public:
	//*****************************************************************//
	// SINGLETON!
	//	- static accessor to get the singleton object
	static Game* GetInstance( void );
	static void  DeleteInstance( void );

	
	//*****************************************************************//
	// Setup, Play, Cleanup
	bool	Initialize	( void );
	int		Update		( void );
	void	Terminate	( void );
	
	
	//*****************************************************************//
	// Screen Accessors
	SGD::Size	GetScreenSize	( void ) const	{	return m_szScreenSize;	}

	// Font Accessor (#include "BitmapFont.h" to use!)
	BitmapFont*	GetFont			( void ) const	{	return	m_pFont;		}

	float GetSpeedVar(){ return m_fSpeedVar; }
	//*****************************************************************//
	// Game State Mutator:
	void	ChangeState( IGameState* pNextState );
	void	PushState(IGameState* pPushState);
	void	ClearStates();


	void SetSpeedVar(float Variable) { m_fSpeedVar = Variable; }
	bool GetMainMenuState() { return mainMenu; }
	bool GetPausedState() { return m_bPaused; }
	

	void SetMainMenuState(bool _MenuState) { mainMenu = _MenuState; }
	void SetPausedState(bool _paused) { m_bPaused = _paused; }
	void PopState();

private:
	//*****************************************************************//
	// SINGLETON!
	//	- static member to hold the singleton object
	//	- prevent access to constructors / destructor / =op
	static Game* s_pInstance;

	Game( void )	= default;					// default constructor
	~Game( void )	= default;					// destructor

	Game( const Game& )				= delete;	// disabled copy constructor
	Game& operator=( const Game& )	= delete;	// disabled assignment operator

	float m_fSpeedVar = 1;
	bool fullscreen = false;

	bool mainMenu = true;
	bool m_bPaused = false;
	//*****************************************************************//
	// Screen Size
	SGD::Size		m_szScreenSize		= SGD::Size{ 1024, 768 };
	
	// Font
	BitmapFont*		m_pFont				= nullptr;

	//*****************************************************************//
	// Assets

	//*****************************************************************//
	// Active Game State
	IGameState*		m_pCurrState		= nullptr;
	std::vector<IGameState*> stateStack;



	//*****************************************************************//
	// Game Entities:

	//*****************************************************************//
	// Game Time
	unsigned long	m_ulGameTime	= 0;

};

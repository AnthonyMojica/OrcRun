//*********************************************************************//
//	File:		Game.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Game class initializes the SGD Wrappers
//				and runs the game state machine
//*********************************************************************//

#include "Game.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_Utilities.h"

#include "BitmapFont.h"
#include "IGameState.h"
#include "MainMenuState.h"
#include "GameplayState.h"
#include "IntroState.h"

#include <ctime>
#include <cstdlib>
#include <cassert>

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>


//*********************************************************************//
// SINGLETON
//	- instantiate the static member
/*static*/ Game* Game::s_pInstance = nullptr;

// GetInstance
//	- allocate the singleton if necessary
//	- return the singleton
/*static*/ Game* Game::GetInstance( void )
{
	if( s_pInstance == nullptr )
		s_pInstance = new Game;

	return s_pInstance;
}

// DeleteInstance
//	- deallocate the singleton
/*static*/ void Game::DeleteInstance( void )
{
	delete s_pInstance;
	s_pInstance = nullptr;
}


//*********************************************************************//
// Initialize
//	- initialize the SGD wrappers
//	- start in the MainMenuState
bool Game::Initialize( void )
{
	// Seed First!
	srand( (unsigned int)time( nullptr ) );
	rand();

	// Try to initialize the wrappers
	// (Graphics Manager MUST be first!)
	if( SGD::GraphicsManager::GetInstance()->Initialize( L"SGD Game Project", m_szScreenSize, false ) == false
		|| SGD::InputManager::GetInstance()->Initialize() == false 
		|| SGD::AudioManager::GetInstance()->Initialize() == false )
		return false;	// failure!!!
	
// Hide the console window
#if !defined( DEBUG ) && !defined( _DEBUG )
	SGD::GraphicsManager::GetInstance()->ShowConsoleWindow( false );
#endif

	//Initialize Graphics and Audio assets

	// Change the background color
	SGD::GraphicsManager::GetInstance()->SetClearColor( {155, 0, 0, 0 } );	// black
	
	// Allocate & Initialize the font
	m_pFont = new BitmapFont;
	m_pFont->Initialize();

	// Start in the MainMenuState
	ChangeState(GameplayState::GetInstance());
	PushState(IntroState::GetInstance());
	IntroState::GetInstance()->Enter();

	// Store the starting time
	m_ulGameTime = GetTickCount();
	return true;	// success!
}

//*********************************************************************//
// Update
//	- update the SGD wrappers
//	- update & render the current state
int	Game::Update( void )
{
	// Try to update the wrappers
	if( SGD::GraphicsManager::GetInstance()->Update() == false 
		|| SGD::InputManager::GetInstance()->Update() == false 
		|| SGD::AudioManager::GetInstance()->Update() == false )
		return +1;	// exit when window is closed

	if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::Alt) && SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::Enter))
	{
		fullscreen = !fullscreen;
		if (fullscreen)
		{
			SGD::GraphicsManager::GetInstance()->Resize(m_szScreenSize, false);
		}
		else
		{
			SGD::GraphicsManager::GetInstance()->Resize(m_szScreenSize, true);
		}
		return 0;
	}
	
	// Calculate the elapsed time between frames
	unsigned long now = GetTickCount();
	float elapsedTime = (now - m_ulGameTime) / 1000.0f;
	m_ulGameTime = now;
	
	// Cap the elapsed time to 1/8th of a second
	if( elapsedTime > 0.125f )
		elapsedTime = 0.125f;

	stateStack.begin();
	// Update & Render the current state
	for (unsigned int i = 0; i < stateStack.size(); i++)
	{
		if (m_bPaused == false || i > 0)
		{
			int x = stateStack[i]->Update(elapsedTime);
			if (x == 2)
				return 2;	// exit success
			if (x == 3)
				return 0;
		}

		stateStack[i]->Render(elapsedTime);
	}


	return 0;		// keep running
}


//*********************************************************************//
// Terminate
//	- exit the current state
//	- terminate the SGD wrappers
void Game::Terminate( void )
{
	// Exit the current state
	ChangeState( nullptr );


	// Terminate & Deallocate the font
	if( m_pFont != nullptr )
	{
		m_pFont->Terminate();
		delete m_pFont;
	}


	// Terminate the SGD wrappers (in reverse order)
	SGD::AudioManager::GetInstance()->Terminate();
	SGD::AudioManager::DeleteInstance();
	
	SGD::InputManager::GetInstance()->Terminate();
	SGD::InputManager::DeleteInstance();
	
	SGD::GraphicsManager::GetInstance()->Terminate();
	SGD::GraphicsManager::DeleteInstance();
}


//*********************************************************************//
// ChangeState
//	- unload the old state
//	- load the new state
void Game::ChangeState( IGameState* pNextState )
{
	// Exit the current state (if it exists)
	ClearStates();

	// Store the new state
	stateStack.push_back(pNextState);

	// Enter the new state (if it exists)
	if (stateStack.front() != nullptr)
		stateStack.front()->Enter();
}

void Game::PushState(IGameState* pPushState)
{
	stateStack.push_back(pPushState);
}

void	Game::ClearStates()
{
	for (unsigned int i = 0; i < stateStack.size(); i++)
	{
		stateStack[i]->Exit();
	}
	stateStack.clear();
}

void Game::PopState()
{
	stateStack.pop_back();
}

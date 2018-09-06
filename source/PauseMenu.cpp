//*********************************************************************//
//	File:		PauseMenu.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Pauses the game
//*********************************************************************//

#include "PauseMenu.h"
#include "MainMenuState.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"


//*********************************************************************//
// GetInstance
//	- create & return THE singleton object
/*static*/ PauseMenu* PauseMenu::GetInstance(void)
{
	static PauseMenu s_Instance;

	return &s_Instance;
}


//*********************************************************************//
// Enter
//	- called EACH time the screen is shown/switched to
//	- load resources / reset data
/*virtual*/ void PauseMenu::Enter(void)		/*override*/
{
	// Reset the cursor to the top
	// (commented out to keep the last cursor position)
	//m_nCursor = 0;

	m_fEscapeTimer = 0.01f;
}

//*********************************************************************//
// Exit
//	- called EACH time the screen is hidden/switched from
//	- unload resources
/*virtual*/ void PauseMenu::Exit(void)		/*override*/
{
}


//*********************************************************************//
// Update
//	- called EVERY FRAME
//	- handle input & update
/*virtual*/ int PauseMenu::Update(float elapsedTime)	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	m_fEscapeTimer -= elapsedTime;
	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true && m_fEscapeTimer <= 0)
	{
		Game::GetInstance()->SetPausedState(false);
		Game::GetInstance()->PopState();
		// Exit this state immediately
		return 3;	// keep playing in the new state
	}



	// Move the cursor?
	if (pInput->IsKeyPressed(SGD::Key::Down) == true)
	{
		// next option
		m_nCursor++;

		// wrap around
		if (m_nCursor > 1)
			m_nCursor = 0;
	}
	else if (pInput->IsKeyPressed(SGD::Key::Up) == true)
	{
		// prev option
		m_nCursor--;

		// wrap around
		if (m_nCursor < 0)
			m_nCursor = 1;
	}


	// Select an option?
	if (pInput->IsKeyPressed(SGD::Key::Enter) == true)
	{
		// Which option is chosen?
		if (m_nCursor == 0)
		{
			// ChangeState is VERY VOLATILE!!!
			//	- can only be safely called by a game state's
			//	 Update or Render methods!
			//	GameplayState::GetInstance()->Exit();
			Game::GetInstance()->SetPausedState(false);
			Game::GetInstance()->PopState();
			// Exit this state immediately
			return 3;	// keep playing in the new state
		}
		if (m_nCursor == 1)
		{
			Game::GetInstance()->SetPausedState(false);
			Game::GetInstance()->SetMainMenuState(true);
			Game::GetInstance()->ChangeState(GameplayState::GetInstance());
			Game::GetInstance()->PushState(MainMenuState::GetInstance());
			return 3;
		}
			
		else //m_nCursor == 1
		{
			return 0;	// quit the game
		}
	}


	return 1;	// keep playing
}

//*********************************************************************//
// Render
//	- called EVERY FRAME
//	- draw menus / entities
/*virtual*/ void PauseMenu::Render(float elapsedTime)		/*override*/
{
	// Access the bitmap font
	BitmapFont* pFont = Game::GetInstance()->GetFont();


	// Align text based on window width
	float width = Game::GetInstance()->GetScreenSize().width;


	// Display the game title centered at 4x scale
	pFont->Draw("-Paused-", { (width - (4 * 31 * 4.0f)) / 2, 150 },
		1.0f, { 240, 255, 240 });

	// Display the menu options centered at 1x scale
	pFont->Draw("Continue", { (width - (4 * 72)) / 2, 300 },
		.6f, { 240, 255, 240 });
	pFont->Draw("Exit to Menu", { (width - (4 * 110)) / 2, 350 },
		.6f, { 240, 255, 240 });

	// Display the cursor next to the option
	pFont->Draw("#             #", { (width - (7 * 78)) / 2, 300.0f + 50 * m_nCursor },
		.6f, { 255, 255, 0 });
}

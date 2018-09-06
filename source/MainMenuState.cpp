//*********************************************************************//
//	File:		MainMenuState.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	MainMenuState class handles the main menu
//*********************************************************************//

#include "MainMenuState.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "HowToPlayState.h"
#include "CreditsState.h"
#include "OptionsState.h"
#include <iostream>
#include <fstream>


//*********************************************************************//
// GetInstance
//	- create & return THE singleton object
/*static*/ MainMenuState* MainMenuState::GetInstance( void )
{
	static MainMenuState s_Instance;

	return &s_Instance;
}


//*********************************************************************//
// Enter
//	- called EACH time the screen is shown/switched to
//	- load resources / reset data
/*virtual*/ void MainMenuState::Enter( void )		/*override*/
{
	// Reset the cursor to the top
	// (commented out to keep the last cursor position)
	//m_nCursor = 0;
	m_fAttractTimer = 10.0f;
	LoadSettings();
}

//*********************************************************************//
// Exit
//	- called EACH time the screen is hidden/switched from
//	- unload resources
/*virtual*/ void MainMenuState::Exit( void )		/*override*/
{
}


//*********************************************************************//
// Update
//	- called EVERY FRAME
//	- handle input & update
/*virtual*/ int MainMenuState::Update( float elapsedTime )	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (pInput->IsAnyKeyPressed())
	{
		m_fAttractTimer = 10;
	}
	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
		m_nCursor = 4;

	m_fAttractTimer -= elapsedTime;

	if (m_fAttractTimer <= 0)
	{
		m_bAttractState = true;
		Game::GetInstance()->PopState();
		Game::GetInstance()->PushState(HowToPlayState::GetInstance());
		HowToPlayState::GetInstance()->Enter();
		return 3;
	}

	// Move the cursor?
	if( pInput->IsKeyPressed( SGD::Key::Down ) == true )
	{
		// next option
		m_nCursor++;

		// wrap around
		if( m_nCursor > 4 )
			m_nCursor = 4;
	}
	else if( pInput->IsKeyPressed( SGD::Key::Up ) == true ) 
	{
		// prev option
		m_nCursor--;

		// wrap around
		if( m_nCursor < 0 )
			m_nCursor = 0;
	}


	// Select an option?
	if( pInput->IsKeyPressed( SGD::Key::Enter ) == true )
	{
		// Which option is chosen?
		if( m_nCursor == 0 )
		{
			// ChangeState is VERY VOLATILE!!!
			//	- can only be safely called by a game state's
			//	 Update or Render methods!
		//	GameplayState::GetInstance()->Exit();
			Game::GetInstance()->SetMainMenuState(false);
			Game::GetInstance()->ChangeState( GameplayState::GetInstance() );
			// Exit this state immediately
			return 3;	// keep playing in the new state
		}
		if (m_nCursor == 1)
		{
			Game::GetInstance()->PopState();
			Game::GetInstance()->PushState(OptionsState::GetInstance());
			OptionsState::GetInstance()->Enter();
			return 3;
		}
		if (m_nCursor == 2)
		{
			Game::GetInstance()->PopState();
			Game::GetInstance()->PushState(HowToPlayState::GetInstance());
			HowToPlayState::GetInstance()->Enter();
			return 3;
		}
		if (m_nCursor == 3)
		{
			Game::GetInstance()->PopState();
			Game::GetInstance()->PushState(CreditsState::GetInstance());
			CreditsState::GetInstance()->Enter();
			return 3;
		}

		if (m_nCursor == 4)
			return 2;
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
/*virtual*/ void MainMenuState::Render( float elapsedTime )		/*override*/
{
	// Access the bitmap font
	BitmapFont* pFont = Game::GetInstance()->GetFont();
	

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenSize().width;


	// Display the game title centered at 4x scale
	pFont->Draw( "Orc RUN!", { (width - (4 * 32 * 4.0f))/2, 150 }, 
				 1.0f, { 240, 255, 240 } );

	// Display the menu options centered at 1x scale
	pFont->Draw( "Play", { (width - (4 * 32))/2, 300 }, 
				 .6f, {240, 255, 240} );
	pFont->Draw("Options", { (width - (4 * 62)) / 2, 350 },
		.6f, { 240, 255, 240 });
	pFont->Draw("How To Play", { (width - (4 * 98)) / 2, 400 },
		.6f, { 240, 255, 240 });
	pFont->Draw("Credits", { (width - (4 * 62)) / 2, 450 },
		.6f, { 240, 255, 240 });
	pFont->Draw( "Exit", { (width - (4 * 32))/2, 500 }, 
				 .6f, {240, 255, 240} );




	switch (m_nCursor)
	{
	case 0:
		pFont->Draw("#", { (width - (7 * 32)) / 2, 300.0f + 50 * m_nCursor },
			.6f, { 255, 255, 0 });
		pFont->Draw("#", { (width - (7 * 32)) / 2 + 206, 300.0f + 50 * m_nCursor },
			.6f, { 255, 255, 0 });
		break;
	case 1:
		pFont->Draw("#", { (width - (7 * 32)) / 2 - 58, 300.0f + 50 * m_nCursor },
			.6f, { 255, 255, 0 });
		pFont->Draw("#", { (width - (7 * 32)) / 2 + 258, 300.0f + 50 * m_nCursor },
			.6f, { 255, 255, 0 });
		break;
	case 2:
		pFont->Draw("#", { (width - (7 * 32)) / 2 - 128, 300.0f + 50 * m_nCursor },
			.6f, { 255, 255, 0 });
		pFont->Draw("#", { (width - (7 * 32)) / 2 + 338, 300.0f + 50 * m_nCursor },
			.6f, { 255, 255, 0 });
		break;
	case 3:
		pFont->Draw("#", { (width - (7 * 32)) / 2 - 58, 300.0f + 50 * m_nCursor },
			.6f, { 255, 255, 0 });
		pFont->Draw("#", { (width - (7 * 32)) / 2 + 258, 300.0f + 50 * m_nCursor },
			.6f, { 255, 255, 0 });
		break;
	case 4:
		pFont->Draw("#", { (width - (7 * 32)) / 2, 300.0f + 50 * m_nCursor },
			.6f, { 255, 255, 0 });
		pFont->Draw("#", { (width - (7 * 32)) / 2 + 206, 300.0f + 50 * m_nCursor },
			.6f, { 255, 255, 0 });
		break;
	default:
		break;
	}


	// Display the cursor next to the option
	

}

void MainMenuState::LoadSettings()
{

	std::ifstream fin;

	fin.open("settings.txt");

	if (fin.is_open())
	{
		int x = 0;
		
		fin >> x;
		OptionsState::GetInstance()->SetMusicVol(x);
		fin >> x;
		OptionsState::GetInstance()->SetSFXVol(x);

		fin.close();

	}



}

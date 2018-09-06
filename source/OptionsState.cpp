//*********************************************************************//
//	File:		OptionsState.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Change Settings and Save to a Text File
//*********************************************************************//

#include "OptionsState.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "Game.h"
#include "BitmapFont.h"
#include "GameplayState.h"
#include "HowToPlayState.h"
#include "CreditsState.h"
#include "MainMenuState.h"
#include <string>
#include <iostream>
#include <fstream>

//*********************************************************************//
// GetInstance
//	- create & return THE singleton object
/*static*/ OptionsState* OptionsState::GetInstance(void)
{
	static OptionsState s_Instance;

	return &s_Instance;
}


//*********************************************************************//
// Enter
//	- called EACH time the screen is shown/switched to
//	- load resources / reset data
/*virtual*/ void OptionsState::Enter(void)		/*override*/
{
	// Reset the cursor to the top
	// (commented out to keep the last cursor position)
	m_nCursor = 0;
	m_fBufferInput = 0;

	m_hBackground = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/AJM_BackMusic.xwm");
	m_hOrcHit = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/AJM_orcDead1.wav");

}

//*********************************************************************//
// Exit
//	- called EACH time the screen is hidden/switched from
//	- unload resources
/*virtual*/ void OptionsState::Exit(void)		/*override*/
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hBackground);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hOrcHit);

}


//*********************************************************************//
// Update
//	- called EVERY FRAME
//	- handle input & update
/*virtual*/ int OptionsState::Update(float elapsedTime)	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	SGD::AudioManager::GetInstance()->SetAudioVolume(m_hOrcHit, m_nSFXVolume);
	m_fBufferInput -= elapsedTime;
	m_fSoundBuffer -= elapsedTime;

	if (m_nCursor == 6)
	{
		SGD::AudioManager::GetInstance()->SetAudioVolume(m_hBackground, OptionsState::GetInstance()->GetMusicVolume());
		if (!m_bMusicPlaying)
		{
			SGD::AudioManager::GetInstance()->PlayAudio(m_hBackground, true);
			m_bMusicPlaying = true;
		}
	}
	else
	{
		SGD::AudioManager::GetInstance()->StopAudio(m_hBackground);
		m_bMusicPlaying = false;
	}
	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
	{
		SaveSettings();
		Exit();
		Game::GetInstance()->PopState();
		Game::GetInstance()->PushState(MainMenuState::GetInstance());
		MainMenuState::GetInstance()->Enter();
		return 3;
	}

	// Move the cursor?
	if (pInput->IsKeyPressed(SGD::Key::Down) == true && m_nCursor < 6)
	{
		// next option
		m_nCursor++;

		// wrap around
		if (m_nCursor > 1 && m_nCursor < 6)
			m_nCursor = 4;
	}
	else if (pInput->IsKeyPressed(SGD::Key::Up) == true && m_nCursor < 6)
	{
		// prev option
		m_nCursor--;

		// wrap around
		if (m_nCursor == 3 || m_nCursor == 2)
			m_nCursor = 1;
		if (m_nCursor < 0)
			m_nCursor = 0;
	}


	if (pInput->IsKeyDown(SGD::Key::LeftArrow) == true && m_fBufferInput <= 0)
	{
		m_fBufferInput = .04f;
		if (m_nCursor == 6)
		{
			m_nMusicVolume--;
			if (m_nMusicVolume < 0)
				m_nMusicVolume = 0;
		}
		if (m_nCursor == 7)
		{
			m_nSFXVolume--;
			if (m_nSFXVolume < 0)
				m_nSFXVolume = 0;
			if (m_fSoundBuffer <= 0)
			{
				m_fSoundBuffer = .5f;
				SGD::AudioManager::GetInstance()->PlayAudio(m_hOrcHit, false);
			}
		}

	}
	if (pInput->IsKeyDown(SGD::Key::RightArrow) == true && m_fBufferInput <= 0)
	{
		m_fBufferInput = .04f;
		if (m_nCursor == 6)
		{
			m_nMusicVolume++;
			if (m_nMusicVolume > 100)
				m_nMusicVolume = 100;
		}
		if (m_nCursor == 7)
		{
			m_nSFXVolume++;
			if (m_nSFXVolume > 100)
				m_nSFXVolume = 100;
			if (m_fSoundBuffer <= 0)
			{
				m_fSoundBuffer = .5f;
				SGD::AudioManager::GetInstance()->PlayAudio(m_hOrcHit, false);
			}
		}
	}

	// Select an option?
	if (pInput->IsKeyPressed(SGD::Key::Enter) == true)
	{
		// Which option is chosen?
		if (m_nCursor == 0)
		{
			m_nCursor = 6;
		}
		else if (m_nCursor == 1)
		{
			m_nCursor = 7;
		}
		else if (m_nCursor == 6)
		{
			m_nCursor = 0;
		}
		else if (m_nCursor == 7)
		{
			m_nCursor = 1;
		}
		else if (m_nCursor == 4)
		{
			SaveSettings();
			Exit(); 
			Game::GetInstance()->PopState();
			Game::GetInstance()->PushState(MainMenuState::GetInstance());
			MainMenuState::GetInstance()->Enter();
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
/*virtual*/ void OptionsState::Render(float elapsedTime)		/*override*/
{
	// Access the bitmap font
	BitmapFont* pFont = Game::GetInstance()->GetFont();


	// Align text based on window width
	float width = Game::GetInstance()->GetScreenSize().width;


	// Display the game title centered at 4x scale
	pFont->Draw("Options", { (width - (4 * 32 * 4.0f)) / 2, 150 },
		1.0f, { 240, 255, 240 });

	// Display the menu options centered at 1x scale
	pFont->Draw("Music Vol", { (width - (4 * 162)) / 2, 300 },
		.5f, { 240, 255, 240 });
	std::string MusicVol = std::to_string(m_nMusicVolume);
	pFont->Draw(MusicVol.c_str(), SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 + 220, 300 }, .5f, SGD::Color{});
	
	pFont->Draw("Sound FX Vol", { (width - (4 * 162)) / 2, 350 },
		.5f, { 240, 255, 240 });
	std::string SFXVol = std::to_string(m_nSFXVolume);
	pFont->Draw(SFXVol.c_str(), SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 + 220, 350 }, .5f, SGD::Color{});

	
	pFont->Draw("Exit", { (width - (4 * 32)) / 2, 500 },
		.6f, { 240, 255, 240 });


	switch (m_nCursor)
	{
	case 0:
		pFont->Draw("#", { (width - (7 * 104)) / 2, 300.0f + 50 * m_nCursor },
			.6f, { 255, 255, 0 });
		pFont->Draw("#", { (width - (7 * 32)) / 2 + 84, 300.0f + 50 * m_nCursor },
			.6f, { 255, 255, 0 });
		break;
	case 1:
		pFont->Draw("#", { (width - (7 * 104)) / 2, 300.0f + 50 * m_nCursor },
			.6f, { 255, 255, 0 });
		pFont->Draw("#", { (width - (7 * 32)) / 2 + 180, 300.0f + 50 * m_nCursor },
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
	case 6:
		pFont->Draw("#", { (width - (7 * 32)) / 2 + 295, 300.0f + 50 * 0 },
			.6f, { 255, 255, 0 });
		pFont->Draw("#", { (width - (7 * 32)) / 2 + 435, 300.0f + 50 * 0 },
			.6f, { 255, 255, 0 });
		break;
	case 7:
		pFont->Draw("#", { (width - (7 * 32)) / 2 + 295, 300.0f + 50 * 1 },
			.6f, { 255, 255, 0 });
		pFont->Draw("#", { (width - (7 * 32)) / 2 + 435, 300.0f + 50 * 1 },
			.6f, { 255, 255, 0 });
		break;
	default:
		break;
	}


	// Display the cursor next to the option


}

void OptionsState::SaveSettings()
{

	std::ofstream fout;

	fout.open("settings.txt");

	if (fout.is_open())
	{

		fout << m_nMusicVolume;
		fout << "\n";
		fout << m_nSFXVolume;


		fout.close();
	}




}
//*********************************************************************//
//	File:		HowToPlayState.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Displays how to play
//*********************************************************************//


#include "HowToPlayState.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "Game.h"
#include "BitmapFont.h"
#include "MainMenuState.h"
#include "CreditsState.h"


//*********************************************************************//
// GetInstance
//	- create & return THE singleton object
/*static*/ HowToPlayState* HowToPlayState::GetInstance(void)
{
	static HowToPlayState s_Instance;
	return &s_Instance;
}


//*********************************************************************//
// Enter
//	- called EACH time the screen is shown/switched to
//	- load resources / reset data
/*virtual*/ void HowToPlayState::Enter(void)		/*override*/
{
	// Reset the cursor to the top
	// (commented out to keep the last cursor position)
	m_nCursor = 8;
	m_aAttractTimer = 5;

	if (m_cWalkAnim == nullptr)
	{
		SGD::HTexture walkImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/AJM_HeroWalkAdv.png");
		m_cWalkAnim = new CellAnimation;
		m_cWalkAnim->Initialize(200, 150, 5, 16, .03f, 1.0f, walkImage);
	}

	if (m_cFire == nullptr)
	{
		SGD::HTexture BowImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/AJM_HeroBow.png");
		m_cFire = new CellAnimation;
		m_cFire->Initialize(200, 150, 5, 16, .03f, 1.0f, BowImage);
	}
	if (m_nSlash == nullptr)
	{
		SGD::HTexture slashImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/AJM_HeroSlash.png");
		m_nSlash = new CellAnimation;
		m_nSlash->Initialize(200, 150, 5, 16, .03f, 1.0f, slashImage);
	}

	m_hOrc = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_OrcSword.png", SGD::Color{});
	m_hPauseBack = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_PauseBack.png", SGD::Color{});

	m_cWalkAnim->Restart(true, 1.0f);

}

//*********************************************************************//
// Exit
//	- called EACH time the screen is hidden/switched from
//	- unload resources
/*virtual*/ void HowToPlayState::Exit(void)		/*override*/
{
	if (m_cWalkAnim != nullptr)
	{
		m_cWalkAnim->Terminate();
		delete m_cWalkAnim;
		m_cWalkAnim = nullptr;
	}

	if (m_cFire != nullptr)
	{
		m_cFire->Terminate();
		delete m_cFire;
		m_cFire = nullptr;
	}
	if (m_nSlash != nullptr)
	{
		m_nSlash->Terminate();
		delete m_nSlash;
		m_nSlash = nullptr;
	}
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hOrc);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hPauseBack);

}


//*********************************************************************//
// Update
//	- called EVERY FRAME
//	- handle input & update
/*virtual*/ int HowToPlayState::Update(float elapsedTime)	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();
	if (m_cWalkAnim != nullptr && m_cWalkAnim->IsPlaying())
	{
		m_cWalkAnim->Update(elapsedTime);
	}
	if (m_nSlash != nullptr && m_nSlash->IsPlaying())
	{
		m_nSlash->Update(elapsedTime);
	}
	if (m_cFire != nullptr && m_cFire->IsPlaying())
	{
		m_cFire->Update(elapsedTime);
	}
	m_aAttractTimer -= elapsedTime;

	if (m_aAttractTimer <= 0 && MainMenuState::GetInstance()->GetAttractState() == true)
	{
		m_nHowtoState++;
		if (m_nHowtoState > 2)
		{
			Exit();
			Game::GetInstance()->PopState();
			Game::GetInstance()->PushState(CreditsState::GetInstance());
			CreditsState::GetInstance()->Enter();
			return 3;
		}
		m_aAttractTimer = 5;
	}
	if (pInput->IsAnyKeyPressed() && MainMenuState::GetInstance()->GetAttractState() == true)
	{
		Exit();
		MainMenuState::GetInstance()->SetAttractState(false);
		Game::GetInstance()->PopState();
		Game::GetInstance()->PushState(MainMenuState::GetInstance());
		MainMenuState::GetInstance()->Enter();
		return 3;
	}



	// Select an option?
	if (pInput->IsKeyPressed(SGD::Key::Enter) == true)
	{

		m_nHowtoState++;
		if (m_nHowtoState > 2)
		{
			Exit();
			Game::GetInstance()->PopState();
			Game::GetInstance()->PushState(MainMenuState::GetInstance());
			MainMenuState::GetInstance()->Enter();
			return 3;
		}
	}
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
	{
		Exit();
		Game::GetInstance()->PopState();
		Game::GetInstance()->PushState(MainMenuState::GetInstance());
		MainMenuState::GetInstance()->Enter();
		return 3;
	}

	m_fAnimtimer -= elapsedTime;

	if (m_fAnimtimer <= 0)
	{
		m_fAnimtimer = 3;
		m_nSlash->Restart(false, 1);
		m_cFire->Restart(false, 1);
	}
	return 1;	// keep playing
}

//*********************************************************************//
// Render
//	- called EVERY FRAME
//	- draw menus / entities
/*virtual*/ void HowToPlayState::Render(float elapsedTime)		/*override*/
{
	// Access the bitmap font
	BitmapFont* pFont = Game::GetInstance()->GetFont();




	// Align text based on window width
	float width = Game::GetInstance()->GetScreenSize().width;

	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hPauseBack, SGD::Point{ 0, 0 });
	// Display the game title centered at 4x scale
	pFont->Draw("How to Play!", { (width - (4 * 42 * 4.0f)) / 2, 50 },
		1.0f, { 240, 255, 240 });

	pFont->Draw("Press Enter", { (width - (4 * 32)) / 2, 700 },
		.6f, { 240, 255, 240 });
	
	if (m_nHowtoState == 0)
	{
		if (m_cWalkAnim != nullptr && m_cWalkAnim->IsPlaying())
		{
			m_cWalkAnim->Render(SGD::Point{ -15, 375 });
		}
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hOrc, SGD::Point{ 900, 425 }, 0.0f, SGD::Vector{}, SGD::Color{}, SGD::Size{ .4f, .4f });
		// Display the menu options centered at 1x scale
		pFont->Draw("Run this way", { (width - (4 * 188)) / 2, 460 },
			.4f, { 240, 255, 240 });
		pFont->Draw("$ $ $ $", { (width - (4 * 188)) / 2, 480 },
			.7f, { 240, 255, 240 });

		pFont->Draw("Enemies Approach", { (width - (4 * 2)) / 2 + 5, 490 },
			.4f, { 240, 255, 240 });
		pFont->Draw("% % % % %", { (width - (4 * 2)) / 2 + 5, 450 },
			.7f, { 240, 255, 240 });

		pFont->Draw("Controls:", { (width - (4 * 225)) / 2 + 5, 250 },
			.5f, { 240, 255, 240 });
		pFont->Draw("%A to move Left", { (width - (4 * 255)) / 2 + 5, 290 },
			.4f, { 240, 255, 240 });
		pFont->Draw("$D to move right", { (width - (4 * 255)) / 2 + 5, 330 },
			.4f, { 240, 255, 240 });

		pFont->Draw("Get as far as you can!", { (width - (4 * 135)) / 2 - 65, 580 },
			.5f, { 240, 255, 240 });
	}

	if (m_nHowtoState == 1)
	{

		if (m_nSlash != nullptr && m_nSlash->IsPlaying())
		{
			m_nSlash->Render(SGD::Point{ -15, 375 });
		}
		else
		if (m_cWalkAnim != nullptr && m_cWalkAnim->IsPlaying())
		{
			m_cWalkAnim->Render(SGD::Point{ -15, 375 });
		}
		pFont->Draw("Controls:", { (width - (4 * 225)) / 2 + 5, 250 },
			.5f, { 240, 255, 240 });
		pFont->Draw("Left Click to Slash", { (width - (4 * 255)) / 2 + 5, 300 },
			.4f, { 240, 255, 240 });

	}
	if (m_nHowtoState == 2)
	{

		if (m_cFire != nullptr && m_cFire->IsPlaying())
		{
			m_cFire->Render(SGD::Point{ -15, 375 });
		}
		else
		if (m_cWalkAnim != nullptr && m_cWalkAnim->IsPlaying())
		{
			m_cWalkAnim->Render(SGD::Point{ -15, 375 });
		}
		pFont->Draw("Controls:", { (width - (4 * 225)) / 2 + 5, 250 },
			.5f, { 240, 255, 240 });
		pFont->Draw("Hold Right Mouse to Pull BowString", { (width - (4 * 255)) / 2 + 5, 300 },
			.4f, { 240, 255, 240 });
		pFont->Draw("Release to Fire", { (width - (4 * 255)) / 2 + 5, 350 },
			.4f, { 240, 255, 240 });

	}
}

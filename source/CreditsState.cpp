//*********************************************************************//
//	File:		CreditsState.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Scrolls the Credits
//*********************************************************************//

#include "CreditsState.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "Game.h"
#include "BitmapFont.h"
#include "MainMenuState.h"


//*********************************************************************//
// GetInstance
//	- create & return THE singleton object
/*static*/ CreditsState* CreditsState::GetInstance(void)
{
	static CreditsState s_Instance;
	return &s_Instance;
}


//*********************************************************************//
// Enter
//	- called EACH time the screen is shown/switched to
//	- load resources / reset data
/*virtual*/ void CreditsState::Enter(void)		/*override*/
{
	// Reset the cursor to the top
	// (commented out to keep the last cursor position)
	m_nCursor = 8;
	m_fScroll = 900;
	if (m_cWalkAnim == nullptr)
	{
		SGD::HTexture walkImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/AJM_HeroWalkAdv.png");
		m_cWalkAnim = new CellAnimation;
		m_cWalkAnim->Initialize(200, 150, 5, 16, .03f, 1.0f, walkImage);
	}
	if (m_cOrcWalkAnim == nullptr)
	{
		SGD::HTexture walkImage2 = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/AJM_OrcSwordWalk.png");
		m_cOrcWalkAnim = new CellAnimation;
		m_cOrcWalkAnim->Initialize(200, 150, 5, 17, .03f, 1.0f, walkImage2);
	}
	if (m_cOrcWalkAnim2 == nullptr)
	{
		SGD::HTexture walkImage3 = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/AJM_OrcSwordWalk.png");
		m_cOrcWalkAnim2 = new CellAnimation;
		m_cOrcWalkAnim2->Initialize(200, 150, 5, 17, .01f, 1.0f, walkImage3);
	}
	m_hPauseBack = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_PauseBack.png", SGD::Color{});
	smallOrcPace = -150;
	m_cOrcWalkAnim2->Restart(true, 1);
	m_cOrcWalkAnim2->SetScale(.6f, .6f);
	m_cOrcWalkAnim->Restart(true, 1);
	m_cWalkAnim->Restart(true, 1.0f);
	OrcPOSX = 2200;
	smallOrcPOSX = 4900;
	heroPOSX = -825;
	m_aAttractTimer = 33.0f;
}

//*********************************************************************//
// Exit
//	- called EACH time the screen is hidden/switched from
//	- unload resources
/*virtual*/ void CreditsState::Exit(void)		/*override*/
{
	if (m_cWalkAnim != nullptr)
	{
		m_cWalkAnim->Terminate();
		delete m_cWalkAnim;
		m_cWalkAnim = nullptr;
	}
	if (m_cOrcWalkAnim != nullptr)
	{
		m_cOrcWalkAnim->Terminate();
		delete m_cOrcWalkAnim;
		m_cOrcWalkAnim = nullptr;
	}
	if (m_cOrcWalkAnim2 != nullptr)
	{
		m_cOrcWalkAnim2->Terminate();
		delete m_cOrcWalkAnim2;
		m_cOrcWalkAnim2 = nullptr;
	}
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hPauseBack);

}


//*********************************************************************//
// Update
//	- called EVERY FRAME
//	- handle input & update
/*virtual*/ int CreditsState::Update(float elapsedTime)	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	m_aAttractTimer -= elapsedTime;
	if (m_aAttractTimer <= 0 && MainMenuState::GetInstance()->GetAttractState() == true)
	{
			Exit();
			MainMenuState::GetInstance()->SetAttractState(false);
			Game::GetInstance()->PopState();
			Game::GetInstance()->PushState(MainMenuState::GetInstance());
			MainMenuState::GetInstance()->Enter();
			return 3;
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

	// Press Escape to quit
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true)
	{
		Exit();
		Game::GetInstance()->PopState();
		Game::GetInstance()->PushState(MainMenuState::GetInstance());
		MainMenuState::GetInstance()->Enter();
		return 3;
	}

	if (m_cWalkAnim != nullptr && m_cWalkAnim->IsPlaying())
	{
		m_cWalkAnim->Update(elapsedTime);
	}
	if (m_cOrcWalkAnim != nullptr && m_cOrcWalkAnim->IsPlaying())
	{
		m_cOrcWalkAnim->Update(elapsedTime);
	}
	if (m_cOrcWalkAnim2 != nullptr && m_cOrcWalkAnim2->IsPlaying())
	{
		m_cOrcWalkAnim2->Update(elapsedTime);
	}
	if (_runaway == false)
		heroPOSX += 80 * elapsedTime;
	else
		heroPOSX -= 130 * elapsedTime;

	OrcPOSX -= 90 * elapsedTime;
	smallOrcPOSX += smallOrcPace * elapsedTime;


	if (heroPOSX >= 400 && _runaway == false)
	{
		heroPOSX = 580;
	_runaway = true;
	m_cWalkAnim->SetScale(-1.0f, 1.0f);
	}
	if (m_fScroll <= -2900)
	{
		OrcPOSX = 2200;
		smallOrcPOSX = 2800;
		smallOrcPace = -90;
		//heroPOSX = -825;
		m_fScroll = 1500;
	}
	m_fScroll -= 110 * elapsedTime;
	return 1;	// keep playing
}

//*********************************************************************//
// Render
//	- called EVERY FRAME
//	- draw menus / entities
/*virtual*/ void CreditsState::Render(float elapsedTime)		/*override*/
{
	// Access the bitmap font
	BitmapFont* pFont = Game::GetInstance()->GetFont();

	if (m_cWalkAnim != nullptr && m_cWalkAnim->IsPlaying())
	{
		m_cWalkAnim->Render(SGD::Point{ heroPOSX, 375 });
	}
	//else if (m_cWalkAnim != nullptr && m_cWalkAnim->IsPlaying())
	//{
	//	m_cWalkAnim->Render(SGD::Point{ heroPOSX, 375 });
	//}

	for (int i = 0; i < 8; i++)
	{
		if (m_cOrcWalkAnim != nullptr && m_cOrcWalkAnim->IsPlaying())
		{
			
			m_cOrcWalkAnim->Render(SGD::Point{ (float)(i*75 + OrcPOSX), 375 }, 1.0f);
		}
	}
	if (m_cOrcWalkAnim2 != nullptr && m_cOrcWalkAnim2->IsPlaying())
	{

		m_cOrcWalkAnim2->Render(SGD::Point{ smallOrcPOSX, 435 });
	}

	SGD::GraphicsManager::GetInstance()->DrawTexture(m_hPauseBack, SGD::Point{ 0, 0 });

	// Align text based on window width
	float width = Game::GetInstance()->GetScreenSize().width;


	// Display the game title centered at 4x scale
	pFont->Draw("Credits", { (width - (4 * 12 * 4.0f)) / 2 - 50, m_fScroll },
		0.6f, { 240, 255, 240 });

	//PROGRAM Credits
	pFont->Draw("((((((((((((((((", { (width - (4 * 32 * 4.0f)) / 2 , m_fScroll + 270 },
		0.5f, { 240, 255, 240 });
	pFont->Draw("Programming ", { (width - (4 * 32 * 4.0f)) / 2 +75, m_fScroll + 300 },
		0.5f, { 240, 255, 240 });
	pFont->Draw("and", { (width - (4 * 32 * 4.0f)) / 2 + 205, m_fScroll + 340 },
		0.4f, { 240, 255, 240 });
	pFont->Draw("Game Design", { (width - (4 * 32 * 4.0f)) / 2 + 75, m_fScroll + 370 },
		0.5f, { 240, 255, 240 });
	pFont->Draw("Anthony J Mojica", { (width - (4 * 42 * 4.0f)) / 2 + 125, m_fScroll + 425 },
		.4f, { 240, 255, 240 });
	pFont->Draw("((((((((((((((((", { (width - (4 * 32 * 4.0f)) / 2, m_fScroll + 445 },
		0.5f, { 240, 255, 240 });


	//ART Credits
	pFont->Draw("((((((((((((((((", { (width - (4 * 32 * 4.0f)) / 2, m_fScroll + 750 },
		0.5f, { 240, 255, 240 });
	pFont->Draw("Art Design ", { (width - (4 * 32 * 4.0f)) / 2 + 75, m_fScroll + 780 },
		0.5f, { 240, 255, 240 });
	pFont->Draw("and", { (width - (4 * 32 * 4.0f)) / 2 + 205, m_fScroll + 820 },
		0.4f, { 240, 255, 240 });
	pFont->Draw("Animations", { (width - (4 * 32 * 4.0f)) / 2 + 75, m_fScroll + 850 },
		0.5f, { 240, 255, 240 });
	pFont->Draw("Edward Frosch", { (width - (4 * 42 * 4.0f)) / 2 + 155, m_fScroll + 905 },
		.4f, { 240, 255, 240 });
	pFont->Draw("((((((((((((((((", { (width - (4 * 32 * 4.0f)) / 2, m_fScroll + 925 },
		0.5f, { 240, 255, 240 });

	//Audio
	pFont->Draw("((((((((((((((((", { (width - (4 * 32 * 4.0f)) / 2, m_fScroll + 1230 },
		0.5f, { 240, 255, 240 });
	pFont->Draw("Game Audio", { (width - (4 * 32 * 4.0f)) / 2 + 75, m_fScroll + 1260 },
		0.5f, { 240, 255, 240 });
	pFont->Draw("Quin Imeson", { (width - (4 * 32 * 4.0f)) / 2 + 85, m_fScroll + 1300 },
		0.4f, { 240, 255, 240 });
	pFont->Draw("Anthony J Mojica", { (width - (4 * 42 * 4.0f)) / 2 + 105, m_fScroll + 1330 },
		.4f, { 240, 255, 240 });
	pFont->Draw("((((((((((((((((", { (width - (4 * 32 * 4.0f)) / 2, m_fScroll + 1350 },
		0.5f, { 240, 255, 240 });
	// Display the cursor next to the option


	//BM Font
	pFont->Draw("((((((((((((((((", { (width - (4 * 32 * 4.0f)) / 2, m_fScroll + 1530 },
		0.5f, { 240, 255, 240 });
	pFont->Draw("PlayTester and", { (width - (4 * 32 * 4.0f)) / 2 + 72, m_fScroll + 1560 },
		0.4f, { 240, 255, 240 });
	pFont->Draw("Unibrow Design", { (width - (4 * 32 * 4.0f)) / 2 + 59, m_fScroll + 1590 },
		0.4f, { 240, 255, 240 });
	pFont->Draw("Steven Martinez", { (width - (4 * 32 * 4.0f)) / 2 + 55, m_fScroll + 1620 },
		0.4f, { 240, 255, 240 });
	pFont->Draw("((((((((((((((((", { (width - (4 * 32 * 4.0f)) / 2, m_fScroll + 1640 },
		0.5f, { 240, 255, 240 });

	pFont->Draw("((((((((((((((((", { (width - (4 * 32 * 4.0f)) / 2, m_fScroll + 1900 },
		0.5f, { 240, 255, 240 });
	pFont->Draw("BitMap Font", { (width - (4 * 32 * 4.0f)) / 2 + 75, m_fScroll + 1940 },
		0.5f, { 240, 255, 240 });
	pFont->Draw("OpenGameArt.org", { (width - (4 * 32 * 4.0f)) / 2 + 55, m_fScroll + 1980 },
		0.4f, { 240, 255, 240 });
	pFont->Draw("((((((((((((((((", { (width - (4 * 32 * 4.0f)) / 2, m_fScroll + 2000 },
		0.5f, { 240, 255, 240 });

	pFont->Draw("((((((((((((((((", { (width - (4 * 32 * 4.0f)) / 2, m_fScroll + 2200 },
		0.5f, { 240, 255, 240 });
	pFont->Draw("Structure of Game Design", { (width - (4 * 32 * 4.0f)) / 2 - 95, m_fScroll + 2240 },
		0.5f, { 240, 255, 240 });
	pFont->Draw("at Full Sail University", { (width - (4 * 32 * 4.0f)) / 2 - 35, m_fScroll + 2280 },
		0.4f, { 240, 255, 240 });
	pFont->Draw("((((((((((((((((", { (width - (4 * 32 * 4.0f)) / 2, m_fScroll + 2300 },
		0.5f, { 240, 255, 240 });

}

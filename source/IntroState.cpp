//*********************************************************************//
//	File:		IntroState.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Intro Screen
//*********************************************************************//

#include "IntroState.h"
#include "MainMenuState.h"

#include "../SGD Wrappers/SGD_InputManager.h"
#include "Game.h"
#include "BitmapFont.h"



//*********************************************************************//
// GetInstance
//	- create & return THE singleton object
/*static*/ IntroState* IntroState::GetInstance(void)
{
	static IntroState s_Instance;

	return &s_Instance;
}


//*********************************************************************//
// Enter
//	- called EACH time the screen is shown/switched to
//	- load resources / reset data
/*virtual*/ void IntroState::Enter(void)		/*override*/
{
	// Reset the cursor to the top
	// (commented out to keep the last cursor position)
	//m_nCursor = 0;
	m_fAttractTimer = 6.0f;
	if (m_cWalkAnim == nullptr)
	{
		SGD::HTexture walkImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/AJM_HeroWalkAdv.png");
		m_cWalkAnim = new CellAnimation;
		m_cWalkAnim->Initialize(200, 150, 5, 16, .01f, 1.0f, walkImage);
	}
	HeroPOSX = -300;
	m_fTextheight = 465;
	m_cWalkAnim->Restart(true, 1.0f);
}

//*********************************************************************//
// Exit
//	- called EACH time the screen is hidden/switched from
//	- unload resources
/*virtual*/ void IntroState::Exit(void)		/*override*/
{
	if (m_cWalkAnim != nullptr)
	{
		m_cWalkAnim->Terminate();
		delete m_cWalkAnim;
		m_cWalkAnim = nullptr;
	}
}


//*********************************************************************//
// Update
//	- called EVERY FRAME
//	- handle input & update
/*virtual*/ int IntroState::Update(float elapsedTime)	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();

	if (m_cWalkAnim != nullptr && m_cWalkAnim->IsPlaying())
	{
		m_cWalkAnim->Update(elapsedTime);
	}

	if (pInput->IsAnyKeyPressed())
	{
		Exit();
		Game::GetInstance()->PopState();
		Game::GetInstance()->PushState(MainMenuState::GetInstance());
		MainMenuState::GetInstance()->Enter();
		return 3;
	}

	m_fAttractTimer -= elapsedTime;

	if (m_fAttractTimer <= 0)
	{
		Exit();
		Game::GetInstance()->PopState();
		Game::GetInstance()->PushState(MainMenuState::GetInstance());
		MainMenuState::GetInstance()->Enter();
		return 3;
	}
	if (HeroPOSX >= 800 && m_fTextheight > 180)
	{
		m_fTextheight -= 250 * elapsedTime;
	}
	if (HeroPOSX > 900 && m_fSwap == false)
	{
		m_fSwap = true;
		HeroPOSX += 200;
		m_cWalkAnim->SetScale(-1.0f, 1.0f);
	}


	if (m_fSwap == false)
		HeroPOSX += 400 * elapsedTime;
	else
		HeroPOSX -= 400 * elapsedTime;

	return 1;	// keep playing

}

//*********************************************************************//
// Render
//	- called EVERY FRAME
//	- draw menus / entities
/*virtual*/ void IntroState::Render(float elapsedTime)		/*override*/
{
	// Access the bitmap font
	BitmapFont* pFont = Game::GetInstance()->GetFont();


	// Align text based on window width
	float width = Game::GetInstance()->GetScreenSize().width;


	if (m_cWalkAnim != nullptr && m_cWalkAnim->IsPlaying())
	{
		m_cWalkAnim->Render(SGD::Point{ HeroPOSX, 375 });
	}

	// Display the game title centered at 4x scale
	if (HeroPOSX >= 300 && (HeroPOSX < 1300 || HeroPOSX >1325) || m_fSwap)
		pFont->Draw("O", { (width - (4 * 22 * 4.0f)) / 2, m_fTextheight },
		1.0f, { 240, 255, 240 });
	if (HeroPOSX >= 350 && (HeroPOSX < 1350 || HeroPOSX >1375) || m_fSwap)
		pFont->Draw("R", { (width - (4 * 22 * 4.0f)) / 2 + 50, m_fTextheight },
		1.0f, { 240, 255, 240 });		
	if (HeroPOSX >= 400 && (HeroPOSX < 1400 || HeroPOSX >1425) || m_fSwap)
		pFont->Draw("C", { (width - (4 * 22 * 4.0f)) / 2 + 100, m_fTextheight },
		1.0f, { 240, 255, 240 });	
	if (HeroPOSX >= 450 && (HeroPOSX < 1450 || HeroPOSX >1475) || m_fSwap)
		pFont->Draw(" ", { (width - (4 * 22 * 4.0f)) / 2 + 150, m_fTextheight },
		1.0f, { 240, 255, 240 });	
	if (HeroPOSX >= 500 && (HeroPOSX < 1500 || HeroPOSX >1525) || m_fSwap)
		pFont->Draw("R", { (width - (4 * 22 * 4.0f)) / 2 + 200, m_fTextheight },
		1.0f, { 240, 255, 240 });	
	if (HeroPOSX >= 550 && (HeroPOSX < 1550 || HeroPOSX >1575) || m_fSwap)
		pFont->Draw("U", { (width - (4 * 22 * 4.0f)) / 2 + 250, m_fTextheight },
		1.0f, { 240, 255, 240 });	
	if (HeroPOSX >= 600 && (HeroPOSX < 1600 || HeroPOSX >1625) || m_fSwap)
		pFont->Draw("N", { (width - (4 * 22 * 4.0f)) / 2 + 300, m_fTextheight },
		1.0f, { 240, 255, 240 });	
	if (HeroPOSX >= 650 && (HeroPOSX < 1650 || HeroPOSX >1675) || m_fSwap)
		pFont->Draw("!", { (width - (4 * 22 * 4.0f)) / 2 + 350, m_fTextheight },
		1.0f, { 240, 255, 240 });



	// Display the cursor next to the option


}
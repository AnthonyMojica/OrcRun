//*********************************************************************//
//	File:		Bush.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Entity, Displays a Bush.
//*********************************************************************//

#include "Bush.h"
#include "Tree.h"

#include "../SGD Wrappers/SGD_Utilities.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Event.h"


#include <Windows.h>
#include <ctime>
#include <string>
#include "BitmapFont.h"
#include "GameplayState.h"
#include "Game.h"

Bush::Bush()
{
	// Seed First!
	srand((unsigned int)time(nullptr));
	rand();
	m_fBushScale = .7f;
	//m_fBushScale = (float)(((rand() % 10) * .1) + .2f);

}


Bush::~Bush()
{
}

/*virtual*/ void	Bush::Render(void)
{

	// Validate the image
	SGD_ASSERT(m_hImage != SGD::INVALID_HANDLE,
		"Entity::Render - image was not set!");

	// HACK: rotate
	//m_fRotation += 0.001f;
	if (GameplayState::GetInstance()->GetDebug())
	{
		SGD::GraphicsManager::GetInstance()->DrawRectangle(GetRect(), SGD::Color{ 255, 0, 0 });
		std::string Info = std::to_string(m_vtVelocity.x);
		Game::GetInstance()->GetFont()->Draw("X Velocity", SGD::Point{ m_ptPosition.x, m_ptPosition.y }, .2f, SGD::Color{});
		Game::GetInstance()->GetFont()->Draw(Info.c_str(), SGD::Point{ m_ptPosition.x, m_ptPosition.y + 10.0f }, .2f, SGD::Color{});
		Info = std::to_string(m_vtVelocity.y);
		Game::GetInstance()->GetFont()->Draw("Y Velocity", SGD::Point{ m_ptPosition.x, m_ptPosition.y + 20.0f }, .2f, SGD::Color{});
		Game::GetInstance()->GetFont()->Draw(Info.c_str(), SGD::Point{ m_ptPosition.x, m_ptPosition.y + 30.0f }, .2f, SGD::Color{});
	}
	// Draw the image


	SGD::GraphicsManager::GetInstance()->DrawTexture(
		m_hImage, m_ptPosition,
		m_fRotation, m_szSize / 2, SGD::Color{}, SGD::Size{ m_fBushScale, m_fBushScale });

}

void	Bush::HandleEvent(const SGD::Event* pEvent) 	// Callback function to process events
{

}
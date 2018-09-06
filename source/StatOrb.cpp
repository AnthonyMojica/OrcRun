//*********************************************************************//
//	File:		StatOrb.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Entity, Displays Player Stats.
//*********************************************************************//

#include "StatOrb.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Utilities.h"
#include "../SGD Wrappers/SGD_Event.h"

#include <string>
#include "BitmapFont.h"
#include "GameplayState.h"
#include "Game.h"
StatOrb::StatOrb()
{
	IListener::RegisterForEvent("HEALTH_LOSS");
	IListener::RegisterForEvent("STAMINA_LOSS");
	IListener::RegisterForEvent("HEALTH_GAIN");
	IListener::RegisterForEvent("STAMINA_GAIN");
	IListener::RegisterForEvent("ARROW_FIRED");
	IListener::RegisterForEvent("ARROW_PICKUP");
	IListener::RegisterForEvent("ARROW_HIT_HERO");
	_reloadtimer = 0;
}


StatOrb::~StatOrb()
{


}

void StatOrb::Update(float elapsedTime)
{
	_reloadtimer -= elapsedTime;
	m_fPickupTimer -= elapsedTime;

}
/*virtual*/ void StatOrb::Render(void)
{
	// Validate the image
	SGD_ASSERT(m_hImage != SGD::INVALID_HANDLE,
		"Entity::Render - image was not set!");

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

	SGD::Point offset = SGD::Point{ m_ptPosition.x, m_ptPosition.y };

	for (int i = 0; i < m_nCurrentHealth; i++)
	{
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_hImage, offset, 
																SGD::Rectangle{ 0, 0, 32, 32 }, 
																m_fRotation, m_szSize / 2);
		offset.x += 35;
	}
	offset.y += 85;
	offset.x = m_ptPosition.x;
	for (int i = 0; i < m_nCurrentStamina; i++)
	{
		SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_hImage, offset,
																SGD::Rectangle{ 32, 0, 64, 32 },
																m_fRotation, m_szSize / 2);
		offset.x += 35;
	}
	offset.y -= 15;
	offset.x = m_ptPosition.x + 220;
	for (int i = 0; i < m_nCurrentArrows; i++)
	{
		if (i + 1 == m_nCurrentArrows && _reloadtimer <= 0)
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_hImage, SGD::Point{ 339, 603 },
			SGD::Rectangle{ 64, 0, 128, 32 },-.83f,	m_szSize);
		else
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_hImage, offset,
			SGD::Rectangle{ 64, 0, 128, 32 }, -1.0f, m_szSize);
		offset.x += 25;
	}

}

void	StatOrb::HandleEvent(const SGD::Event* pEvent) 	// Callback function to process events
{


	if (pEvent->GetEventID() == "HEALTH_LOSS")
	{
		m_nCurrentHealth--;
	}
	if (pEvent->GetEventID() == "ARROW_HIT_HERO")
	{
		m_nCurrentHealth--;
	}
	if (pEvent->GetEventID() == "STAMINA_LOSS")
	{
		m_nCurrentStamina--;
	}
	if (pEvent->GetEventID() == "HEALTH_GAIN" && m_nCurrentHealth < m_nMaxHealth)
	{
		m_nCurrentHealth++;
	}
	if (pEvent->GetEventID() == "STAMINA_GAIN" && m_nCurrentStamina < m_nMaxStamina)
	{
		m_nCurrentStamina++;
	}
	if (pEvent->GetEventID() == "ARROW_FIRED")
	{
		m_nCurrentArrows--;
		_reloadtimer = 1.0f;
	}
	if (pEvent->GetEventID() == "ARROW_PICKUP" && m_fPickupTimer <= 0)
	{
		m_fPickupTimer = .5f;
		if (m_nCurrentArrows < m_nMaxArrows)
		m_nCurrentArrows++;
	}
}
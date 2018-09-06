//*********************************************************************//
//	File:		OrcBow.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Character, Bow Enemy.
//*********************************************************************//

#include "OrcBow.h"
#include "CreateOrcArrowMsg.h"

#include "../SGD Wrappers/SGD_Utilities.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include <string>
#include "BitmapFont.h"
#include "GameplayState.h"
#include "Game.h"

OrcBow::OrcBow()
{
	if (m_cWalkAnim == nullptr)
	{
		SGD::HTexture walkImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/AJM_OrcBowWalk.png");
		m_cWalkAnim = new CellAnimation;
		m_cWalkAnim->Initialize(200, 150, 5, 17, .03f, 1.0f, walkImage);
	}
	if (m_cBowAnim == nullptr)
	{
		SGD::HTexture bowImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/AJM_OrcBowfire.png");
		m_cBowAnim = new CellAnimation;
		m_cBowAnim->Initialize(200, 150, 5, 23, .02f, 1.0f, bowImage);
	}
	m_cWalkAnim->Restart(true, 1);
}


OrcBow::~OrcBow()
{
	if (m_cWalkAnim != nullptr)
	{
		m_cWalkAnim->Terminate();
		delete m_cWalkAnim;
		m_cWalkAnim = nullptr;
	}
	if (m_cBowAnim != nullptr)
	{
		m_cBowAnim->Terminate();
		delete m_cBowAnim;
		m_cBowAnim = nullptr;
	}
}

/*virtual*/ void	OrcBow::Update(float elapsedTime)
{

	if (m_cBowAnim != nullptr && m_cBowAnim->IsPlaying())
	{
		m_cBowAnim->Update(elapsedTime);
	}
	else if (m_cWalkAnim != nullptr && m_cWalkAnim->IsPlaying())
	{
		m_cWalkAnim->Update(elapsedTime);
	}

	m_fPull = (GetPosition().x - GetOwner()->GetPosition().x) * 0.00112f;
	m_fFireTimer -= elapsedTime;
	if (m_fFireTimer <= 0 && isAlive && m_fireSystem)
	{
		m_fFireTimer = 2.3f;
		m_fireSystem = !m_fireSystem;
		CreateOrcArrowMsg* newArrow = new CreateOrcArrowMsg(this);
		newArrow->QueueMessage();
		newArrow = nullptr;
		m_vtVelocity.x = -110;
	}

	if (m_fFireTimer <= 0 && isAlive && !m_fireSystem)
	{
		m_fFireTimer = .5f;
		m_fireSystem = !m_fireSystem;
		m_cBowAnim->Restart(false, 1);
		m_vtVelocity.x = -30;
	}

	Orc::Update(elapsedTime);
}
/*virtual*/ void	OrcBow::Render(void)
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
		Game::GetInstance()->GetFont()->Draw("X Velocity", SGD::Point{ m_ptPosition.x, m_ptPosition.y - 50.0f }, .2f, SGD::Color{});
		Game::GetInstance()->GetFont()->Draw(Info.c_str(), SGD::Point{ m_ptPosition.x, m_ptPosition.y - 40.0f }, .2f, SGD::Color{});
		Info = std::to_string(m_vtVelocity.y);
		Game::GetInstance()->GetFont()->Draw("Y Velocity", SGD::Point{ m_ptPosition.x, m_ptPosition.y - 30.0f }, .2f, SGD::Color{});
		Game::GetInstance()->GetFont()->Draw(Info.c_str(), SGD::Point{ m_ptPosition.x, m_ptPosition.y - 20.0f }, .2f, SGD::Color{});
	}
	// Draw the image

	SGD::Point offsetSize = SGD::Point{ (m_ptPosition.x - 39.0f), (m_ptPosition.y - 59.0f) };
	SGD::Point offsetSize2 = SGD::Point{ (m_ptPosition.x - 65.0f), (m_ptPosition.y - 85.0f) };
	if (m_cBowAnim != nullptr && m_cBowAnim->IsPlaying() && isAlive)
	{
		m_cBowAnim->Render(offsetSize2);
	}
	else if (m_cWalkAnim != nullptr && m_cWalkAnim->IsPlaying() && isAlive)
	{
		m_cWalkAnim->Render(offsetSize2);
	}
	else
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(
			m_hImage, offsetSize,
			m_fRotation, m_szSize * 2.3f, SGD::Color{}, SGD::Size{ .4f, .4f });
	}
}
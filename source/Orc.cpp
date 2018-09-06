//*********************************************************************//
//	File:		Orc.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Character, Sword Enemy.
//*********************************************************************//

#include "Orc.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Utilities.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "../SGD Wrappers/SGD_IListener.h"
#include "AddScoreMessage.h"
#include "DestroyEntityMessage.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "Game.h"
#include "GameplayState.h"
#include "Arrow.h"
#include "OptionsState.h"
#include <string>
#include "BitmapFont.h"

Orc::Orc()
{
	int x = GetType();
	//m_nMaxHealth = 1;
	m_nHealth = 1;
	IListener::RegisterForEvent("ARROW_HIT");
	m_hOrcDeath1 = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/AJM_orcDead1.wav");
	m_hOrcDeath2 = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/AJM_orcDead2.wav");

	SGD::AudioManager::GetInstance()->SetAudioVolume(m_hOrcDeath1, OptionsState::GetInstance()->GetSFXVolume());
	SGD::AudioManager::GetInstance()->SetAudioVolume(m_hOrcDeath2, OptionsState::GetInstance()->GetSFXVolume());

	if (m_cWalkAnim == nullptr)
	{
		SGD::HTexture walkImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/AJM_OrcSwordWalk.png");
		m_cWalkAnim = new CellAnimation;
		m_cWalkAnim->Initialize(200, 150, 5, 17, .03f, 1.0f, walkImage);
	}
	if (m_cSlashAnim == nullptr)
	{
		SGD::HTexture slashImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/AJM_OrcSlash.png");
		m_cSlashAnim = new CellAnimation;
		m_cSlashAnim->Initialize(200, 150, 5, 19, 0.02f, 1.0f, slashImage);
	}

	m_cWalkAnim->Restart(true, 1);
}


Orc::~Orc()
{
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hOrcDeath1);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hOrcDeath2);
	SetOwner(nullptr);
	if (m_cWalkAnim != nullptr)
	{
		m_cWalkAnim->Terminate();
		delete m_cWalkAnim;
		m_cWalkAnim = nullptr;
	}
	if (m_cSlashAnim != nullptr)
	{
		m_cSlashAnim->Terminate();
		delete m_cSlashAnim;
		m_cSlashAnim = nullptr;
	}

}

/*virtual*/ void	Orc::Update(float elapsedTime)	
{

	if (m_cSlashAnim != nullptr && m_cSlashAnim->IsPlaying())
	{
		m_cSlashAnim->Update(elapsedTime);
	}
	else if (m_cWalkAnim != nullptr && m_cWalkAnim->IsPlaying())
	{
		m_cWalkAnim->Update(elapsedTime);
	}

	if (m_nHealth == 0 && isAlive)
	{
		if ((int)(m_ptPosition.x) % 2 == 0)
		SGD::AudioManager::GetInstance()->PlayAudio(m_hOrcDeath1);
		else 
		SGD::AudioManager::GetInstance()->PlayAudio(m_hOrcDeath2);
		isAlive = false;
		m_vtVelocity.x += 50;

		m_vtVelocity.y -= 275;
		
	}

	if (m_ptPosition.x <= m_eOwner->GetPosition().x + 20 && isAlive && DamageDone == false)
	{
		SGD::Event* attackEvent = new SGD::Event{"HEALTH_LOSS", nullptr, this };
		attackEvent->QueueEvent();
		DamageDone = true;
	}

	if (m_ptPosition.y < 460)
	{
		_beginFalling = true;
	}

	if (_beginFalling)
	{
		m_vtVelocity.y += 675 * elapsedTime;
		m_fRotation += 2.8f * elapsedTime ;
	}
	m_ptPosition += m_vtVelocity * Game::GetInstance()->GetSpeedVar() * elapsedTime;

	_hitTimer -= elapsedTime;

	//Deletes Objects once they are off screen enough - a bit of 
	//a rough estimate to account for objects with improper size to image ratio
	if (m_ptPosition.x < -(m_szSize.width + 450))
	{
		DestroyEntityMessage* DestroyIt = new DestroyEntityMessage(this);
		DestroyIt->QueueMessage();
		DestroyIt = nullptr;
	}

	if (GetPosition().x < GetOwner()->GetPosition().x + 80 && m_bSlashDone == false)
	{
		m_cSlashAnim->Restart(false, 1.0f);
		m_bSlashDone = true;
	}



}
/*virtual*/ void	Orc::Render(void)
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
	if (m_cSlashAnim != nullptr && m_cSlashAnim->IsPlaying() && isAlive)
	{
		m_cSlashAnim->Render(offsetSize2);
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

void	Orc::HandleEvent(const SGD::Event* pEvent) 	// Callback function to process events
{

	Arrow* tempEntity = reinterpret_cast<Arrow*>(pEvent->GetSender());

	if (pEvent->GetEventID() == "ARROW_HIT" && tempEntity->GetOwner()->GetType() == ENT_HERO)
	{
		m_nHealth--;
		if (m_nHealth >= 0)
		{
			AddScoreMessage* moreScore = new AddScoreMessage(this);
			moreScore->QueueMessage();
			moreScore = nullptr;
		}
		if (_hitTimer <= 0)
		{
			GameplayState::GetInstance()->AddHitCount(1);
			_hitTimer = .5f;
		}
	}
}

/*virtual*/ void Orc::HandleCollision(const IEntity* pOther)	/*override*/
{
	if (pOther->GetType() == ENT_HERO)
	{
		m_nHealth--;
		if (m_nHealth >= 0)
		{
		AddScoreMessage* moreScore = new AddScoreMessage(this);
		moreScore->QueueMessage();
		moreScore = nullptr;
		}
	}
}
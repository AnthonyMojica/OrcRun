//*********************************************************************//
//	File:		Hero.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Character, Main Player Character.
//*********************************************************************//

#include "Hero.h"
#include "../SGD Wrappers/SGD_Utilities.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "DestroyEntityMessage.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "CreateArrowMessage.h"
#include "CreateOrcMessage.h"
#include "SlashMessage.h"
#include "Game.h"
#include "../SGD Wrappers/SGD_AudioManager.h"
#include "GameOverMessage.h"
#include "GameplayState.h"
#include "OptionsState.h"
#include "BitmapFont.h"
#include <string>

Hero::Hero()
{
	int x = GetType();
	// Allocate & start the animation
	if (m_cSlashAnim == nullptr)
	{
		SGD::HTexture slashImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/AJM_HeroSlash.png");
		m_cSlashAnim = new CellAnimation;
		m_cSlashAnim->Initialize(200, 150, 5,19, 0.01f, 1.0f, slashImage);
	}

	if (m_cWalkAnim == nullptr)
	{
		SGD::HTexture walkImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/AJM_HeroWalkAdv.png");
		m_cWalkAnim = new CellAnimation;
		m_cWalkAnim->Initialize(200, 150, 5, 16, .03f, 1.0f, walkImage);

	}
	if (m_cBowAnim == nullptr)
	{
		SGD::HTexture bowImage = SGD::GraphicsManager::GetInstance()->LoadTexture(
			L"resource/graphics/AJM_HeroBow.png");
		m_cBowAnim = new CellAnimation;
		m_cBowAnim->Initialize(200, 150, 5, 23, .06f, 1.0f, bowImage);

	}
	SGD::AudioManager* pAudio = SGD::AudioManager::GetInstance(); 
	OptionsState* pOptions = OptionsState::GetInstance();

	m_hFire1 = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/AJM_Bowshot1.wav");
	m_hFire2 = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/AJM_Bowshot2.wav");
	m_hFire3 = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/AJM_Bowshot3.wav");
	m_hDeath = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/AJM_death 1.wav");
	m_hHit1 = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/AJM_Hit 1.wav");
	m_hHit2 = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/AJM_Hit 2.wav");
	
	pAudio->SetAudioVolume(m_hFire1, pOptions->GetSFXVolume());
	pAudio->SetAudioVolume(m_hFire2, pOptions->GetSFXVolume());
	pAudio->SetAudioVolume(m_hFire3, pOptions->GetSFXVolume());
	pAudio->SetAudioVolume(m_hDeath, pOptions->GetSFXVolume());
	pAudio->SetAudioVolume(m_hHit1, pOptions->GetSFXVolume());
	pAudio->SetAudioVolume(m_hHit2, pOptions->GetSFXVolume());

//	m_cSlashAnim->Restart(false, 1.0f);
	//m_nMaxHealth = m_nHealth = m_nStamina = m_nMaxStamina = 4;
	IListener::RegisterForEvent("HEALTH_LOSS");
	IListener::RegisterForEvent("ARROW_PICKUP");
	IListener::RegisterForEvent("ARROW_HIT_HERO");
	m_nStamina = 4;
	m_nHealth = 4;
	m_nMaxStamina = 4;
	m_nMaxHealth = 4;
}


Hero::~Hero()
{
	if (m_cSlashAnim != nullptr)
	{
		m_cSlashAnim->Terminate();
		delete m_cSlashAnim;
		m_cSlashAnim = nullptr;
	}

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
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hFire1);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hFire2);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hFire3);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hDeath);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hHit1);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hHit2);
	

}

/*virtual*/ void	Hero::Update(float elapsedTime)	
{

	if (GetPosition().x > 200)
		Game::GetInstance()->SetSpeedVar(1.7f);
	else
		Game::GetInstance()->SetSpeedVar(1.1f);


	//Movement
	if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::D) && isAlive && GetPosition().x < Game::GetInstance()->GetScreenSize().width - 150)
	{
		m_ptPosition.x += 125 * elapsedTime;
		m_cWalkAnim->SetFrameSpeed(.02f);
	}
	if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::A) && isAlive && GetPosition().x > -40)
	{
		m_ptPosition.x -= 125 * elapsedTime;
		m_cWalkAnim->SetFrameSpeed(.05f);
	}

	//KILLS THE PLAYER - FOR TESTING
	//if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::G) && m_nStamina > 0 && isAlive)
	//{
	//	m_nHealth--;
	//}
	//Bow and Arrow
	m_fFireTimer += elapsedTime;
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::MouseLeft) && m_nStamina > 0 && isAlive)
	{
		m_cSlashAnim->Restart(false, 1.0f);
		m_nStamina--;
		SGD::Event* stamLoss = new SGD::Event{ "STAMINA_LOSS", nullptr, this };
		stamLoss->QueueEvent();

		SlashMessage* newSlash = new SlashMessage(this);
		newSlash->QueueMessage();
		newSlash = nullptr;
	}
	if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::MouseRight) && m_nArrowAmount > 0)
	{
		m_cBowAnim->Restart(false, 1.0f);
		m_cWalkAnim->Pause();
		m_fPull = 0;
		_finishBow = false;
	}
	if (SGD::InputManager::GetInstance()->IsKeyDown(SGD::Key::MouseRight) && m_nArrowAmount > 0 && isAlive)
	{
		m_fPull += elapsedTime;
	//	Game::GetInstance()->SetSpeedVar(.1f);
	}
	else
	{
		m_cBowAnim->Pause();
	}
	if (SGD::InputManager::GetInstance()->IsKeyReleased(SGD::Key::MouseRight) && m_fFireTimer > 1 && m_nArrowAmount > 0 && isAlive)
	{
		if (m_fPull < .5f)
		SGD::AudioManager::GetInstance()->PlayAudio(m_hFire2);
		else if (m_fPull < 1.0f)
		SGD::AudioManager::GetInstance()->PlayAudio(m_hFire3);
		else
		SGD::AudioManager::GetInstance()->PlayAudio(m_hFire1);

		m_fFireTimer = 0.0f;
		CreateArrowMessage* newArrow = new CreateArrowMessage(this);
		newArrow->QueueMessage();
		newArrow = nullptr;
		
		SGD::Event* arrowFired = new SGD::Event{ "ARROW_FIRED", nullptr, this };
		arrowFired->QueueEvent();
		arrowFired = nullptr;
		m_nArrowAmount--;
		_finishBow = true;
	//	m_cBowAnim->Pause();
	}

	if (m_cSlashAnim != nullptr && m_cSlashAnim->IsPlaying())
	{
		m_cSlashAnim->Update(elapsedTime);
	}	
	else if (m_cBowAnim != nullptr && m_cBowAnim->IsPlaying() && (m_cBowAnim->GetCurrFrame() < 14 || _finishBow))
	{
		m_cBowAnim->Update(elapsedTime);
		if (m_cBowAnim->GetCurrFrame() == 23)
		{
			m_cWalkAnim->Restart(true, 1.0f);
		}
	}
	else if (m_cWalkAnim != nullptr && m_cWalkAnim->IsPlaying() == false)
		m_cWalkAnim->Restart(true, 1.0f);

	if (m_cWalkAnim != nullptr && m_cWalkAnim->IsPlaying())
	{
		m_cWalkAnim->Update(elapsedTime);
	}

	if (m_fStaminaTimer > 2 )
	{
		if (m_nStamina < m_nMaxStamina)
		{
			m_nStamina++;
			SGD::Event* stamGain = new SGD::Event{ "STAMINA_GAIN", nullptr, this };
			stamGain->QueueEvent();
		}
		m_fStaminaTimer = 0;
	}
	if (m_nHealth <= 0 && isAlive)
	{
		isAlive = false;
		m_vtVelocity.x += -50;
		m_vtVelocity.y -= 275;
		SGD::AudioManager::GetInstance()->PlayAudio(m_hDeath, false);
		GameOverMessage* GameOver = new GameOverMessage(this);
		GameOver->QueueMessage();
		GameOver = nullptr;
	}
	m_fStaminaTimer += elapsedTime;

	if (isAlive == false)
	{
		m_vtVelocity.y += 675 * elapsedTime;
		m_fRotation -= 1.5f * elapsedTime;
	}


	m_fOrctimer -= elapsedTime;
	if (m_fOrctimer <= 0)
	{
		m_fOrctimer = (rand() % 10 + 1) * .2f;
		CreateOrcMessage* newOrc = new CreateOrcMessage(this);
		newOrc->QueueMessage();
		newOrc = nullptr;
	}

	m_fPickupTimer -= elapsedTime;

	Entity::Update(elapsedTime);
}
/*virtual*/ void	Hero::Render(void)
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

	// Draw the image
	if (m_cSlashAnim != nullptr && m_cSlashAnim->IsPlaying())
	{
		m_cSlashAnim->Render(m_ptPosition);
	}
	else if (m_cBowAnim != nullptr && m_cBowAnim->IsPlaying())
	{
		m_cBowAnim->Render(m_ptPosition);
	}
	else
	{
		if (m_cWalkAnim != nullptr && m_cWalkAnim->IsPlaying() && isAlive)
		{
			m_cWalkAnim->Render(m_ptPosition);
		}
			//SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_hImage, m_ptPosition, SGD::Rectangle{ 400, 150, 600, 300 });

		else
			SGD::GraphicsManager::GetInstance()->DrawTextureSection(m_hImage, m_ptPosition, SGD::Rectangle{ 400, 150, 600, 300 }, m_fRotation+ 180, m_szSize/2);
	}
}

void	Hero::HandleEvent(const SGD::Event* pEvent) 	// Callback function to process events
{
	if (pEvent->GetEventID() == "HEALTH_LOSS")
	{
		m_nHealth--;
		if (isAlive)
		SGD::AudioManager::GetInstance()->PlayAudio(m_hHit2);

	}
	if (pEvent->GetEventID() == "ARROW_PICKUP" && m_fPickupTimer <= 0)
	{
		m_fPickupTimer = .5f;
		if (m_nArrowAmount < m_nMaxArrows)
			m_nArrowAmount++;
	}
	if (pEvent->GetEventID() == "ARROW_HIT_HERO")
	{
		m_nHealth--;
		GameplayState::GetInstance()->AddOrcHitCount(1);
		if (isAlive)
		SGD::AudioManager::GetInstance()->PlayAudio(m_hHit1);

	}
}
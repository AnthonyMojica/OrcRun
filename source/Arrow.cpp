//*********************************************************************//
//	File:		Arrow.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Entity, Controls and Displays Arrows.
//*********************************************************************//

#include "Arrow.h"

#include "../SGD Wrappers/SGD_Utilities.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Event.h"
#include "DestroyEntityMessage.h"
#include "Character.h"
#include "Orc.h"
#include "Game.h"
#include "GameplayState.h"
#include "BitmapFont.h"
#include <string>
Arrow::Arrow()
{
}


Arrow::~Arrow()
{
	SetOwner(nullptr);
}

/*virtual*/ void	Arrow::Update(float elapsedTime)
{

	if (GetPosition().y > 510)
	{
		m_vtVelocity.y = 0;
		m_vtVelocity.x = -100;
	}
	else
		m_vtVelocity.y += (850 * elapsedTime);


	if (GetOwner()->GetType() == ENT_HERO && m_ptPosition.y <510)
	{

		SGD::Vector rotateVec = SGD::Vector{ 0, -1 };
		rotateVec.Rotate(m_fRotation);

		if (m_vtVelocity.ComputeDotProduct(rotateVec) < 0.999f)
		{
			if (m_vtVelocity.ComputeSteering(rotateVec) < 0)
				m_fRotation += 2 * elapsedTime;
			if (m_vtVelocity.ComputeSteering(rotateVec) > 0)
				m_fRotation -= 2 * elapsedTime;
		}
	}
	if (GetOwner()->GetType() == ENT_ORC)
	{

		SGD::Vector rotateVec = SGD::Vector{ 0, -1 };
		rotateVec.Rotate(m_fRotation);

		if (m_vtVelocity.ComputeDotProduct(rotateVec) > 0.999f)
		{
			if (m_vtVelocity.ComputeSteering(rotateVec) > 0)
				m_fRotation += 2 * elapsedTime;
			if (m_vtVelocity.ComputeSteering(rotateVec) < 0)
				m_fRotation -= 2 * elapsedTime;
		}
	}
	if (m_eOwner->GetType() == ENT_ORC)
	{
		if (m_ptPosition.x < dynamic_cast<Orc*>(m_eOwner)->GetOwner()->GetPosition().x + 50 && isBroken == false && m_ptPosition.y > 509)
		{
			SGD::Event* ArrowPickup = new SGD::Event{ "ARROW_PICKUP", nullptr, this };
			ArrowPickup->QueueEvent();
			ArrowPickup = nullptr;

			DestroyEntityMessage* DestroyIt = new DestroyEntityMessage(this);
			DestroyIt->QueueMessage();
			DestroyIt = nullptr;

		}
	}
	else if (m_ptPosition.x < GetOwner()->GetPosition().x + 50 && isBroken == false && m_eOwner->GetType() == ENT_HERO)
	{
		SGD::Event* ArrowPickup = new SGD::Event{ "ARROW_PICKUP", nullptr, this };
		ArrowPickup->QueueEvent();
		ArrowPickup = nullptr;

		DestroyEntityMessage* DestroyIt = new DestroyEntityMessage(this);
		DestroyIt->QueueMessage();
		DestroyIt = nullptr;
	}

	m_ptPosition += m_vtVelocity * elapsedTime;



	//Deletes Objects once they are off screen enough - a bit of 
	//a rough estimate to account for objects with improper size to image ratio
	if (m_ptPosition.x < -(m_szSize.width + 450))
	{
		DestroyEntityMessage* DestroyIt = new DestroyEntityMessage(this);
		DestroyIt->QueueMessage();
		DestroyIt = nullptr;
	}

}
/*virtual*/ void	Arrow::Render(void)
{
	// Validate the image
	SGD_ASSERT(m_hImage != SGD::INVALID_HANDLE,
		"Entity::Render - image was not set!");

	SGD::Point offset = SGD::Point{ m_ptPosition.x - 25.0f, m_ptPosition.y };
	SGD::Size SizeOffset = SGD::Size{ 115.0f, 0.0f };
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
		m_hImage, offset,
		m_fRotation, SizeOffset, SGD::Color{}, SGD::Size{ 0.2f, 0.1f });

}

/*virtual*/ void Arrow::HandleCollision(const IEntity* pOther)	/*override*/
{
	if ((pOther->GetType() == ENT_ORC
		|| pOther->GetType() == ENT_HERO)
		&& pOther != m_eOwner && m_ptPosition.y < 509)
	{
		SGD::Event* event = new SGD::Event{ "ARROW_HIT", nullptr, this };
		event->QueueEvent(pOther);

		if (pOther->GetType() == ENT_HERO && m_ptPosition.y > 470 
			&& m_ptPosition.x > dynamic_cast<Orc*>(GetOwner())->GetOwner()->GetPosition().x + 50
			&& m_ptPosition.x <  dynamic_cast<Orc*>(GetOwner())->GetOwner()->GetPosition().x + 130)
		{
			SGD::Event* event = new SGD::Event{ "ARROW_HIT_HERO", nullptr, this };
			event->QueueEvent();
			DestroyEntityMessage* DestroyIt = new DestroyEntityMessage(this);
			DestroyIt->QueueMessage();
			DestroyIt = nullptr;
		}
		//event = nullptr;
		if (pOther->GetType() == ENT_ORC)
		{
			const Character* _Orc =
				dynamic_cast<const Character*>(pOther);

			if (_Orc->GetAliveStatus())
			{
				DestroyEntityMessage* DestroyIt = new DestroyEntityMessage(this);
				DestroyIt->QueueMessage();
				DestroyIt = nullptr;
			}
		}

	}


}


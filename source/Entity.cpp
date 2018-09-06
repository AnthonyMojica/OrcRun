//*********************************************************************//
//	File:		Entity.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Entity class is the base moving-object entity,
//				storing shared members for children classes
//*********************************************************************//

#include "Entity.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Utilities.h"
#include "Game.h"
#include "GameplayState.h"
#include "DestroyEntityMessage.h"
#include "BitmapFont.h"
#include <string>

Entity::Entity(void)
{
	int x = GetType();
	bool nothing = false;
}
//*********************************************************************//
// Update
//	- move the entity by its velocity
//	  (given that velocity is the rate of change in pixels-per-second)
/*virtual*/ void Entity::Update(float elapsedTime)	/*override*/
{
	m_ptPosition += (m_vtVelocity * Game::GetInstance()->GetSpeedVar()) * elapsedTime;


	//Deletes Objects once they are off screen enough - a bit of 
	//a rough estimate to account for objects with improper size to image ratio
	if (m_ptPosition.x < -(m_szSize.width + 550))
	{
	DestroyEntityMessage* DestroyIt = new DestroyEntityMessage(this);
	DestroyIt->QueueMessage();
	DestroyIt = nullptr;
    }
}

//*********************************************************************//
// Render
//	- draw the entity's image at its position
/*virtual*/ void Entity::Render( void )		/*override*/
{
	// Validate the image
	SGD_ASSERT( m_hImage != SGD::INVALID_HANDLE, 
		"Entity::Render - image was not set!" );
	
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
		m_fRotation, m_szSize / 2 );
}


//*********************************************************************//
// GetRect
//	- calculate the entity's bounding rectangle
/*virtual*/ SGD::Rectangle Entity::GetRect( void ) const	/*override*/
{
	return SGD::Rectangle{ m_ptPosition, m_szSize };
}


//*********************************************************************//
// HandleCollision
//	- respond to collision between entities
/*virtual*/ void Entity::HandleCollision( const IEntity* pOther )	/*override*/
{
	/* DO NOTHING */
	(void)pOther;		// unused parameter
}


//*********************************************************************//
// AddRef
//	- increase the reference count
/*virtual*/ void Entity::AddRef( void )		/*final*/
{
	SGD_ASSERT( m_unRefCount != 0xFFFFFFFF, 
		"Entity::AddRef - maximum reference count has been exceeded" );

	++m_unRefCount;
}

//*********************************************************************//
// Release
//	- decrease the reference count
//	- self-destruct when no references remain
/*virtual*/ void Entity::Release( void )	/*final*/
{
	--m_unRefCount;

	if( m_unRefCount == 0 )
		delete this;
}


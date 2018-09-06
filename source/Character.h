//*********************************************************************//
//	File:		Character.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Entity, Parent of Player and Enemies.
//*********************************************************************//

#pragma once

#include "Entity.h"
#include "../SGD Wrappers/SGD_IListener.h"


class Character : public Entity
{

protected:
	bool isAlive = true;

public:
	Character() = default;
	virtual ~Character() = default;

	//*****************************************************************//
	// Interface:
	//	- virtual functions for children classes to override
	//virtual void	Update(float elapsedTime)	override;
	//virtual void	Render(void)				override;

	//virtual int		GetType(void)	const			override	{ return ENT_CHARACTER; }
	//virtual SGD::Rectangle GetRect(void)	const			override;
	//virtual void	HandleCollision(const IEntity* pOther)	override;

	bool GetAliveStatus() const { return isAlive; }

};


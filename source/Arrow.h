//*********************************************************************//
//	File:		Arrow.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Entity, Controls and Displays Arrows.
//*********************************************************************//

#pragma once
#include "Entity.h"



class Arrow : public Entity
{
	virtual void	Update(float elapsedTime)	override;
	virtual void	Render(void)				override;
	virtual void	HandleCollision(const IEntity* pOther)	override;


public:
	Arrow();
	virtual ~Arrow();

	virtual int		GetType(void)	const			override	{ return ENT_ARROW; }

	//Accessors
	Entity* GetOwner() { return m_eOwner; }

	//Mutator
	void SetOwner(Entity* _Owner)
	{
		if (m_eOwner != nullptr)
			m_eOwner->Release();
		m_eOwner = _Owner;
		if (m_eOwner != nullptr)
			m_eOwner->AddRef();
	}

private:
	Entity * m_eOwner = nullptr;

	bool isBroken = false;
};


//*********************************************************************//
//	File:		Orc.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Character, Sword Enemy.
//*********************************************************************//

#pragma once
#include "Hero.h"


class Orc : public Character, public SGD::IListener
{



	bool DamageDone = false;
	bool Attacked = false;
	int m_nHealth;
	bool _beginFalling = false;
	float _hitTimer = 0;

	SGD::HAudio m_hOrcDeath1 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hOrcDeath2 = SGD::INVALID_HANDLE;



public:
	Orc();
	virtual ~Orc();

	virtual void	Update(float elapsedTime)	override;
	virtual void	Render(void)				override;
	void	HandleEvent(const SGD::Event* pEvent);
	virtual void Orc::HandleCollision(const IEntity* pOther)	override;


	virtual int		GetType(void)	const			override	{ return ENT_ORC; }

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
	
	CellAnimation* m_cWalkAnim = nullptr;
	CellAnimation* m_cSlashAnim = nullptr;
	bool m_bSlashDone = false;
};


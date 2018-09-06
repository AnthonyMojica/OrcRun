//*********************************************************************//
//	File:		StatOrb.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Entity, Displays Player Stats.
//*********************************************************************//

#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_IListener.h"

class StatOrb : public Entity, public SGD::IListener
{

	int m_nCurrentHealth = 4;
	int m_nCurrentStamina = 4;
	int m_nMaxHealth = 4;
	int m_nMaxStamina = 4;
	int m_nCurrentArrows = 5;
	int m_nMaxArrows = 5;
	float _reloadtimer = 0;
	float m_fPickupTimer = 0;
	virtual void	Update(float elapsedTime)	override;
	virtual void	Render(void)				override;
	void	HandleEvent(const SGD::Event* pEvent) override;

public:

	StatOrb();
	virtual ~StatOrb();

	//Accessors
	int GetCurrentHealth() { return m_nCurrentHealth; }
	int GetCurrentStamina(){ return m_nCurrentStamina; }

	//Mutators
	void SetCurrentHealth(int _HealthDif){ m_nCurrentHealth += _HealthDif; }
	void SetCurrentStamina(int _StaminaDif){ m_nCurrentStamina += _StaminaDif; }


};


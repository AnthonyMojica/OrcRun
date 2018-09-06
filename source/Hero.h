//*********************************************************************//
//	File:		Hero.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Character, Main Player Character.
//*********************************************************************//

#pragma once
#include "Character.h"

class Hero : public Character, public SGD::IListener
{

	int m_nArrowAmount = 5;
	int m_nMaxArrows = 5;
	int m_nStamina = 4;
	int m_nMaxStamina = 4;
	int m_nScore = 0;
	int m_nDistanceTraveled;
	float m_fSpeed;
	float m_fFireTimer;
	float m_fOrctimer;
	float m_fStaminaTimer;
	float m_fPull;
	int m_nHealth;
	int m_nMaxHealth;
	bool isAlive = true;
	bool _finishBow = false;
	float m_fPickupTimer = 0;

	void	HandleEvent(const SGD::Event* pEvent) override;
	SGD::HAudio m_hFire1 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hFire2 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hFire3 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hDeath = SGD::INVALID_HANDLE;
	SGD::HAudio m_hHit1 = SGD::INVALID_HANDLE;
	SGD::HAudio m_hHit2 = SGD::INVALID_HANDLE;



protected: //Enemies need health too
	
public:
	Hero();
	virtual ~Hero();

	virtual void	Update(float elapsedTime)	override;
	virtual void	Render(void)				override;
	virtual int		GetType(void)	const			override	{ return ENT_HERO; }


	float GetPull(){ return m_fPull; }

	void SetPull(float _pull) { m_fPull = _pull; }

	//**************************************************//
	//Animations
	CellAnimation* m_cSlashAnim = nullptr;
	CellAnimation* m_cWalkAnim = nullptr;
	CellAnimation* m_cBowAnim = nullptr;

};


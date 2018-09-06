//*********************************************************************//
//	File:		OrcBow.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Character, Bow Enemy.
//*********************************************************************//

#pragma once
#include "Orc.h"

class OrcBow : public Orc
{
	
	float m_fPull = 0;
	float m_fFireTimer = 0; 
	bool m_fireSystem = false;

public:
	OrcBow();
	virtual ~OrcBow();

	virtual void	Update(float elapsedTime)	override;
	virtual void	Render(void)				override;

	float GetPull(){ return m_fPull; }

	CellAnimation* m_cWalkAnim = nullptr;
	CellAnimation* m_cBowAnim = nullptr;
};


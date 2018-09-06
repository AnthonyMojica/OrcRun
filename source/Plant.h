//*********************************************************************//
//	File:		Plant.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Entity, Displays Plants.
//*********************************************************************//

#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_IListener.h"

class Plant : public Entity, public SGD::IListener
{
	float m_fPlantScale;
	virtual void	Render(void)				override;
	void	HandleEvent(const SGD::Event* pEvent) override;

public:
	Plant();
	virtual ~Plant();

	float GetPlantScale() { return m_fPlantScale; }
};

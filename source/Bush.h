//*********************************************************************//
//	File:		Bush.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Entity, Displays a Bush.
//*********************************************************************//

#pragma once
#include "Entity.h"
#include "../SGD Wrappers/SGD_IListener.h"

class Bush : public Entity, public SGD::IListener
{
	float m_fBushScale;
	virtual void	Render(void)				override;
	void	HandleEvent(const SGD::Event* pEvent) override; 

public:
	Bush();
	virtual ~Bush();

	float GetBushScale() { return m_fBushScale; }
};


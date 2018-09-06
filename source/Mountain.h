//*********************************************************************//
//	File:		Mountain.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Entity, Displays Mountains.
//*********************************************************************//

#pragma once
#include "Entity.h"

class Mountain : public Entity
{

	float MountainScale;
	virtual void	Render(void)				override;

public:
	Mountain();
	virtual ~Mountain();

	float GetMountainScale(){ return MountainScale; }
};


//*********************************************************************//
//	File:		Cloud.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Entity, Displays a Cloud.
//*********************************************************************//

#pragma once

#include "Entity.h"


class Cloud : public Entity
{
	float cloudScale;
	virtual void	Render(void)				override;

public:
	Cloud();
	virtual ~Cloud() = default;
};


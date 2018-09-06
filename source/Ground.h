//*********************************************************************//
//	File:		Ground.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Entity, Displays the Ground.
//*********************************************************************//

#pragma once

#include "Entity.h"

class Ground : public Entity
{

	virtual void	Render(void)				override;

public:
	Ground();
	virtual ~Ground();
};


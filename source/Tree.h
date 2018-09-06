//*********************************************************************//
//	File:		Tree.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Child of Entity, Displays Trees.
//*********************************************************************//

#pragma once
#include "Entity.h"
class Tree : public Entity
{
	float TreeScale;
	virtual void	Render(void)				override;

public:
	Tree();
	virtual ~Tree();

	float GetTreeScale() { return TreeScale; }
};


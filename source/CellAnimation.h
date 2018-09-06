//*********************************************************************//
//	File:		CellAnimation.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	CellAnimation class runs an animation of 
//				fixed-size frames, based on the elapsed time
//*********************************************************************//

#pragma once

#include "../SGD Wrappers/SGD_Handle.h"
#include "../SGD Wrappers/SGD_Geometry.h"
#include "../SGD Wrappers/SGD_Color.h"


//*********************************************************************//
// CellAnimation class
//	- runs animation using an image of fixed-size frames
//	- image MUST have a size power-of-2 (e.g. 64, 128, 256, 512)
//	- frames MUST have a fixed-size
class CellAnimation
{
public:
	//*****************************************************************//
	// Default Constructor & Destructor
	CellAnimation( void )	= default;
	~CellAnimation( void )	= default;
	
	//*****************************************************************//
	// Initialize & Terminate
	virtual void	Initialize	(int _FrameWidth, int _FrameHeight, 
								int _numCols, int _NumFrames, float _TimePerFrame, 
								float _Speed, SGD::HTexture _image );	// should have one function/file per animation
	virtual void	Terminate(void);
	
	//*****************************************************************//
	// Controls:
	virtual void	Update(float elapsedTime);
	virtual void	Render(SGD::Point position, float scale = 1.0f, SGD::Color color = {});
	
	SGD::Rectangle GetRect( SGD::Point position, float scale = 1.0f ) const;

	virtual void	Restart(bool looping = false, float speed = 1.0f);
	virtual void	Pause(bool pause = true)	{ m_bIsPlaying = !pause; }

	void SetFrameSpeed(float _frameSpeed){ m_fTimePerFrame = _frameSpeed; }
	void SetScale(float _x, float _y) { m_fScaleX = _x; m_fScaleY = _y; }
	int GetCurrFrame()	{ return m_nCurrFrame;  }
	//*****************************************************************//
	// Accessors:
	bool	IsPlaying	( void ) const		{	return m_bIsPlaying;	}
	bool	IsFinished	( void ) const		{	return m_bIsFinished;	}

private:
	//*****************************************************************//
	// image
	SGD::HTexture	m_hImage		= SGD::INVALID_HANDLE;

	// cell algorithm data
	int				m_nFrameWidth	= 0;
	int				m_nFrameHeight	= 0;
	int				m_nNumCols		= 0;

	// animation data
	int				m_nCurrFrame	= 0;
	int				m_nNumFrames	= 0;

	float			m_fTimeWaited	= 0.0f;
	float			m_fTimePerFrame	= 0.0f;
	float			m_fSpeed		= 1.0f;

	bool			m_bIsPlaying	= false;
	bool			m_bIsLooping	= false;
	bool			m_bIsFinished	= false;

	//Scale
	float m_fScaleX = 1.0f;
	float m_fScaleY = 1.0f;


};
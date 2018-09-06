//*********************************************************************//
//	File:		CellAnimation.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	CellAnimation class runs an animation of
//				fixed-size frames based on the elapsed time
//*********************************************************************//

#include "CellAnimation.h"

#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_Utilities.h"


//*********************************************************************//
// Initialize
//	- set up the animation
//	- should load from a file
//	- hardcoded to use SGD_Anim_Explosion.png
void CellAnimation::Initialize(int _FrameWidth, int _FrameHeight,
	int _numCols, int _NumFrames, float _TimePerFrame, float _Speed, SGD::HTexture _image)
{
	// Load the image
	m_hImage = _image;

	m_nFrameWidth = _FrameWidth;
	m_nFrameHeight = _FrameHeight;
	m_nNumCols = _numCols;

	m_nCurrFrame = 0;
	m_nNumFrames = (int)_NumFrames;	// indices [0] - [9]

	m_fTimeWaited = 0.0f;
	m_fTimePerFrame = _TimePerFrame;
	m_fSpeed = _Speed;

	m_bIsPlaying = false;
	m_bIsLooping = false;
	m_bIsFinished = false;
}

//*********************************************************************//
// Terminate
//	- unload the image
void CellAnimation::Terminate ( void )
{
	SGD::GraphicsManager::GetInstance()->UnloadTexture( m_hImage );
}


//*********************************************************************//
// Update
//	- run the animation based on the elapsed time
void CellAnimation::Update( float elapsedTime )
{
	// Is the animation paused?
	if( m_bIsPlaying == false )
		return;


	// Increase our timer
	m_fTimeWaited += elapsedTime * m_fSpeed;

	// Time for next frame?
	if( m_fTimeWaited >= m_fTimePerFrame )
	{
		// Move to the next frame
		m_nCurrFrame++;
		m_fTimeWaited = 0.0f;

		// Have we reached the end?
		if( m_nCurrFrame >= m_nNumFrames )
		{
			// Should it loop to the beginning?
			if( m_bIsLooping == true )
				m_nCurrFrame = 0;
			else 
			{
				// Stop at the last valid frame
				m_nCurrFrame = m_nNumFrames-1;
				m_bIsPlaying = false;
				m_bIsFinished = true;
			}
		}
	}
}

//*********************************************************************//
// Render
//	- draw the current frame using the Cell Algorithm
void CellAnimation::Render( SGD::Point position, float scale,
	SGD::Color color )
{
	// Validate the image
	SGD_ASSERT( m_hImage != SGD::INVALID_HANDLE,
		"CellAnimation::Render - animation was not initialized" );

	// Check the parameters
	if(color.alpha == 0)
		return;

	// Calculate the source rect for the current frame
	SGD::Rectangle frame = { };

	frame.left = (float)( (m_nCurrFrame % m_nNumCols) * m_nFrameWidth  );
	frame.top  = (float)( (m_nCurrFrame / m_nNumCols) * m_nFrameHeight );

	frame.right = frame.left + m_nFrameWidth;
	frame.bottom = frame.top + m_nFrameHeight;


	// Draw
	SGD::GraphicsManager::GetInstance()->DrawTextureSection(
		m_hImage,
		position,
		frame,
		0, {},
		color, 
		SGD::Size{ m_fScaleX, m_fScaleY } );
}


//*********************************************************************//
// GetRect
//	- return the frame rect at the given position
SGD::Rectangle	CellAnimation::GetRect( SGD::Point position, float scale ) const
{
	return SGD::Rectangle{ position, SGD::Size{ (float)m_nFrameWidth, (float)m_nFrameHeight } * scale };
}


//*********************************************************************//
// Restart
//	- reset to frame 0
void CellAnimation::Restart( bool looping, float speed )
{
	// Store the parameters
	m_bIsLooping	= looping;
	m_fSpeed		= speed;

	// Reset
	m_nCurrFrame	= 0;
	m_fTimeWaited	= 0.0f;
	m_bIsPlaying	= true;
	m_bIsFinished	= false;
}

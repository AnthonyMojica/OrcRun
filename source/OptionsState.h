//*********************************************************************//
//	File:		OptionsState.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	Change Settings and Save to a Text File
//*********************************************************************//

#pragma once

#include "IGameState.h"
#include "../SGD Wrappers/SGD_AudioManager.h"

class OptionsState: public IGameState
{
public:
	//*****************************************************************//
	// Singleton Accessor:
	static OptionsState* GetInstance(void);


	//*****************************************************************//
	// IGameState Interface:
	virtual void Enter(void)				override;	// load resources / reset data
	virtual void Exit(void)				override;	// unload resources

	virtual int Update(float elapsedTime)	override;	// handle input / update entities
	virtual void Render(float elapsedTime)	override;	// draw entities / menu

	int GetMusicVolume() { return m_nMusicVolume; }
	int GetSFXVolume() { return m_nSFXVolume; }

	void SetMusicVol(int _Music) { m_nMusicVolume = _Music; }
	void SetSFXVol(int _SFX) { m_nSFXVolume = _SFX; }

private:
	//*****************************************************************//
	// SINGLETON (not-dynamically allocated)
	//	- Hide the "Quadrilogy-of-Evil" so they cannot be called
	//	  by outside functions
	OptionsState(void) = default;	// default constructor
	virtual ~OptionsState(void) = default;	// destructor

	OptionsState(const OptionsState&) = delete;	// copy constructor
	OptionsState& operator= (const OptionsState&) = delete;	// assignment operator

	void SaveSettings();

	SGD::HAudio m_hBackground = SGD::INVALID_HANDLE;
	SGD::HAudio m_hOrcHit = SGD::INVALID_HANDLE;

	//*****************************************************************//
	// cursor index / position
	int m_nCursor = 0;

	int m_nMusicVolume = 100;
	int m_nSFXVolume = 100;
	float m_fBufferInput = 0;
	float m_fSoundBuffer = 0;
	bool m_bMusicPlaying = false;
};


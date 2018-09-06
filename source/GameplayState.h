//*********************************************************************//
//	File:		GameplayState.h
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	GameplayState class initializes & runs the game logic
//*********************************************************************//

#pragma once

#include "IGameState.h"							// uses IGameState
#include "../SGD Wrappers/SGD_Handle.h"			// uses HTexture & HAudio
#include "../SGD Wrappers/SGD_Declarations.h"	// uses Message
#include <vector>

//*********************************************************************//
// Forward class declaration
//	- tells the compiler that the type exists
//	- can make pointers or references to the type
//	- MUST include their headers in the .cpp to dereference
class Entity;
class EntityManager;

struct HighScore
{
	std::string Name;
	int Distance;
	int Score;
};
//*********************************************************************//
// GameplayState class
//	- runs the game logic
//	- SINGLETON! (Static allocation, not dynamic)
class GameplayState : public IGameState
{
public:
	//*****************************************************************//
	// Singleton Accessor:
	static GameplayState* GetInstance( void );

	
	//*****************************************************************//
	// IGameState Interface:
	virtual void	Enter	( void )				override;	// load resources / reset data
	virtual void	Exit	( void )				override;	// unload resources
													
	virtual int	Update	( float elapsedTime )	override;	// handle input & update game entities
	virtual void	Render	( float elapsedTime )	override;	// render game entities / menus

	Entity* GetHero() { return m_eHero; }
	bool GetGameStatus() { return m_bGameOver; }
	void SetGameStatus(bool _GameOver) { m_bGameOver = _GameOver; }
	void AddOrcArrowCount(int _orcarrowFired) { m_nOrcArrowsFired += _orcarrowFired; }
	void AddArrowCount(int _arrowFired) { m_nHeroArrowsFired += _arrowFired; }
	void AddHitCount(int HeroHit) { m_nHeroArrowHits += HeroHit; }
	void AddOrcHitCount(int OrcHit) { m_nOrcArrowHits += OrcHit; }

	std::vector<HighScore*> GetHighScores() { return m_vHighScores; }
	void AddHighScore(HighScore* newScore) { m_vHighScores.push_back(newScore); }

	bool GetDebug() { return m_bInDebug; }

private:
	//*****************************************************************//
	// SINGLETON (not-dynamically allocated)
	GameplayState( void )			= default;	// default constructor
	virtual ~GameplayState( void )	= default;	// destructor

	GameplayState( const GameplayState& )				= delete;	// copy constructor
	GameplayState& operator= ( const GameplayState& )	= delete;	// assignment operator
		
	//*****************************************************************//
	// Assets
	SGD::HTexture m_hHero = SGD::INVALID_HANDLE;
	SGD::HTexture m_hCloud = SGD::INVALID_HANDLE;
	SGD::HTexture m_hMountain = SGD::INVALID_HANDLE;
	SGD::HTexture m_hGround = SGD::INVALID_HANDLE;
	SGD::HTexture m_hTree = SGD::INVALID_HANDLE;
	SGD::HTexture m_hPlantRed = SGD::INVALID_HANDLE;
	SGD::HTexture m_hPlantBlue = SGD::INVALID_HANDLE;
	SGD::HTexture m_hArrow = SGD::INVALID_HANDLE;
	SGD::HTexture m_hBush = SGD::INVALID_HANDLE;
	SGD::HTexture m_hOrc = SGD::INVALID_HANDLE;
	SGD::HTexture m_hOrcBow = SGD::INVALID_HANDLE;
	SGD::HTexture m_hHealthBackground = SGD::INVALID_HANDLE;
	SGD::HTexture m_hHealthStamina = SGD::INVALID_HANDLE;
	SGD::HTexture m_hGameOverBack = SGD::INVALID_HANDLE;
	SGD::HTexture m_hBowAlone = SGD::INVALID_HANDLE;


	SGD::HAudio m_hBackground = SGD::INVALID_HANDLE;
	SGD::HAudio m_hVictory = SGD::INVALID_HANDLE;
	SGD::HAudio m_hLoss = SGD::INVALID_HANDLE;




	//*****************************************************************//
	// Game Entities
	EntityManager*	m_pEntities			= nullptr;
	Entity* m_eHero = nullptr;
	//Entity* m_eOrc = nullptr;

	//*****************************************************************//
	// Factory Methods
	Entity* CreateHero(void) const;
	Entity* CreateOrc(Entity* _Hero) const;
	Entity* CreateOrcBow(Entity* _Hero) const;
	Entity* CreateCloud(void) const;
	Entity* CreateGround(void)const;
	Entity* CreateMountain(void)const;
	Entity* CreateTree(void)const;
	Entity* CreateBush(void)const;
	Entity* CreateArrow(Entity* _Hero)const;
	Entity* CreateArrowOrc(Entity* _OrcBow)const;
	Entity* CreatePlant(void) const;

	void AddScore(int _addScore){ m_fScore += _addScore; }
	void InitiateBackground() const;
	void RandomlyGenerateBackground(float elapsedTime);
	void GameOverScreen(float elapsedTime);
	void SaveHighScore(HighScore* _newScore);
	void AddController();

	void LoadHighScore();
	//*****************************************************************//
	// Message Callback Procedure
	static void MessageProc( const SGD::Message* pMsg );

	float m_fcloudTimer = 0;
	float m_fmountainTimer = 0;
	float m_fTreeTimer = 0;
	float m_fBushTimer = 0;
	float m_fPlantTimer = 0;
	float m_fOrctimer = 0; 
	bool m_bGameOver = false;
	bool m_fGoToCredits = false;
	float m_fDistanceTraveled = 0;
	int m_nBestPreviousDistance = 100;
	int m_fScore = 0;
	bool m_bPlayerWon = false;

	//FPS
	int m_unFrames = 0;
	int m_unFPS = 0;
	float m_fFPSTimer = 0;

	//Debug Mode
	bool m_bInDebug = false;

	bool m_bNewBestDistance = false;
	float m_fBestTimer = 0;

	//Advanced Stats
	int m_nHeroArrowsFired = 0;
	int m_nOrcArrowsFired = 0;
	int m_nHeroArrowHits = 0;
	int m_nOrcArrowHits= 0;
	float m_fPlayeraccuracy = 0;
	float m_fOrcAccuracy = 0;

	//Fancy Score Screen
	int GameOverSequence = 0;
	float ScoreScreenTimer = 3;
	float ScoreScreenTimer2 = 0;
	bool fancyCount = true;
	int m_fDistanceTraveled2 = 0;
	int m_fScore2 = 0;
	int m_nTotalScore = 0;
	int m_nTotalScore2 = 0;
	int FinalScore = 0;

	unsigned int m_uController = 0;
	std::vector<HighScore*> m_vHighScores;

};

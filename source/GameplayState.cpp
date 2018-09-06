//*********************************************************************//
//	File:		GameplayState.cpp
//	Author:		Anthony J Mojica
//	Course:		Structure of Game Design
//	Purpose:	GameplayState class initializes & runs the game logic
//*********************************************************************//

#include "GameplayState.h"

#include "Game.h"
#include "MainMenuState.h"
#include "PauseMenu.h"
#include "CreditsState.h"
#include "OptionsState.h"

#include "../SGD Wrappers/SGD_AudioManager.h"
#include "../SGD Wrappers/SGD_GraphicsManager.h"
#include "../SGD Wrappers/SGD_InputManager.h"
#include "../SGD Wrappers/SGD_String.h"
#include "../SGD Wrappers/SGD_Utilities.h"
#include "BitmapFont.h"

#include "../SGD Wrappers/SGD_EventManager.h"
#include "../SGD Wrappers/SGD_Event.h"

#include "../SGD Wrappers/SGD_MessageManager.h"
#include "../SGD Wrappers/SGD_Message.h"
#include "MessageID.h"
#include "DestroyEntityMessage.h"
#include "CreateArrowMessage.h"
#include "CreateOrcArrowMsg.h"
#include "SlashMessage.h"
#include "AddScoreMessage.h"
#include "CreateOrcMessage.h"

#include "EntityManager.h"
#include "Entity.h"
#include "Hero.h"
#include "Cloud.h"
#include "Ground.h"
#include "Mountain.h"
#include "Tree.h"
#include "Bush.h"
#include "Arrow.h"
#include "Orc.h"
#include "OrcBow.h"
#include "StatOrb.h"

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <ctime>
#include <iostream>
#include <fstream>



//*********************************************************************//
// GetInstance
//	- allocate static global instance
//	- return THE instance
/*static*/ GameplayState* GameplayState::GetInstance( void )
{
	static GameplayState s_Instance;	// stored in global memory once
	return &s_Instance;
}


//*********************************************************************//
// Enter
//	- reset game
//	- load resources / assets
//	- allocate & initialize game entities
/*virtual*/ void GameplayState::Enter(void)	/*override*/
{

	srand(static_cast<unsigned int>(time(nullptr)));
	rand();
	// Initialize the Event & Message Managers
	SGD::EventManager::GetInstance()->Initialize();
	SGD::MessageManager::GetInstance()->Initialize(&GameplayState::MessageProc);

	//Initialize Graphics and Audio assets
	m_hCloud = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_Cloud.png", SGD::Color{});
	m_hHero = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_HeroBow.png", SGD::Color{});
	m_hGround = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_Ground.png", SGD::Color{});
	m_hMountain = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_Mountain.png", SGD::Color{});
	m_hTree = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_Tree.png", SGD::Color{});
	m_hPlantBlue = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_PlantBlue.png", SGD::Color{});
	m_hPlantRed = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_PlantRed.png", SGD::Color{});
	m_hArrow = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_Arrow.png", SGD::Color{});
	m_hBush = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_Bush.png", SGD::Color{});
	m_hOrc = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_OrcSword.png", SGD::Color{});
	m_hOrcBow = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_OrcBow.png", SGD::Color{});
	m_hHealthBackground = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_HealthandStamina.png", SGD::Color{});
	m_hHealthStamina = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_HealthStamArrow.png", SGD::Color{});
	m_hGameOverBack = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_GameOverScore.png", SGD::Color{});
	m_hBowAlone = SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/AJM_BowALONE.png", SGD::Color{});

	m_hBackground = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/AJM_BackMusic.xwm");
	m_hVictory = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/AJM_newVictory2.wav");
	m_hLoss = SGD::AudioManager::GetInstance()->LoadAudio(L"resource/audio/AJM_Death 2.wav");


	SGD::AudioManager::GetInstance()->SetAudioVolume(m_hVictory, OptionsState::GetInstance()->GetSFXVolume());
	SGD::AudioManager::GetInstance()->SetAudioVolume(m_hLoss, OptionsState::GetInstance()->GetSFXVolume());

	m_fGoToCredits = false;
	GameOverSequence = 0;
	ScoreScreenTimer = 3;
	ScoreScreenTimer2 = 0;
	fancyCount = true;
	m_fDistanceTraveled2 = 0;
	m_fScore2 = 0;
	m_nTotalScore = 0;
	m_nTotalScore2 = 0; 
	m_bGameOver = false;
	m_fDistanceTraveled = 0;
	m_fScore = 0;

	//Advanced Stats
	m_nOrcArrowsFired = 0;
	m_nHeroArrowsFired = 0;
	m_fPlayeraccuracy = 0;
	m_fOrcAccuracy = 0;
	m_nHeroArrowHits = 0;
	m_nOrcArrowHits = 0;

	//LoadHighScore();

	//m_hArrow		= SGD::GraphicsManager::GetInstance()->LoadTexture(L"resource/graphics/SGD_Bullet.png", SGD::Color{});

	SGD::GraphicsManager::GetInstance()->SetClearColor(
		SGD::Color{ 135, 180, 222, 255 });


	// Allocate the Entity Manager
	m_pEntities = new EntityManager;

	//Display the Ground
	Entity* ground = CreateGround();
	m_pEntities->AddEntity(ground, 6);



	ground->Release();
	ground = nullptr;

	// Create the Hero
	if (Game::GetInstance()->GetMainMenuState() == false)
	{
	Entity* newHero = CreateHero();
	m_pEntities->AddEntity(newHero, 7);

	newHero->Release();
	newHero = nullptr;

	//Create the Stat Orbs representing Health and Stamina
	StatOrb* StatOrbs = new StatOrb;
	StatOrbs->SetImage(m_hHealthStamina);
	StatOrbs->SetPosition(SGD::Point{ 108, 605 });
	StatOrbs->SetSize(SGD::Size{ 5, 5 });

	m_pEntities->AddEntity(StatOrbs, 10);
	StatOrbs->Release();

	//Health Background
	Entity* HealthBack = new Entity;
	HealthBack->SetImage(m_hHealthBackground);
	HealthBack->SetPosition(SGD::Point{ 50, 530 });
	HealthBack->SetSize(SGD::Size{ 5, 5 });
	m_pEntities->AddEntity(HealthBack, 9);
	HealthBack->Release();

	Entity* BowRecharge = new Entity;
	BowRecharge->SetImage(m_hBowAlone);
	BowRecharge->SetPosition(SGD::Point{ 330, 530 });
	BowRecharge->SetSize(SGD::Size{ 5, 5 });
	m_pEntities->AddEntity(BowRecharge, 9);
	BowRecharge->Release();

	SGD::AudioManager::GetInstance()->SetAudioVolume(m_hBackground, OptionsState::GetInstance()->GetMusicVolume());
	SGD::AudioManager::GetInstance()->PlayAudio(m_hBackground, true);


	}


	m_uController = SGD::InputManager::GetInstance()->GetControllerFlags();

	InitiateBackground();

	ScoreScreenTimer = 1;
}


//*********************************************************************//
// Exit
//	- deallocate entities
//	- unload resources / assets
/*virtual*/ void GameplayState::Exit( void )	/*override*/
{
	m_bGameOver = false; 
	//Unload Assets
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hHero);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hCloud);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hGround);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hMountain);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hTree);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hPlantBlue);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hPlantRed);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hArrow);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hBush);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hOrc);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hOrcBow);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hHealthBackground);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hHealthStamina);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hGameOverBack);
	SGD::GraphicsManager::GetInstance()->UnloadTexture(m_hBowAlone);

	SGD::AudioManager::GetInstance()->UnloadAudio(m_hBackground);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hVictory);
	SGD::AudioManager::GetInstance()->UnloadAudio(m_hLoss);

	m_fBestTimer = 0;

	if (m_eHero != nullptr)
	{
		m_eHero->Release();
		m_eHero = nullptr;
	}
	// Release game entities
	if( m_pEntities != nullptr )
	{
		m_pEntities->RemoveAll();
		delete m_pEntities;
		m_pEntities = nullptr;
	}
	
	// Terminate & deallocate the SGD wrappers
	SGD::MessageManager::GetInstance()->Terminate();
	SGD::MessageManager::DeleteInstance();

	SGD::EventManager::GetInstance()->Terminate();
	SGD::EventManager::DeleteInstance();
}


//*********************************************************************//
// Update
//	- handle input & update entities
/*virtual*/ int GameplayState::Update(float elapsedTime)	/*override*/
{
	SGD::InputManager* pInput = SGD::InputManager::GetInstance();



	// Calculate FPS
	m_unFrames++;
	m_fFPSTimer += elapsedTime;
	if (m_fFPSTimer >= 1.0f)		// 1 second
	{
		m_unFPS = m_unFrames;
		m_unFrames = 0;
		m_fFPSTimer = 0.0f;
	}


	// Press Escape to return to MainMenuState
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true && Game::GetInstance()->GetMainMenuState() == false
		&& m_bGameOver == false)
	{
		// ChangeState is VERY VOLATILE!!!
		//	- can only be safely called by a game state's
		//	  Update or Render methods!
		Game::GetInstance()->SetPausedState(true);
		Game::GetInstance()->PushState(PauseMenu::GetInstance());
		PauseMenu::GetInstance()->Enter();
		// Exit this state immediately
		return 1;	// keep playing in the new state
	}
	if (pInput->IsKeyPressed(SGD::Key::Escape) == true && m_bGameOver)
	{
		Game::GetInstance()->SetMainMenuState(true);
		Game::GetInstance()->ChangeState(GameplayState::GetInstance());
		Game::GetInstance()->PushState(MainMenuState::GetInstance());
		return 3;
	}

	if (pInput->IsKeyPressed(SGD::Key::F1) == true)
	{
		m_bInDebug = !m_bInDebug;

	}

	//if (SGD::InputManager::GetInstance()->IsKeyPressed(SGD::Key::S))
	//{
	//	Game::GetInstance()->SetSpeedVar(0.4f);
	//}
	RandomlyGenerateBackground(elapsedTime);

	if (m_bGameOver == false)
		m_fDistanceTraveled += elapsedTime * (Game::GetInstance()->GetSpeedVar() + 2);

	if (m_fDistanceTraveled > m_nBestPreviousDistance && Game::GetInstance()->GetMainMenuState() == false)
	{
		if (m_bPlayerWon == false)
		{
			m_fBestTimer = 2;
			m_bPlayerWon = true;
			SGD::AudioManager::GetInstance()->PlayAudio(m_hVictory);
		}
		m_nBestPreviousDistance = (int)m_fDistanceTraveled;
	}
	// Update the entities
	m_pEntities->UpdateAll( elapsedTime );
	m_pEntities->CheckCollisions(8, 12);
	m_pEntities->CheckCollisions(7, 12);

	
	// Process the Event Manager
	//	- all the events will be sent to the registered IListeners' HandleEvent methods
	SGD::EventManager::GetInstance()->Update();

	// Process the Message Manager
	//	- all the messages will be sent to our MessageProc
	SGD::MessageManager::GetInstance()->Update();


	//GAME OVER SEQUENCE
	if (m_bGameOver)
	{
		GameOverScreen(elapsedTime);
	}


	if (m_fGoToCredits)
	{
		Game::GetInstance()->SetMainMenuState(true);
		Game::GetInstance()->ChangeState(GameplayState::GetInstance());
		Game::GetInstance()->PushState(CreditsState::GetInstance());
		CreditsState::GetInstance()->Enter();
		return 3;
	}
	m_fBestTimer -= elapsedTime;

	return 1;	// keep playing
}


//*********************************************************************//
// Render
//	- render the game entities
/*virtual*/ void GameplayState::Render(float elapsedTime)	/*override*/
{
	BitmapFont* pFont = Game::GetInstance()->GetFont();

	// Render the entities
	m_pEntities->RenderAll();

	if (Game::GetInstance()->GetMainMenuState() == false)
	{
		if (m_fBestTimer > 0)
		{
		pFont->Draw("New Best Distance!", SGD::Point{ 250, 75 }, .5f, SGD::Color{});
		int dist = (int)m_fDistanceTraveled;
		}

	pFont->Draw("Distance: ", SGD::Point{ 550, 570 }, .4f, SGD::Color{});
	int dist = (int)m_fDistanceTraveled;
	std::string _distance = std::to_string(dist);
	pFont->Draw(_distance.c_str(), SGD::Point{ 850, 570 }, .4f, SGD::Color{});

	pFont->Draw("Best Distance: ", SGD::Point{ 550, 600 }, .4f, SGD::Color{});
	dist = m_nBestPreviousDistance;
	_distance = std::to_string(dist);
	pFont->Draw(_distance.c_str(), SGD::Point{ 920, 600 }, .4f, SGD::Color{});

	FinalScore = m_fScore;
	std::string _score = std::to_string(FinalScore);
	pFont->Draw("Orcs Slain: ", SGD::Point{ 550, 670 }, .4f, SGD::Color{});
	pFont->Draw(_score.c_str(), SGD::Point{ 850, 670 }, .4f, SGD::Color{});

	FinalScore = m_fScore + (int)m_fDistanceTraveled + m_nTotalScore2;
	_score = std::to_string(FinalScore);
	pFont->Draw("total Score: ", SGD::Point{ 550, 700 }, .5f, SGD::Color{});
	pFont->Draw(_score.c_str(), SGD::Point{ 930, 700 }, .5f, SGD::Color{});

	if (m_bInDebug)
	{
		pFont->Draw("Debug Mode ON: F1 to Exit Debug", SGD::Point{ 10, 10 }, .5f, SGD::Color{});
		std::string _FPS = std::to_string(m_unFPS);
		pFont->Draw(_FPS.c_str(), SGD::Point{50, 50 }, .5f, SGD::Color{});
	}

	}
	if (m_bGameOver)
	{
		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hGameOverBack, 
			SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 - 495 , 110 });

		SGD::GraphicsManager::GetInstance()->DrawTexture(m_hGameOverBack,
			SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 + 20, 110 }, 0.0f, {}, {}, SGD::Size{ .8f, 1.6f });


		if (GameOverSequence >= 1)
		{
			std::string _score2 = std::to_string(m_fScore2);
			pFont->Draw("Score: ", SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 - 435, 170 }, .5f, SGD::Color{});
			pFont->Draw(_score2.c_str(), SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 - 250, 170 }, .5f, SGD::Color{});

		}
		if (GameOverSequence >= 1)
		{
			std::string _distance2 = std::to_string(m_fDistanceTraveled2);
			pFont->Draw("Distance: ", SGD::Point{ 
				Game::GetInstance()->GetScreenSize().width / 2 - 435, 220 }, .5f, SGD::Color{});

			pFont->Draw(_distance2.c_str(), SGD::Point{ 
				Game::GetInstance()->GetScreenSize().width / 2 - 160, 220 }, .5f, SGD::Color{});

		}
		if (GameOverSequence >= 1)
		{
			std::string _TotalScore = std::to_string(m_nTotalScore2);
			pFont->Draw("Total: ", SGD::Point{
				Game::GetInstance()->GetScreenSize().width / 2 - 435, 270 }, .5f, SGD::Color{});

				pFont->Draw(_TotalScore.c_str(), SGD::Point{
					Game::GetInstance()->GetScreenSize().width / 2 - 250, 270 }, .5f, SGD::Color{});
		}

		//ADVANCED GAME STATISTICS
		if (GameOverSequence >= 1)
		{
			std::string Orcfired = std::to_string(m_nHeroArrowsFired);
			pFont->Draw("Arrows Fired: ", SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 + 45, 210 }, .28f, SGD::Color{});
			pFont->Draw(Orcfired.c_str(), SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 + 350, 210 }, .28f, SGD::Color{});
		}
		if (GameOverSequence >= 1)
		{
			std::string HeroHits = std::to_string(m_nHeroArrowHits);
			pFont->Draw("Arrows Hit: ", SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 + 45, 240 }, .28f, SGD::Color{});
			pFont->Draw(HeroHits.c_str(), SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 + 350, 240 }, .28f, SGD::Color{});
		}

		if (GameOverSequence >= 1)
		{
			if (m_nHeroArrowHits != 0 && m_nHeroArrowsFired != 0)
				m_fPlayeraccuracy = ((float)m_nHeroArrowHits / (float)m_nHeroArrowsFired) * 100.0f;
			if (m_fPlayeraccuracy >= 100)
				m_fPlayeraccuracy = 100;
			std::string PlayerAccuracy = std::to_string((int)m_fPlayeraccuracy);
			pFont->Draw("Accuracy:           *", SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 +45, 270 }, .28f, SGD::Color{});
			pFont->Draw(PlayerAccuracy.c_str(), SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 + 350, 270 }, .28f, SGD::Color{});
		}
		if (GameOverSequence >= 2)
		{
			std::string Orcfired = std::to_string(m_nOrcArrowsFired);
			pFont->Draw("Orc Arrows Fired: ", SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 + 45, 300 }, .28f, SGD::Color{});
			pFont->Draw(Orcfired.c_str(), SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 + 350, 300 }, .28f, SGD::Color{});
		}

		if (GameOverSequence >= 2)
		{
			std::string Orcfired = std::to_string(m_nOrcArrowHits);
			pFont->Draw("Orc Arrows Hit: ", SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 + 45, 330 }, .28f, SGD::Color{});
			pFont->Draw(Orcfired.c_str(), SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 + 350, 330 }, .28f, SGD::Color{});
		}		
		if (GameOverSequence >= 2)
		{
			if (m_nOrcArrowsFired != 0 && m_nOrcArrowHits != 0)
				m_fOrcAccuracy = ((float)m_nOrcArrowHits / (float)m_nOrcArrowsFired) * 100.0f;
			if (m_fOrcAccuracy >= 100)
				m_fOrcAccuracy = 100;
			std::string Orcfired = std::to_string((int)m_fOrcAccuracy);
			pFont->Draw("Orc Accuracy:       * ", SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 + 45, 360 }, .28f, SGD::Color{});
			pFont->Draw(Orcfired.c_str(), SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 + 350, 360 }, .28f, SGD::Color{});
		}

		if (GameOverSequence >= 2)
		{
			if (m_vHighScores.size() > 0)
			{
				std::string Orcfired = m_vHighScores[0]->Name;
				//	pFont->Draw("Orc Arrows Hit: ", SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 + 45, 390 }, .28f, SGD::Color{});
				pFont->Draw(Orcfired.c_str(), SGD::Point{ Game::GetInstance()->GetScreenSize().width / 2 + 350, 390 }, .28f, SGD::Color{});
			}
		}

	}


}
void GameplayState::GameOverScreen(float elapsedTime)
{
	ScoreScreenTimer -= elapsedTime; 
	ScoreScreenTimer2 -= elapsedTime;
	fancyCount = !fancyCount;

	if (GameOverSequence == 0 && ScoreScreenTimer <= 0)
	{
		GameOverSequence = 1;
		ScoreScreenTimer = 1;
	}
	if (GameOverSequence == 1 && ScoreScreenTimer <= 0)
	{
		GameOverSequence = 2;
		ScoreScreenTimer = 1;

		if (m_bPlayerWon)
		{
			m_fBestTimer = 10;
		SGD::AudioManager::GetInstance()->PlayAudio(m_hVictory);
		}
		else
			SGD::AudioManager::GetInstance()->PlayAudio(m_hLoss);
	}
	if (m_fScore > 0 && ScoreScreenTimer2 <= 0 && GameOverSequence == 2)
	{
		ScoreScreenTimer2 = .01f;
		m_fScore -= 1;
		m_fScore2 += 1;
		m_nTotalScore2 += 1;
		ScoreScreenTimer = 2.0f;
	}

	if (m_fScore <= 0 && GameOverSequence == 2)
	{
		GameOverSequence = 3;
		ScoreScreenTimer = 1;
	}

	if (GameOverSequence == 3 && ScoreScreenTimer <= 0)
	{
		GameOverSequence = 4;
		ScoreScreenTimer = 1;
	}

	if (m_fDistanceTraveled > 0 && ScoreScreenTimer2 <= 0 && GameOverSequence == 4)
	{
		ScoreScreenTimer2 = .01f;
		m_fDistanceTraveled -= 1;
		m_fDistanceTraveled2 += 1;
		m_nTotalScore2 += 1;
		ScoreScreenTimer = 1.0f;
	}
	if (m_fDistanceTraveled <= 0 && GameOverSequence == 4)
	{
		GameOverSequence = 5;
		ScoreScreenTimer = 7;
	}
	if (ScoreScreenTimer <= 0 && GameOverSequence == 5)
	{
		
	/*	HighScore* newScore = new HighScore;
		newScore->Name = "Wheeler";
		newScore->Distance = m_fDistanceTraveled2;
		newScore->Score = m_fScore2;*/
		//SaveHighScore(newScore);
		//delete newScore;
		m_fGoToCredits = true;
	}
}


Entity* GameplayState::CreateHero(void) const
{
	Hero* newHero = new Hero;

	newHero->SetImage(m_hHero);
	newHero->SetSize(SGD::Size{ 200, 150 });
	newHero->SetPosition(SGD::Point{80, 377 });

	return newHero;
}

Entity* GameplayState::CreateOrc(Entity* _Hero) const
{
	Orc* newOrc = new Orc;

	newOrc->SetImage(m_hOrc);
	newOrc->SetSize(SGD::Size{ 75, 75 });
	newOrc->SetPosition(SGD::Point{ Game::GetInstance()->GetScreenSize().width + newOrc->GetSize().width, 460 });
	newOrc->SetVelocity(SGD::Vector{ -150, 0 });
	newOrc->SetOwner(_Hero);
	return newOrc;
}

Entity* GameplayState::CreateOrcBow(Entity* _Hero) const
{
	OrcBow* newBowOrc = new OrcBow;

	newBowOrc->SetImage(m_hOrcBow);
	newBowOrc->SetSize(SGD::Size{ 75, 75 });
	newBowOrc->SetPosition(SGD::Point{ Game::GetInstance()->GetScreenSize().width + newBowOrc->GetSize().width, 460 });
	newBowOrc->SetVelocity(SGD::Vector{ -110, 0 });
	newBowOrc->SetOwner(_Hero);
	return newBowOrc;
}

void GameplayState::InitiateBackground() const
{

	for (int i = 1; i < 6; i++)
	{
		int randomize = i * 400;
		Entity* newCloud = CreateCloud();
		newCloud->SetPosition(SGD::Point{ newCloud->GetPosition().x - (float)(rand() % randomize), newCloud->GetPosition().y });
		m_pEntities->AddEntity(newCloud, 0);
		newCloud->Release();
		newCloud = nullptr;
		rand();
		Entity* newMountain = CreateMountain();
		newMountain->SetSize(SGD::Size{ 550, 50 });
		newMountain->SetPosition(SGD::Point{ newMountain->GetPosition().x - (float)(rand() % randomize), newMountain->GetPosition().y });
		m_pEntities->AddEntity(newMountain, 1);
		newMountain->Release();
		newMountain = nullptr;
		rand();

		Entity* newTree = CreateTree();
		newTree->SetPosition(SGD::Point{ newTree->GetPosition().x - (float)(rand() % randomize), newTree->GetPosition().y });
		m_pEntities->AddEntity(newTree, 2);
		newTree->Release();
		newTree = nullptr;
		rand();

		Entity* newPlant = CreatePlant();
		newPlant->SetPosition(SGD::Point{ newPlant->GetPosition().x - (float)(rand() % randomize), newPlant->GetPosition().y });
		m_pEntities->AddEntity(newPlant, 4);
		newPlant->Release();
		newPlant = nullptr;

		Entity* newBush = CreateBush();
		newBush->SetPosition(SGD::Point{ newBush->GetPosition().x - (float)(rand() % randomize), newBush->GetPosition().y });
		m_pEntities->AddEntity(newBush, 3);
		newBush->Release();
		newBush = nullptr;

	}
}
Entity* GameplayState::CreateCloud(void) const
{
	Cloud* newCloud = new Cloud;

	newCloud->SetImage(m_hCloud);
	newCloud->SetSize(SGD::Size{ (float)(rand() % 50 + 140), (float)(rand() % 50 + 120) });
	//newCloud->SetPosition(SGD::Point{ 100, 200 });
	newCloud->SetPosition(SGD::Point{ Game::GetInstance()->GetScreenSize().width + (newCloud->GetSize().width),
		(float)(rand() % 200 - 80) });
	newCloud->SetVelocity(SGD::Vector{ -20, 0 });
	//FOR TESTING
	//newCloud->SetSize(SGD::Size{ 50, 50 }); 
	//newCloud->SetPosition(SGD::Point{ 100, 100 });
	//newCloud->SetVelocity(SGD::Vector{ 0, 0 });
	
	return newCloud;
}
Entity* GameplayState::CreateMountain(void) const
{
	Mountain* newMountain = new Mountain;

	newMountain->SetImage(m_hMountain);
	newMountain->SetSize(SGD::Size{ 550, 50});
	float yOffset = 520 - (230 * (newMountain->GetMountainScale() * 3));
	newMountain->SetPosition(SGD::Point{ Game::GetInstance()->GetScreenSize().width, yOffset});
	newMountain->SetVelocity(SGD::Vector{ -50, 0 });

	return newMountain;
}

Entity* GameplayState::CreateTree(void) const
{
	Tree* newTree = new Tree;

	newTree->SetImage(m_hTree);
	newTree->SetSize(SGD::Size{ 50, 50 });
	float yOffset = 680 - (125 * (newTree->GetTreeScale() * 4));
	newTree->SetPosition(SGD::Point{ Game::GetInstance()->GetScreenSize().width, yOffset - 150 });
	newTree->SetVelocity(SGD::Vector{ -90, 0 });

	return newTree;
}

Entity* GameplayState::CreateBush(void) const
{
	Bush* newBush = new Bush;

	newBush->SetImage(m_hBush);
	newBush->SetSize(SGD::Size{ 50, 50 });
	newBush->SetPosition(SGD::Point{ Game::GetInstance()->GetScreenSize().width, 425 });
	newBush->SetVelocity(SGD::Vector{ -110, 0 });

	return newBush;
}

Entity* GameplayState::CreatePlant(void) const
{
	Bush* newBush = new Bush;

	int x = rand() % 2;
	if (x)
		newBush->SetImage(m_hPlantBlue);
	else
		newBush->SetImage(m_hPlantRed);
	newBush->SetSize(SGD::Size{ 30, 30 });
	float yOffset = 250 - (120 * newBush->GetBushScale());
	newBush->SetPosition(SGD::Point{ Game::GetInstance()->GetScreenSize().width, 275 + yOffset });
	newBush->SetVelocity(SGD::Vector{ -120, 0 });

	return newBush;
}

Entity* GameplayState::CreateGround(void) const
{
	Ground* ground = new Ground;
	ground->SetImage(m_hGround);
	ground->SetPosition(SGD::Point{ -10, 525 });
	ground->SetSize(SGD::Size{ Game::GetInstance()->GetScreenSize().width, 400});

	return ground;
}
Entity* GameplayState::CreateArrow(Entity* _Hero) const
{
	float pull = dynamic_cast<Hero*>(_Hero)->GetPull();
	Arrow* arrow = new Arrow;
	arrow->SetImage(m_hArrow);
	arrow->SetPosition(SGD::Point{ _Hero->GetPosition().x + 130, 460 });
	//arrow->SetPosition(GameplayState::GetInstance()->m_eHero->GetPosition());
	arrow->SetSize(SGD::Size{ 15, 15 });
	arrow->SetOwner(_Hero);
	SGD::Vector newVec = SGD::Vector{ 250, -350 };
	if (pull > 1.0f)
		pull = 1.0f;
	newVec *= pull * 2;

	arrow->SetVelocity(newVec);
	arrow->SetRotation(5);

	return arrow;
}
Entity* GameplayState::CreateArrowOrc(Entity* _OrcBow) const
{

	float pull = dynamic_cast<OrcBow*>(_OrcBow)->GetPull();
	Arrow* arrow = new Arrow;
	arrow->SetImage(m_hArrow);
	arrow->SetPosition(SGD::Point{ _OrcBow->GetPosition().x, 400 });
	arrow->SetSize(SGD::Size{ 15, 15 });
	arrow->SetOwner(_OrcBow);
	SGD::Vector newVec = SGD::Vector{ -250, -350 };

	if (pull > 1.0f)
		pull = 1.0f;

	arrow->SetRotation(4.3f);
	if (pull < 0.5f)
	{
		newVec.x = -450;
		newVec.y = -100;
		arrow->SetRotation(3.4f);
	}


	pull += (rand() % 20 - 10) *.015f;

	newVec *= pull * 2;


	arrow->SetVelocity(newVec);
	//arrow->SetRotation(4.3f);


	return arrow;
}

void GameplayState::RandomlyGenerateBackground(float elapsedTime)
{
	m_fcloudTimer -= elapsedTime;
	m_fmountainTimer -= elapsedTime;
	m_fTreeTimer -= elapsedTime;
	m_fBushTimer -= elapsedTime;
	m_fPlantTimer -= elapsedTime;

	float Variable = -(Game::GetInstance()->GetSpeedVar()* 2.5f);
	
	if (m_fcloudTimer <= 0)
	{
		m_fcloudTimer = (float)(rand() % 7) + 9;
		if (m_fcloudTimer < 0)
			m_fcloudTimer = (float)(rand() % 2) + 1;
		Entity* newCloud = CreateCloud();
		m_pEntities->AddEntity(newCloud, 0);
		newCloud->Release();
		newCloud = nullptr;
	}
	
	if (m_fmountainTimer <= 0)
	{
		m_fmountainTimer = (float)(rand() % 3) + 2;
		if (m_fmountainTimer < 0)
			m_fmountainTimer = (float)(rand() % 2) + 1;
		Entity* newMountain = CreateMountain();
		m_pEntities->AddEntity(newMountain, 1);
		newMountain->Release();
		newMountain = nullptr;
	}
	if (m_fTreeTimer <= 0)
	{
		m_fTreeTimer = (float)(rand() % 2) + 2;
		if (m_fTreeTimer < 0)
			m_fTreeTimer = (float)(rand() % 2) + 1;
		Entity* newTree = CreateTree();
		m_pEntities->AddEntity(newTree, 2);
		newTree->Release();
		newTree = nullptr;
	}
	if (m_fPlantTimer <= 0)
	{
		m_fPlantTimer = (float)(rand() % 4) + 3;
		if (m_fPlantTimer < 0)
			m_fPlantTimer = (float)(rand() % 2) + 1;
		Entity* newPlant = CreatePlant();
		m_pEntities->AddEntity(newPlant, 4);
		newPlant->Release();
		newPlant = nullptr;
	}
	if (m_fBushTimer <= 0)
	{
		m_fBushTimer = (float)(rand() % 4) + 6;
		if (m_fBushTimer < 0)
			m_fBushTimer = (float)(rand() % 2) + 1;
		Entity* newBush = CreateBush();
		m_pEntities->AddEntity(newBush, 3);
		newBush->Release();
		newBush = nullptr;
	}
}

//*********************************************************************//
// MessageProc
//	- process messages queued in the MessageManager
//	- STATIC METHOD!!!
//		- no invoking object!
//		- MUST USE THE SINGLETON
/*static*/ void GameplayState::MessageProc( const SGD::Message* pMsg )
{
/* Show warning when a Message ID enumerator is not handled */
#pragma warning( push )
#pragma warning( 1 : 4061 )


	// What type of message?
	switch( pMsg->GetMessageID() )
	{
	case MessageID::MSG_DESTROY_ENTITY:
	{
		  // Downcast the message to the actual child type
		  const DestroyEntityMessage* pDestroyMsg =
			  dynamic_cast< const DestroyEntityMessage* >(pMsg);

		  SGD_ASSERT(pDestroyMsg != nullptr,
			  "GameplayState::MessageProc - MSG_DESTROY_ENTITY is not a DestroyEntityMessage!");

		  // Access the entity attached to the child message
		  Entity* pEntity = pDestroyMsg->GetEntity();

		  // Use the singleton to access members!
		  GameplayState::GetInstance()->m_pEntities->RemoveEntity(pEntity);
		  break;
	}

	case MessageID::MSG_CREATE_ARROW:
	{
			const CreateArrowMessage* pArrowMsg =
				dynamic_cast< const CreateArrowMessage* >(pMsg);

			//Hero* tempHero = pArrowMsg->GetHero();
			Entity* newArrow = GameplayState::GetInstance()->CreateArrow(pArrowMsg->GetHero());
			//tempHero = nullptr;

			GameplayState::GetInstance()->m_pEntities->AddEntity(newArrow, 12);
			GameplayState::GetInstance()->AddArrowCount(1);
			newArrow->Release();
			newArrow = nullptr;
			break;
	}
	case MessageID::MSG_CREATE_ARROW_ORC:
	{
			const CreateOrcArrowMsg* pArrowMsg =
				dynamic_cast< const CreateOrcArrowMsg* >(pMsg);

			//Hero* tempHero = pArrowMsg->GetHero();
			Entity* newArrow = GameplayState::GetInstance()->CreateArrowOrc(pArrowMsg->GetOrc());
			//tempHero = nullptr;

			GameplayState::GetInstance()->m_pEntities->AddEntity(newArrow, 12);
			newArrow->Release();
			newArrow = nullptr;

			break;
	}
	case MessageID::MSG_SLASH:
	{
		 const SlashMessage* pSlashMsg =
			 dynamic_cast<const SlashMessage*> (pMsg);
		GameplayState::GetInstance()->m_pEntities->CheckCollisions(7, 8);
		break;
	}
	case MessageID::MSG_ADD_SCORE:
	{
		 if (GameplayState::GetInstance()->GetGameStatus() == false)
		{
			const AddScoreMessage* AddScoreMsg =
				dynamic_cast<const AddScoreMessage*> (pMsg);

			if (AddScoreMsg->GetHero()->GetType() == 2)
				GameplayState::GetInstance()->AddScore(1);
		}
		break;
	}
	case MessageID::MSG_CREATE_ORC:
	{
		  const CreateOrcMessage* CreateOrcMsg =
			  dynamic_cast<const CreateOrcMessage*> (pMsg);

		  Entity* newOrc;

		  if ((rand()% 20) < 8)
			newOrc = GameplayState::GetInstance()->CreateOrcBow(CreateOrcMsg->GetHero());
		  else
			newOrc = GameplayState::GetInstance()->CreateOrc(CreateOrcMsg->GetHero());

		GameplayState::GetInstance()->m_pEntities->AddEntity(newOrc, 8);
		GameplayState::GetInstance()->AddOrcArrowCount(1);
		newOrc->Release();
		newOrc = nullptr;

		break;
	}
	case MessageID::MSG_GAME_OVER:
	{
		GameplayState::GetInstance()->SetGameStatus(true);

		break;
	}
		break;
	default:
	case MessageID::MSG_UNKNOWN:
		SGD_PRINT( L"GameplayState::MessageProc - unknown message id\n" );
		break;
	}


/* Restore previous warning levels */
#pragma warning( pop )
}

void GameplayState::SaveHighScore(HighScore* _newScore)
{

	//m_vHighScores.push_back(_newScore);
	//std::ofstream bout;
	//bout.open("HighScores.bin", std::ios_base::binary | std::ios_base::app);

	//if (bout.is_open())
	//{
	//	bout.write((char*)&m_vHighScores[0], sizeof(HighScore));
	//	bout.close();
	//}
}

void GameplayState::LoadHighScore()
{
	//std::ifstream bin;
	//bin.open("HighScores.bin", std::ios_base::binary | std::ios_base::in);

	//if (bin.is_open())
	//{
	//	int size = 0;

	//	bin.seekg(0, std::ios_base::end);
	//	size = (int)(bin.tellg());
	//	size /= sizeof(HighScore);

	//	bin.seekg(0, std::ios_base::beg);

	//	HighScore* tempScore;
	//	m_vHighScores.resize(size);
	//	bin.read((char*)&m_vHighScores[0], (sizeof(HighScore)) * m_vHighScores.size());

	//	//delete tempScore;
	//	bin.close();
	//}

}

void GameplayState::AddController()
{







}

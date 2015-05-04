/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 06/04/2015 | Main pour le jeu Isaac		                 */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#pragma comment( lib, "SDL2.lib" ) 
#pragma comment( lib, "SDL2main.lib" )
#pragma comment( lib, "SDL2_mixer.lib" )
#pragma comment( lib, "SDL2_ttf.lib" )
#pragma comment( lib, "SDL2_image.lib" )

#include "kr_common.h"
#include "kr_log.h"
#include "kr_util.h"
#include "kr_input.h"
#include "kr_text.h"
#include "kr_level.h"
#include "kr_map.h"
#include "kr_config.h"
#include "kr_sound.h"
#include "kr_fps.h"
#include "entite.h"
#include "kr_sprite.h"
#include "HUD.h"
#include "Menu.h"
#include "level_editor.h"
#include "level_state.h"
#include "weapon.h"
#include "nature.h"
//http://noproblo.dayjo.org/ZeldaSounds/
 

int Isaac(int*argc, char**argv);
int Editor(void);

Uint32 PeriodicEvent(void);
int main(int argc, char** argv)
{

	Kr_Log_Init(KR_LOG_INFO); // Mise en place d'un fichier de log

	if (GAME) Isaac(&argc, argv);
	else Editor();

	Mix_CloseAudio();	// On quitte SDL_MIXER
	TTF_Quit();			// On quitte SDL_TTF
	SDL_Quit();			// On quitte SDL
	Kr_Log_Quit();		// On ferme les logs

	return EXIT_SUCCESS;
}


/*!
*  \fn     int Isaac(int *argc, char **argv)
*  \brief  Function to launch the game
*
*  \return EXIT_SUCCESS if everything is ok, EXIT_FAILURE otherwise
*/
int Isaac(int *argc, char **argv)
{
	SDL_Window *pWindow = NULL;
	if (!Kr_Init())
	{
		exit(EXIT_FAILURE);
	}
	/* ========================================================================= */
	/*                           CONFIGURATION GENERALE                          */
	/* ========================================================================= */

	Kr_Input inEvent; // Structure pour la gestion des événements
	
	/* Création de la fenêtre */
	pWindow = SDL_CreateWindow("Jeu 2D - Isaac", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, KR_WIDTH_WINDOW, KR_HEIGHT_WINDOW, SDL_WINDOW_SHOWN); // SDL_WINDOW_FULLSCREEN
	if (pWindow == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't create the Window : %s\n", SDL_GetError());
		SDL_Quit();
	}
	SDL_Surface *pscreenSurface = SDL_GetWindowSurface(pWindow);
	if (pscreenSurface == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't create the Surface Window : %s\n", SDL_GetError());
		SDL_Quit();
	}
	/* Création du renderer */
	SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	if (pRenderer == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't create the Renderer", SDL_GetError());
		SDL_Quit();
	}
	/* Initialisation de la structure pour gérer les événements*/
	InitEvents(&inEvent);

	/* Copie des fichiers maps au besoin */
	if (Kr_Map_CopyLevelFiles(LOAD_BACKUP_LEVEL) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't copy all the levels from /maps/backup");
		SDL_Quit();
	}
	/* ========================================================================= */
	/*                                  PRECACHE                                 */
	/* ========================================================================= */
	/* Variable de boucle (uniquement) */
	Uint32 i = 0, j = 0;
	/* Préparation d'images que l'on souhaitera afficher */
	Kr_Sprite	 *pSpritePlayer = NULL;
	Entity		 *pPlayer = NULL;
	SDL_Rect     *pRectPositionPlayer = (SDL_Rect*)UTIL_Malloc(sizeof(SDL_Rect));

	pRectPositionPlayer->x = 250;
	pRectPositionPlayer->y = 250;
	pRectPositionPlayer->w = 32; //Il est nécessaire de fournir la taille de l'image avec .w et .h sinon rien n'apparaitra
	pRectPositionPlayer->h = 32;


	/* Chargement des sprites */
	pSpritePlayer = Kr_Sprite_Init("zelda");			//D'abord création et load du sprite (ici le nom du sprite est "sprites/zelda_sud.png"
	if (Kr_Sprite_Load(pSpritePlayer, sud, 26, 136, 8, pRectPositionPlayer, pRenderer) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite !\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}


	/* Chargement des personnages */
	pPlayer = Entity_Init("zelda");				//Ensuite création et load du sprite (il faut préciser la taille de l'image png)

	if (Entity_Load(pPlayer, 100, 50, MOVESPEED, normal, FALSE, pSpritePlayer) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the entity !\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	/*Chargement de l'arme */
	Weapon *pistoletLumière = Weapon_Init("pistolet lumière");
	Weapon_Load(pistoletLumière, "bullet", 100, 50, 500);
	ChangeWeapon(pPlayer, pistoletLumière);

	/* Préparation d'une Texture contenant un message via util.c*/
	SDL_Rect     textPosition;
	SDL_Color    couleur = { 123, 255, 0 };
	SDL_Texture *pTextureText = NULL;
	TTF_Font    *pFont = NULL;
	char         szCompteur[100] = " ";
	textPosition.x = 0;
	textPosition.y = 0;
	pFont = Kr_Text_OpenFont("cour", 25);
	TTF_SetFontStyle(pFont, TTF_STYLE_BOLD);



	/* Chargement de la map */
	Kr_Map *pMap = NULL;
	pMap = Kr_Map_Init("world");
	if (!pMap)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't Load the map\n");
		exit(EXIT_FAILURE);
	}

	/* Chargement du niveau */
	Boolean   bChangeLevel = TRUE;
	Kr_Level *pCurrentLevel = NULL;
	Uint32    iCurrentLevelNumber = 1;
	Level_State *pCurrentLevelState;

	pCurrentLevelState = Level_State_Init(pPlayer);


	/* Préparation de la gestion des FPS */
	SDL_Texture *pTextureFPS = NULL;
	TTF_Font	*pFontFPS = NULL;
	Kr_Fps		*pFPS = NULL;
	SDL_Color    colorFPS = { 0, 10, 220 };
	SDL_Rect	 rectPositionFPS;
	Uint32       iPreviousTime = 0, iCurrentTime = 0;

	rectPositionFPS.x = 1150;
	rectPositionFPS.y = 685;
	pFontFPS = Kr_Text_OpenFont("cour", 18);
	TTF_SetFontStyle(pFontFPS, TTF_STYLE_ITALIC);

	pFPS = Kr_Fps_Init(pRenderer, pFontFPS, &rectPositionFPS, colorFPS, TRUE);
	if (pFPS == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't initialize the FPS structure\n");
		exit(EXIT_FAILURE);
	}

	/* Musique */
	 // Pas encore géré la continuité de la musique
	Kr_Music *pMusicCurrent = NULL;
	Kr_Music *pMusicOld = NULL;
	Uint32	  iMusicLen = 0;
	pMusicCurrent = Kr_Sound_InitMusic();
	pMusicOld = Kr_Sound_InitMusic();
	Mix_VolumeMusic(5);		// Réglage du volume de la musique (0 à 128) innefficace ?

	/* Sound */
	Mix_AllocateChannels(10);
	
	/* Gestion des entitées */
	Uint32 iCodeUpdateEntity = 1;

	/* Oiseau */
	Sint32 movex = 0, movey = 0;
	Boolean bCheckTypeOiseau = FALSE, bActiverCalculOiseau = FALSE, bDrawOiseau = FALSE;
	Entity *pOiseau1 = NULL;
	Entity *pOiseau2 = NULL;
	Kr_Sound *pSndOiseau1 = NULL;
	Kr_Sound *pSndOiseau2 = NULL;
	Uint32 iTypeOiseau = 1;
	pOiseau1 = ChargementOiseau(pRenderer, 1);
	pOiseau2 = ChargementOiseau(pRenderer, 2);
	pSndOiseau1 = Kr_Sound_Alloc("seagull");
	pSndOiseau2 = Kr_Sound_Alloc("eagle");


	/* Buisson */
	Boolean bDrawBuisson = FALSE;
	Entity *pBuisson1 = NULL;
	Entity *pBuisson2 = NULL;
	Kr_Sound *pSndBuisson = NULL;
	Uint32 iPositionBuissonX = 0, iPositionBuissonY = 0, iValeurBuisson = 0;
	pSndBuisson = Kr_Sound_Alloc("buisson");
	pBuisson1 = ChargementBuisson(pRenderer, 1);
	pBuisson2 = ChargementBuisson(pRenderer, 2);
	
	/* Papillon */
	Boolean bCheckPapillon = FALSE;
	Uint32 iNumberPapillon = 0;
	Entity *pPapillon = NULL;
	pPapillon = ChargementPapillon(pRenderer);

	/* Pigeon effrayé */
	Entity *pPigeonVol = NULL;
	Boolean bFearedPigeon = FALSE, bDrawPigeonVol = FALSE, bOldPigeon = FALSE;
	Uint32  iIndexEntity = 0, iCoordXDebut = 0, iCoordYDebut = 0, iCoordXFin = 0, iCoordYFin = 0;
	Kr_Sound *pSndPigeon = NULL;
	pSndPigeon = Kr_Sound_Alloc("pigeon");
	pPigeonVol = ChargementPigeonVolant(pRenderer);

	/* Evenement périodique */
	Uint32 iPeriodicEvent = 0;

	/* ========================================================================= */
	/*                                 EVENEMENT                                 */
	/* ========================================================================= */
	while (!inEvent.szKey[SDL_SCANCODE_ESCAPE] && !inEvent.bQuit)
	{
		/* Faire au début sinon crash*/
		if (bChangeLevel == TRUE)
		{
			bChangeLevel = FALSE;
			pCurrentLevel = Kr_Level_Change(pCurrentLevel, iCurrentLevelNumber, pRenderer);
			Level_State_Free(pCurrentLevelState,FALSE);
			pCurrentLevelState = Level_State_Init(pPlayer);
			Level_State_Load(pCurrentLevelState, pCurrentLevel, pRenderer);
			Mix_PlayMusic(pCurrentLevel->pMusic->pMsc, -1);
			bCheckPapillon = TRUE;
			bCheckTypeOiseau = TRUE;
			bFearedPigeon = FALSE;
		}

		UpdateEvents(&inEvent);
		
		/* Mise à jour des entitées (dont le player)*/
		iCodeUpdateEntity = updateAllEntities(pRenderer, pCurrentLevelState, inEvent);
		if (iCodeUpdateEntity == FALSE)
		{
			Kr_Log_Print(KR_LOG_ERROR, "Couldn't update an entity\n");
			SDL_Quit();
			return FALSE;
		}

		/* Mise à jour des projectiles du personnage */
		updateProjectilesWeapon(pRenderer, pCurrentLevelState, pCurrentLevelState->pPlayer->pWeapon);

		/* Controle du tir du personnage */
		shoot(inEvent, pPlayer, pRenderer);

		if (inEvent.szMouseButtons[0])
		{
			pPlayer->pSprEntity->pRectPosition->x = inEvent.iMouseX;
			pPlayer->pSprEntity->pRectPosition->y = inEvent.iMouseY;
			Kr_Log_Print(KR_LOG_INFO, "CLIQUE GAUCHE : %d %d \n", inEvent.iMouseX, inEvent.iMouseY);
			inEvent.szMouseButtons[0] = 0; // Un seul clique, si je ne met pas ça, le son sera joué en boucle. La l'utilisateur va devoir relever son doigt
		}
		if (inEvent.szMouseButtons[2])
		{
			//Kr_Log_Print(KR_LOG_INFO, "CLIQUE DROIT\n");
			inEvent.szMouseButtons[2] = 0; // Un seul clique
		}
		if (inEvent.szKey[SDL_SCANCODE_F]) // Gestion de l'affichage des FPS
		{
			if (pFPS->bMustShow) pFPS->bMustShow = FALSE;
			else pFPS->bMustShow = TRUE;
			inEvent.szKey[SDL_SCANCODE_F] = 0;
		}
		if (inEvent.szKey[SDL_SCANCODE_E])
		{
			Kr_Level_Interraction(pCurrentLevel, pPlayer);
			inEvent.szKey[SDL_SCANCODE_E] = 0;
		}

		iCurrentLevelNumber = Kr_Map_ShouldChangeLevel(pMap, pCurrentLevel, pPlayer);
		if (iCurrentLevelNumber)
		{
			bChangeLevel = TRUE;
		}

		/* ========================================================================= */
		/*                              FPS & EVENEMENT                              */
		/* ========================================================================= */

		Kr_Fps_Wait(pFPS, &iCurrentTime, &iPreviousTime, KR_FPS);
		iPeriodicEvent = PeriodicEvent();
		switch (iPeriodicEvent)
		{
			case 0: // Aucun événement
				bActiverCalculOiseau = FALSE;
				break;
			case 1:
				if (bDrawOiseau == FALSE) bActiverCalculOiseau = TRUE;
				break;
			default :
				bActiverCalculOiseau = FALSE;
				break;
		}
		/* Gestion des oiseaux*/
		if (bCheckTypeOiseau == TRUE)
		{
			iTypeOiseau = CalculTypeOiseau(bCheckTypeOiseau, pCurrentLevel);
			bCheckTypeOiseau = FALSE;
		}		
		if (iTypeOiseau == 1)// && bDrawOiseau == FALSE)
		{
			CalculPassageOiseau(pOiseau1, bActiverCalculOiseau, &movex, &movey);
			bDrawOiseau = PassageOiseau(pOiseau1, bActiverCalculOiseau, movex, movey, pRenderer, pSndOiseau1);
		}
		if (iTypeOiseau == 2)// && bDrawOiseau == FALSE)
		{
			CalculPassageOiseau(pOiseau2, bActiverCalculOiseau, &movex, &movey);
			bDrawOiseau = PassageOiseau(pOiseau2, bActiverCalculOiseau, movex, movey, pRenderer, pSndOiseau2);
		}

		/* Gestion des buissons*/
		iValeurBuisson = DetectPassageBuisson(pCurrentLevel, pPlayer, &iPositionBuissonX, &iPositionBuissonY);
		if (iValeurBuisson == 1) bDrawBuisson = AnimationBuisson(pBuisson1, TRUE, iPositionBuissonX, iPositionBuissonY, pRenderer,pSndBuisson);
		if (iValeurBuisson == 2) bDrawBuisson = AnimationBuisson(pBuisson2, TRUE, iPositionBuissonX, iPositionBuissonY, pRenderer,pSndBuisson);

		/* Gestion des papillons*/
		 // NON DEVELOPPE
		if (bCheckPapillon == TRUE)
		{
			CalculApparitionPapillon(bCheckPapillon, pCurrentLevel, &iNumberPapillon);
			bCheckPapillon = FALSE;
		}

		/* Gestion des pigeons */
		if (iCodeUpdateEntity == 2) // Oiseau effrayé
		{
			Entity	  **aEntity = pCurrentLevelState->aEntityLevel;
			for (iIndexEntity = 0; iIndexEntity < pCurrentLevelState->iNbEntities; iIndexEntity++)
			{
				if ((bDrawPigeonVol == FALSE) && (strcmp((*(aEntity + iIndexEntity))->strEntityName, "pigeon1") == 0))
				{
					(*(aEntity + iIndexEntity))->state = invisible;
					bFearedPigeon = TRUE;
					iCoordXDebut = (*(aEntity + iIndexEntity))->pSprEntity->pRectPosition->x;
					iCoordYDebut = (*(aEntity + iIndexEntity))->pSprEntity->pRectPosition->y;
				}
			}				
		}
		else bFearedPigeon = FALSE;
		bOldPigeon = bDrawPigeonVol;
		bDrawPigeonVol = PigeonVol(pPigeonVol, bFearedPigeon, pRenderer, pCurrentLevel, pSndPigeon, iCoordXDebut, iCoordYDebut, &iCoordXFin, &iCoordYFin);
		if ((bDrawPigeonVol == FALSE) && (bOldPigeon == TRUE)) // Atterissage de l'oiseau
		{
			Entity	  **aEntity = pCurrentLevelState->aEntityLevel;
			for (iIndexEntity = 0; iIndexEntity < pCurrentLevelState->iNbEntities; iIndexEntity++)
			{
				if ((strcmp((*(aEntity + iIndexEntity))->strEntityName, "pigeon1") == 0))
				{
					(*(aEntity + iIndexEntity))->state = normal;
					bFearedPigeon = TRUE;
					(*(aEntity + iIndexEntity))->pSprEntity->pRectPosition->x = iCoordXFin;
					(*(aEntity + iIndexEntity))->pSprEntity->pRectPosition->y = iCoordYFin;
				}
			}
		}

		/* ========================================================================= */
		/*                                  DIVERS                                   */
		/* ========================================================================= */

		pTextureText = Kr_Text_FontCreateTexture(pRenderer, pFont, szCompteur, couleur, TRUE, &textPosition); // Création d'une texture contenant le texte d'une certaine couleur avec le mode Blended  
		sprintf(szCompteur, "Cursor : X : %d Y : %d   %s %d | Tile %d %d", pPlayer->pSprEntity->pRectPosition->x, pPlayer->pSprEntity->pRectPosition->y, pCurrentLevel->szLevelName, pCurrentLevel->iLevelNum, Kr_Level_GetTile(pCurrentLevel, inEvent.iMouseX, inEvent.iMouseY),iNumberPapillon);//)pMonLevel->rScrollWindow->x, pMonLevel->rScrollWindow->y // Affichage coordonnée de la map
		// inEvent.iMouseX, inEvent.iMouseY
		/* ========================================================================= */
		/*                                  RENDER                                   */
		/* ========================================================================= */

		SDL_RenderClear(pRenderer); // Dans un premier temps on Clear le renderer
		Kr_Level_Draw(pRenderer, pCurrentLevel);
		if ((bDrawBuisson == TRUE) && (iValeurBuisson == 1)) Entity_Draw(pRenderer, pBuisson1);
		if ((bDrawBuisson == TRUE) && (iValeurBuisson == 2)) Entity_Draw(pRenderer, pBuisson2);
		drawAllEntities(pCurrentLevelState, pRenderer);
		drawProjectilesWeapon(pPlayer->pWeapon->plProjectile, pRenderer);
		if ((bDrawPigeonVol == TRUE)) Entity_Draw(pRenderer, pPigeonVol);
		if ((bDrawOiseau == TRUE) && (iTypeOiseau == 1)) Entity_Draw(pRenderer, pOiseau1);
		if ((bDrawOiseau == TRUE) && (iTypeOiseau == 2)) Entity_Draw(pRenderer, pOiseau2);
		SDL_RenderCopy(pRenderer, pTextureText, NULL, &textPosition);
		Kr_FPS_Show(pFPS);
		SDL_RenderPresent(pRenderer); // Lorsque toutes les surfaces ont été placé on affiche le renderer (l'écran quoi...)
		UTIL_FreeTexture(&pTextureText); // Comme on recréé la texture en permanence dans la boucle il faut la free également dans la boucle
	}


	/* ========================================================================= */
	/*                            LIBERATION MEMOIRE                             */
	/* ========================================================================= */

	UTIL_FreeTexture(&pTextureText);	// Libération mémoire de la texture du Texte ttf
	SDL_DestroyRenderer(pRenderer);	    // Libération mémoire du renderer
	SDL_DestroyWindow(pWindow);			// Libération mémoire de la fenetre
	Kr_Text_CloseFont(&pFont);			// Libération mémoire de la police
	Kr_Text_CloseFont(&pFontFPS);		// Libération mémoire de la police
	Kr_Level_Free(pCurrentLevel);
	Entity_Free(pOiseau1);
	Entity_Free(pOiseau2);
	Entity_Free(pBuisson1);
	Entity_Free(pBuisson2);
	Entity_Free(pPapillon);
	Entity_Free(pPigeonVol);
	Kr_Sound_Free(&pSndOiseau1);
	Kr_Sound_Free(&pSndOiseau2);
	Kr_Sound_Free(&pSndBuisson);
	Kr_Sound_Free(&pSndPigeon);
	//Entity_Free(pZelda);				// Libération mémoire du zelda est déjà fait si On le précise dans Level_State
	Level_State_Free(pCurrentLevelState, TRUE); // Libération mémoire des données du niveau
	Kr_Map_Free(pMap);


	return EXIT_SUCCESS;
}




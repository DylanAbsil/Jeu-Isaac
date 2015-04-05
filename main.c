/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 18/02/2015 | TTF encapsuler dans kr_text                  */
/*               |            | Renderer en variable global ? Si oui où ?    */
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





//void UpdatePlayerVector(Kr_Input inEvent, Kr_Level *pLevel, SDL_Rect *pPlayer);
//void GetVector(Kr_Input inEvent, Sint32 *vx, Sint32 *vy);

 
int main(int argc, char** argv)
{

	Kr_Log_Init(KR_LOG_INFO); // Mise en place d'un fichier de log

	/* ========================================================================= */
	/*                          INITIALISATION DE LA SDL                         */
	/* ========================================================================= */
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
		return FALSE;
	}
	SDL_Surface *pscreenSurface = SDL_GetWindowSurface(pWindow);
	if (pscreenSurface == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't create the Surface Window : %s\n", SDL_GetError());
		SDL_Quit();
		return FALSE;
	}
	/* Création du renderer */
	SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	if (pRenderer == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't create the Renderer", SDL_GetError());
		SDL_Quit();
		return FALSE;
	}
	/* Initialisation de la structure pour gérer les événements*/
	InitEvents(&inEvent);

	/* ========================================================================= */
	/*                                  PRECACHE                                 */
	/* ========================================================================= */

	/* Préparation d'images que l'on souhaitera afficher */
	Kr_Sprite	 *pSpriteZelda = NULL;
	Entity		 *pZelda = NULL;
	SDL_Rect     rectPositionZelda;
	Uint32		 tempoAnim = 0;

	rectPositionZelda.x = 0;
	rectPositionZelda.y = 0;
	rectPositionZelda.w = 32; //Il est nécessaire de fournir la taille de l'image avec .w et .h sinon rien n'apparaitra
	rectPositionZelda.h = 32;

	/* Chargement des sprites */
	pSpriteZelda = Kr_Sprite_Init("zelda");			//D'abord création et load du sprite (ici le nom du sprite est "sprites/zelda_sud.png"
	
	if (Kr_Sprite_Load(pSpriteZelda, 26, 240, 8, &rectPositionZelda, pRenderer) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite !\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}


	/* Chargement des personnages */
	pZelda = Entity_Init("zelda");				//Ensuite création et load du sprite (il faut préciser la taille de l'image png)

	if (Entity_Load(pZelda, 100, 50, pSpriteZelda) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the entity !\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}


	/* Préparation d'une Texture contenant un message via util.c*/
	SDL_Rect     textPosition;
	SDL_Color    couleur = { 123, 255, 0 };
	SDL_Texture *pTextureText = NULL;
	TTF_Font    *pFont = NULL;
	char         szCompteur[100] = " ";// Tableau contenant la valeur du compteur
	textPosition.x = 0;
	textPosition.y = 0;
	pFont = Kr_Text_OpenFont("cour", 25);
	TTF_SetFontStyle(pFont, TTF_STYLE_BOLD);



	/* Chargement de la map */
	Kr_Map *pMap = NULL;
	pMap = Kr_Map_Init("maps_world");
	if (!pMap)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't Load the map\n");
		exit(EXIT_FAILURE);
	}

	/* Chargement du niveau */
	Boolean   bChangeLevel = TRUE;
	Kr_Level *pCurrentLevel = NULL;
	Uint32    iCurrentLevelNumber = 1;

	/* Chargement des sons*/
	Kr_Sound *pSon = NULL; // Initialisation
	Mix_AllocateChannels(1); // Allocation du nombre de cannaux

	pSon = Kr_Sound_Alloc("arrow_x"); // Cette fonction va renvoyé une structure Kr_Sound avec le son chargé, ou NULL si une erreur
	if (!pSon)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Le son n'a pas été chargé correctement\n");
	}

	/* Préparation de la gestion des FPS */
	SDL_Texture *pTextureFPS = NULL;
	TTF_Font	*pFontFPS	 = NULL;
	Kr_Fps		*pFPS		 = NULL;
	SDL_Color    colorFPS	 = { 0, 10, 220 };
	SDL_Rect	 rectPositionFPS;
	Uint32   iPreviousTime = 0, iCurrentTime = 0; 

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
		}

		UpdateEvents(&inEvent);

		/* Mise à jour des coordonnées du personnage*/
		if (updateEntityVector(inEvent, pCurrentLevel, pZelda, &tempoAnim, pRenderer) == FALSE){				//Update la position et l'animation du perso principal
			Kr_Log_Print(KR_LOG_ERROR, "Couldn't update player vector\n");
			SDL_Quit();
			return FALSE;
		}


		if (inEvent.szMouseButtons[0])
		{
			pZelda->pSprEntity->pRectPosition->x = inEvent.iMouseX;
			pZelda->pSprEntity->pRectPosition->y = inEvent.iMouseY;
			Kr_Log_Print(KR_LOG_INFO, "CLIQUE GAUCHE : %d %d \n", inEvent.iMouseX, inEvent.iMouseY);
			inEvent.szMouseButtons[0] = 0; // Un seul clique, si je ne met pas ça, le son sera joué en boucle. La l'utilisateur va devoir relever son doigt
		}
		if (inEvent.szMouseButtons[2])
		{
			//Kr_Log_Print(KR_LOG_INFO, "CLIQUE DROIT\n");
			inEvent.szMouseButtons[2] = 0; // Un seul clique
		}
		if (inEvent.szKey[SDL_SCANCODE_P])
		{
			inEvent.szKey[SDL_SCANCODE_P] = 0;
			Kr_Sound_Play(pSon, 0, 100, 0); // Joue le son contenu dans la structure pSon, sur le channel 0 avec un volume de 100 et 0 répétions
		}
		if (inEvent.szKey[SDL_SCANCODE_KP_1])
		{
			bChangeLevel = TRUE;
			iCurrentLevelNumber = 4;
			inEvent.szKey[SDL_SCANCODE_KP_1] = 0;
		}
		if (inEvent.szKey[SDL_SCANCODE_KP_2])
		{
			bChangeLevel = TRUE;
			iCurrentLevelNumber = 4;
			inEvent.szKey[SDL_SCANCODE_KP_2] = 0;
		}
		if (inEvent.szKey[SDL_SCANCODE_F]) // Gestion de l'affichage des FPS
		{
			if (pFPS->bMustShow) pFPS->bMustShow = FALSE;
			else pFPS->bMustShow = TRUE;
			inEvent.szKey[SDL_SCANCODE_F] = 0;
		}
		if (inEvent.szKey[SDL_SCANCODE_E])
		{
			iCurrentLevelNumber = Kr_Map_ShouldChangeLevel(pMap, pCurrentLevel, pZelda);
			if (iCurrentLevelNumber)
			{
				bChangeLevel = TRUE;
			}
			inEvent.szKey[SDL_SCANCODE_E] = 0;
		}

		/* ========================================================================= */
		/*                                    FPS                                    */
		/* ========================================================================= */

		Kr_Fps_Wait(pFPS, &iCurrentTime, &iPreviousTime, KR_FPS);


		/* ========================================================================= */
		/*                                  DIVERS                                   */
		/* ========================================================================= */
	
		pTextureText = Kr_Text_FontCreateTexture(pRenderer, pFont, szCompteur, couleur, TRUE, &textPosition); // Création d'une texture contenant le texte d'une certaine couleur avec le mode Blended  
		sprintf(szCompteur, "Cursor : X : %d Y : %d   %s %d", pZelda->pSprEntity->pRectPosition->x, pZelda->pSprEntity->pRectPosition->y, pCurrentLevel->szLevelName,pCurrentLevel->iLevelNum);//)pMonLevel->rScrollWindow->x, pMonLevel->rScrollWindow->y // Affichage coordonnée de la map
		// inEvent.iMouseX, inEvent.iMouseY
		/* ========================================================================= */
		/*                                  RENDER                                   */
		/* ========================================================================= */

		SDL_RenderClear(pRenderer); // Dans un premier temps on Clear le renderer
		Kr_Level_Draw(pRenderer, pCurrentLevel);
		Entity_Draw(pRenderer, pZelda);
		//SDL_RenderCopy(pRenderer, pBackground, NULL, &rectPositionImage);
		SDL_RenderCopy(pRenderer, pTextureText, NULL, &textPosition);
		Kr_FPS_Show(pFPS);
		SDL_RenderPresent(pRenderer); // Lorsque toutes les surfaces ont été placé on affiche le renderer (l'écran quoi...)
		UTIL_FreeTexture(&pTextureText); // Comme on recréé la texture en permanence dans la boucle il faut la free également dans la boucle
	
	}


	/* ========================================================================= */
	/*                            LIBERATION MEMOIRE                             */
	/* ========================================================================= */

	//UTIL_FreeTexture(&pBackground);		// Libération mémoire de la texture 
	UTIL_FreeTexture(&pTextureText);	// Libération mémoire de la texture du Texte ttf
	SDL_DestroyRenderer(pRenderer);	// Libération mémoire du renderer
	SDL_DestroyWindow(pWindow);			// Libération mémoire de la fenetre
	Kr_Text_CloseFont(&pFont);			// Libération mémoire de la police
	Kr_Text_CloseFont(&pFontFPS);			// Libération mémoire de la police
	Kr_Level_Free(pCurrentLevel);
	Kr_Sound_Free(&pSon);			// Libération mémoire du son !!
	Entity_Free(pZelda);				// Libération mémoire du zelda

	Kr_Map_Free(pMap);
	Mix_CloseAudio();	// On quitte SDL_MIXER
	TTF_Quit();			// On quitte SDL_TTF
	SDL_Quit();			// On quitte SDL
	Kr_Log_Quit();		// On ferme les logs
	return EXIT_SUCCESS;
}




/*!
*  \fn     void GetVector(Kr_Input inEvent, Sint32 *vx, Sint32 *vy)
*  \brief  Function to get the vector of the player
*
*  \todo   Update this function when the Player structure will be define
*
*  \param  inEvent Structure which handle the input
*  \param  vx      a pointer to the vector on X
*  \param  vy      a pointer to the vector on Y
*  \return none
*/
/*
void GetVector(Kr_Input inEvent, Sint32 *vx, Sint32 *vy)
{

	*vx = *vy = 0;
	if (inEvent.szKey[SDL_SCANCODE_UP])
		*vy = -MOVESPEED;
	if (inEvent.szKey[SDL_SCANCODE_DOWN])
		*vy = MOVESPEED;
	if (inEvent.szKey[SDL_SCANCODE_LEFT])
		*vx = -MOVESPEED;
	if (inEvent.szKey[SDL_SCANCODE_RIGHT])
		*vx = MOVESPEED;
}
*/

/*!
*  \fn     void UpdatePlayerVector(Kr_Input inEvent,Kr_Level *pLevel, SDL_Rect *pPlayer)
*  \brief  Function to get the vector of the player
*
*  \todo   Update this function when the Player structure will be define
*
*  \param  inEvent Structure which handle the input
*  \param  pLevel  a pointer to the Level
*  \param  pPlayer  a pointer to the player
*  \return none
*/
/*
void UpdatePlayerVector(Kr_Input inEvent, Kr_Level *pLevel, SDL_Rect *pPlayer)
{
	Sint32 vx, vy;
	GetVector(inEvent, &vx, &vy);
	Kr_Collision_Move(pLevel, pPlayer, vx, vy);
}
*/
/*

// Préparation d'une image que l'on souhaitera afficher via kr_util
SDL_Texture *pBackground = NULL;
SDL_Rect     rectPositionImage;

rectPositionImage.x = 0;
rectPositionImage.y = 0;
rectPositionImage.w = 20; //Il est nécessaire de fournir la taille de l'image avec .w et .h sinon rien n'apparaitra
rectPositionImage.h = 20;

pBackground = UTIL_LoadTexture(pRenderer, "Personnage.jpg", NULL, NULL);
if (pBackground == NULL)
{
	exit(EXIT_FAILURE);
}



*/
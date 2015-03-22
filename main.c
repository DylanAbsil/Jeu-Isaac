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

// Banque de son : http://www.wavsource.com/
// Utiliser Uint32 gérer la portabilité !!!
#define KR_FPS 30// Nombre de FPS

#define MOVESPEED 10

SDL_Renderer *gpRenderer = NULL;

void UpdatePlayerVector(Kr_Input inEvent, Kr_Level *pLevel, SDL_Rect *pPlayer);
void GetVector(Kr_Input inEvent, Sint32 *vx, Sint32 *vy);


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
	Uint32   iPreviousTime = 0, iCurrentTime = 0; // Variable pour la gestion des FPS

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
	gpRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gpRenderer == NULL)
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

	/* Préparation d'une image que l'on souhaitera afficher via kr_util*/
	SDL_Texture *pBackground = NULL;
	SDL_Rect     rectPositionImage;
	
	rectPositionImage.x = 0; 
	rectPositionImage.y = 0;
	rectPositionImage.w = 24; //Il est nécessaire de fournir la taille de l'image avec .w et .h sinon rien n'apparaitra
	rectPositionImage.h = 24;

	pBackground         = UTIL_LoadTexture("Personnage.jpg", NULL, NULL);
	if (pBackground == NULL)
	{
		exit(EXIT_FAILURE);
	} 


	/* Préparation d'une Texture contenant un message via util.c*/
	SDL_Rect     textPosition;
	SDL_Color    couleur = { 123, 255, 0 }; 
	SDL_Texture *pTextureText = NULL;
	TTF_Font    *pFont = NULL;
	char         szCompteur[100] = " ";// Tableau contenant la valeur du compteur
	Uint32       iCount = 0; //Valeur actuel du compteur

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
	Kr_Level *pLevel1 = NULL;
	Kr_Level *pLevel2 = NULL;
	Kr_Level *pCurrentLevel = NULL;
	pLevel1 = Kr_Level_Init("level1"); // Ne pas préciser l'extension
	if (!Kr_Level_Load(pLevel1, gpRenderer))
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't Load a level\n");
		exit(EXIT_FAILURE);
	}

	pLevel2 = Kr_Level_Init("level2"); // Ne pas préciser l'extension
	if (!Kr_Level_Load(pLevel2, gpRenderer))
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't Load a level\n");
		exit(EXIT_FAILURE); 
	}
	pCurrentLevel = pLevel1;
	/* ========================================================================= */
	/*                                 EVENEMENT                                 */
	/* ========================================================================= */
	// Cette boucle va permettre mettre à jour la structure qui gère les entrées clavier/souris
	// On aura plus qu'à regarder si notre touche a été appuyé et si oui agir en conséquence
	while (!inEvent.szKey[SDL_SCANCODE_ESCAPE] && !inEvent.bQuit)
	{
		
		UpdateEvents(&inEvent);

		/* Mise à jour des coordonnées du personnage*/
		UpdatePlayerVector(inEvent, pCurrentLevel, &rectPositionImage);

		if (inEvent.szMouseButtons[0])
		{
			rectPositionImage.x = inEvent.iMouseX;
			rectPositionImage.y = inEvent.iMouseY;
			Kr_Log_Print(KR_LOG_ERROR, "CLIQUE GAUCHE : %d %d \n", inEvent.iMouseX, inEvent.iMouseY);
			inEvent.szMouseButtons[0] = 0; // Un seul clique, si je ne met pas ça, le son sera joué en boucle. La l'utilisateur va devoir relever son doigt
		}
		if (inEvent.szMouseButtons[1])
		{
			Kr_Log_Print(KR_LOG_INFO, "CLIQUE MOLETTE\n");
			inEvent.szMouseButtons[1] = 0; // Un seul clique
		}
		if (inEvent.szMouseButtons[2])
		{
			//Kr_Log_Print(KR_LOG_INFO, "CLIQUE DROIT\n");
			//Kr_Log_Print(KR_LOG_INFO, "Tiles %d\n", );
			Kr_Level_GetBlock(pCurrentLevel, inEvent.iMouseX, inEvent.iMouseY);
			inEvent.szMouseButtons[2] = 0; // Un seul clique
		}
		if (inEvent.szKey[SDL_SCANCODE_P])
		{
			inEvent.szKey[SDL_SCANCODE_P] = 0; // Un seul clique, comme pour la souris
			if(Mix_PausedMusic()) // Si le son est en pause
			{ 
				Mix_ResumeMusic(); // On le relance
			}
			else
			{
				Mix_PauseMusic();//Sinon je le met en pause
			}
		}
		if (inEvent.szKey[SDL_SCANCODE_KP_1])
		{
			pCurrentLevel = pLevel1;
			inEvent.szKey[SDL_SCANCODE_KP_1] = 0;
		}
		if (inEvent.szKey[SDL_SCANCODE_KP_2])
		{
			pCurrentLevel = pLevel2;
			inEvent.szKey[SDL_SCANCODE_KP_2] = 0;
		}
		
		/* ========================================================================= */
		/*                                    FPS                                    */
		/* ========================================================================= */

		iCurrentTime = SDL_GetTicks(); 
		if (iCurrentTime - iPreviousTime > (1000 / KR_FPS))
		{
			iPreviousTime = iCurrentTime; 
		}
		else
		{
			//Kr_Log_Print(KR_LOG_INFO, "Attente de %d \n", (1000 / KR_FPS) - (iCurrentTime - iPreviousTime));
			SDL_Delay((1000 / KR_FPS) - (iCurrentTime - iPreviousTime));
		}
		
		/* ========================================================================= */
		/*                                  DIVERS                                   */
		/* ========================================================================= */
		pTextureText = Kr_Text_FontCreateTexture(gpRenderer, pFont, szCompteur, couleur, TRUE, &textPosition); // Création d'une texture contenant le texte d'une certaine couleur avec le mode Blended  
		//iCount += (1000 / KR_FPS); // Cette variable permet juste d'afficher le temps depuis lequel l'exe est actif, plus tard on le mettra en forme pour afficher le temps depuis lequel l'utilisateur est dans le jeu
		iCount += 1;
		//sprintf(szCompteur, "Time : %d", iCount); // Mise à jour du compteur
		sprintf(szCompteur, "Cursor : X : %d Y : %d", inEvent.iMouseX, inEvent.iMouseY);//)pMonLevel->rScrollWindow->x, pMonLevel->rScrollWindow->y // Affichage coordonnée de la map

		//Kr_Log_Print(KR_LOG_INFO, "X         : %d |Y          : %d \n", pMonLevel->rLimitation->x, pMonLevel->rLimitation->y);
		/* ========================================================================= */
		/*                                  RENDER                                   */
		/* ========================================================================= */
		// Ici on gère l'affichage des surfaces
		SDL_RenderClear(gpRenderer); // Dans un premier temps on Clear le renderer
		// Remarque, en inversant les deux SDL_RenderCopy, on peut choisir qu'elle image sera en arrière-plan de l'autre
		Kr_Level_Draw(gpRenderer, pCurrentLevel);
		SDL_RenderCopy(gpRenderer, pBackground, NULL, &rectPositionImage); // En arrière plan
		SDL_RenderCopy(gpRenderer, pTextureText, NULL, &textPosition); // En avant plan de la texture précédente
		SDL_RenderPresent(gpRenderer); // Lorsque toutes les surfaces ont été placé on affiche le renderer (l'écran quoi...)
		UTIL_FreeTexture(&pTextureText); // Comme on recréé la texture en permanence dans la boucle il faut la free également dans la boucle
	}

	/* ========================================================================= */
	/*                            LIBERATION MEMOIRE                             */
	/* ========================================================================= */

	UTIL_FreeTexture(&pBackground);		// Libération mémoire de la texture 
	UTIL_FreeTexture(&pTextureText);	// Libération mémoire de la texture du Texte ttf
	SDL_DestroyRenderer(gpRenderer);	// Libération mémoire du renderer
	SDL_DestroyWindow(pWindow);			// Libération mémoire de la fenetre
	Kr_Text_CloseFont(&pFont);			// Libération mémoire de la police
	Kr_Level_Free(pLevel1);
	Kr_Level_Free(pLevel2);
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
void UpdatePlayerVector(Kr_Input inEvent, Kr_Level *pLevel, SDL_Rect *pPlayer)
{
	Sint32 vx, vy;
	GetVector(inEvent, &vx, &vy);
	Kr_Collision_Move(pLevel, pPlayer, vx, vy);
}


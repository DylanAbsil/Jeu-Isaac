/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 18/02/2015 | TTF encapsuler dans kr_text                  */
/*               |            | Renderer en variable global ? Si oui o� ?    */
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
// Utiliser Uint32 g�rer la portabilit� !!!
#define KR_FPS 30// Nombre de FPS

#define MOVESPEED 10



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

	Kr_Input inEvent; // Structure pour la gestion des �v�nements
	Uint32   iPreviousTime = 0, iCurrentTime = 0; // Variable pour la gestion des FPS

	/* Cr�ation de la fen�tre */
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
	/* Cr�ation du renderer */
	SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	if (pRenderer == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't create the Renderer", SDL_GetError());
		SDL_Quit();
		return FALSE;
	}
	/* Initialisation de la structure pour g�rer les �v�nements*/
	InitEvents(&inEvent);

	/* ========================================================================= */
	/*                                  PRECACHE                                 */
	/* ========================================================================= */

	/* Pr�paration d'une image que l'on souhaitera afficher via kr_util*/
	SDL_Texture *pBackground = NULL;
	SDL_Rect     rectPositionImage;
	
	rectPositionImage.x = 0; 
	rectPositionImage.y = 0;
	rectPositionImage.w = 20; //Il est n�cessaire de fournir la taille de l'image avec .w et .h sinon rien n'apparaitra
	rectPositionImage.h = 20;

	pBackground         = UTIL_LoadTexture(pRenderer,"Personnage.jpg", NULL, NULL);
	if (pBackground == NULL)
	{
		exit(EXIT_FAILURE);
	} 


	/* Pr�paration d'une Texture contenant un message via util.c*/
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
	Boolean   bChangeLevel = TRUE;
	Kr_Level *pCurrentLevel = NULL;
	char      szNewLevel[20] = "level1";

	/* ========================================================================= */
	/*                                 EVENEMENT                                 */
	/* ========================================================================= */
	while (!inEvent.szKey[SDL_SCANCODE_ESCAPE] && !inEvent.bQuit)
	{
		/* Faire au d�but sinon crash*/
		if (bChangeLevel == TRUE)
		{
			bChangeLevel = FALSE;
			pCurrentLevel = Kr_Level_Change(pCurrentLevel, szNewLevel, pRenderer);
		}


		UpdateEvents(&inEvent);
		/* Mise � jour des coordonn�es du personnage*/
		UpdatePlayerVector(inEvent, pCurrentLevel, &rectPositionImage);

		if (inEvent.szMouseButtons[0])
		{
			rectPositionImage.x = inEvent.iMouseX;
			rectPositionImage.y = inEvent.iMouseY;
			Kr_Log_Print(KR_LOG_INFO, "CLIQUE GAUCHE : %d %d \n", inEvent.iMouseX, inEvent.iMouseY);
			inEvent.szMouseButtons[0] = 0; // Un seul clique, si je ne met pas �a, le son sera jou� en boucle. La l'utilisateur va devoir relever son doigt
		}
		if (inEvent.szMouseButtons[2])
		{
			//Kr_Log_Print(KR_LOG_INFO, "CLIQUE DROIT\n");
			if (Kr_Level_Event(pCurrentLevel, &rectPositionImage) == 1)
			{
				bChangeLevel = TRUE;
				if (strcmp(szNewLevel, "level1") == 0) strcpy(szNewLevel, "level2");
				else strcpy(szNewLevel, "level1");
			}
			Kr_Log_Print(KR_LOG_INFO, "\n");
			//Kr_Level_GetTile(pCurrentLevel, inEvent.iMouseX, inEvent.iMouseY);
			inEvent.szMouseButtons[2] = 0; // Un seul clique
		}
		if (inEvent.szKey[SDL_SCANCODE_KP_1])
		{
			bChangeLevel = TRUE;
			strcpy(szNewLevel, "level3");
			inEvent.szKey[SDL_SCANCODE_KP_1] = 0;
		}
		if (inEvent.szKey[SDL_SCANCODE_KP_2])
		{
			bChangeLevel = TRUE; 
			strcpy(szNewLevel, "level4");
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
		
		pTextureText = Kr_Text_FontCreateTexture(pRenderer, pFont, szCompteur, couleur, TRUE, &textPosition); // Cr�ation d'une texture contenant le texte d'une certaine couleur avec le mode Blended  
		iCount += 1;
		sprintf(szCompteur, "Cursor : X : %d Y : %d   %s", inEvent.iMouseX, inEvent.iMouseY,pCurrentLevel->szLevelName);//)pMonLevel->rScrollWindow->x, pMonLevel->rScrollWindow->y // Affichage coordonn�e de la map

		//Kr_Log_Print(KR_LOG_INFO, "X         : %d |Y          : %d \n", pMonLevel->rLimitation->x, pMonLevel->rLimitation->y);
		/* ========================================================================= */
		/*                                  RENDER                                   */
		/* ========================================================================= */
		// Ici on g�re l'affichage des surfaces
		SDL_RenderClear(pRenderer); // Dans un premier temps on Clear le renderer
		// Remarque, en inversant les deux SDL_RenderCopy, on peut choisir qu'elle image sera en arri�re-plan de l'autre
		Kr_Level_Draw(pRenderer, pCurrentLevel);
		SDL_RenderCopy(pRenderer, pBackground, NULL, &rectPositionImage); // En arri�re plan
		SDL_RenderCopy(pRenderer, pTextureText, NULL, &textPosition); // En avant plan de la texture pr�c�dente
		SDL_RenderPresent(pRenderer); // Lorsque toutes les surfaces ont �t� plac� on affiche le renderer (l'�cran quoi...)
		UTIL_FreeTexture(&pTextureText); // Comme on recr�� la texture en permanence dans la boucle il faut la free �galement dans la boucle
	}

	/* ========================================================================= */
	/*                            LIBERATION MEMOIRE                             */
	/* ========================================================================= */

	UTIL_FreeTexture(&pBackground);		// Lib�ration m�moire de la texture 
	UTIL_FreeTexture(&pTextureText);	// Lib�ration m�moire de la texture du Texte ttf
	SDL_DestroyRenderer(pRenderer);	// Lib�ration m�moire du renderer
	SDL_DestroyWindow(pWindow);			// Lib�ration m�moire de la fenetre
	Kr_Text_CloseFont(&pFont);			// Lib�ration m�moire de la police
	Kr_Level_Free(pCurrentLevel);
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

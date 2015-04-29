/* ========================================================================= */
/*!
* \file    kr_fps.h
* \brief   Contains the structure to handle the fps
* \author  Olivier Herrou
* \version 1.0
* \date    4 Avril 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 04/04/2015 | Creation.                                    */
/*               |            |                                              */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#include "kr_fps.h"


/*!
*  \fn     Kr_Fps *Kr_Fps_Init(SDL_Renderer *pRenderer, TTF_Font *pFont, SDL_Rect *pRectPosition, SDL_Color color, Boolean bMustShow)
*  \brief  Function to initialize a Kr_Fps structure
*
*  \param  pRenderer	 a pointer to the renderer
*  \param  pFont		 a pointer to a font
*  \param  pRectPosition a pointer to the position of the text
*  \param  color		 the color of the text
*  \param  bMustShow	 True if we must show the fps
*  \return the initialize structure if everything is ok, NULL otherwise
*/
Kr_Fps *Kr_Fps_Init(SDL_Renderer *pRenderer, TTF_Font *pFont, SDL_Rect *pRectPosition, SDL_Color color, Boolean bMustShow)
{
	Kr_Fps *pFPS = NULL;
	pFPS = (Kr_Fps *)UTIL_Malloc(sizeof(Kr_Fps));
	if (pFPS == NULL) return NULL;

	pFPS->color		= color;
	pFPS->pTexture	= NULL;
	pFPS->pRenderer = pRenderer;
	pFPS->pFont		= pFont;
	pFPS->pRectPosition = pRectPosition;
	pFPS->bMustShow = bMustShow;
	return pFPS;
}



/*!
*  \fn     void Kr_Fps_Wait(Kr_Fps *pFPS, Uint32 *iCurrentTime, Uint32 *iPreviousTime, Uint32 iFPS)
*  \brief  Function to handle the FPS 
*
*  \param  pFPS			 a pointer to the Kr_Fps Structure
*  \param  iCurrentTime	 the current time of the exe
*  \param  iPreviousTime the previous time of the exe
*  \param  iFPS			 the FPS we want
*  \return none
*/
void Kr_Fps_Wait(Kr_Fps *pFPS, Uint32 *iCurrentTime, Uint32 *iPreviousTime, Uint32 iFPS)
{
	char szFPS[30] = "";

	*iCurrentTime = SDL_GetTicks();
	if (*iCurrentTime - *iPreviousTime > (1000 / iFPS))
	{
		sprintf(szFPS, "FPS : %3.0lf", (1.0 / (*iCurrentTime - *iPreviousTime) * 1000));
		*iPreviousTime = *iCurrentTime;
	}
	else
	{
		//Kr_Log_Print(KR_LOG_INFO, "Attente de %d ms \n", (1000 / iFPS) - (*iCurrentTime - *iPreviousTime));
		sprintf(szFPS, "FPS : %3.0lf", iFPS);
		SDL_Delay((1000 / iFPS) - (*iCurrentTime - *iPreviousTime));
	}
	if (pFPS->bMustShow)
	{
		UTIL_FreeTexture(&pFPS->pTexture);
		pFPS->pTexture = Kr_Text_FontCreateTexture(pFPS->pRenderer, pFPS->pFont, szFPS, pFPS->color, TRUE, pFPS->pRectPosition); 
		return;
	}	
}



/*!
*  \fn     void Kr_FPS_Show(Kr_Fps *pFPS)
*  \brief  Function to Print the FPS
*
*  \param  pFPS			 a pointer to the Kr_Fps Structure
*  \return none
*/
void Kr_FPS_Show(Kr_Fps *pFPS)
{
	if (pFPS->bMustShow)
	{
		SDL_RenderCopy(pFPS->pRenderer, pFPS->pTexture, NULL, pFPS->pRectPosition); // affichage des FPS
	}
}


/*!
*  \fn     Uint32 PeriodicEvent()
*  \brief  Function to handle all the periodic events of the game
*
*  \return  0 : Aucun évenement
			1 : Passage d'oiseau
*/
Uint32 PeriodicEvent()
{
	static Uint32 iRepeat = 0; // On compte le nombre de fois que cette fonction est appelée
	Uint32 iRetour = 0;
	Uint32 iTimeOiseau = 300;

	iRepeat++;
	if ((iRepeat % iTimeOiseau) == 0)
	{
		Kr_Log_Print(KR_LOG_INFO, "Repeat %d\n", iRepeat);
		return iRetour = 1;
	}
	return iRetour;
}




/* Utilisation
Avant la boucle principale :

// Préparation de la gestion des FPS
SDL_Texture *pTextureFPS = NULL;
TTF_Font	*pFontFPS = NULL;
Kr_Fps		*pFPS = NULL;
SDL_Color    colorFPS = { 0, 10, 220 };
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

Dans la boucle de gestion des événements

if (inEvent.szKey[SDL_SCANCODE_F]) // Gestion de l'affichage des FPS
{
if (pFPS->bMustShow) pFPS->bMustShow = FALSE;
else pFPS->bMustShow = TRUE;
inEvent.szKey[SDL_SCANCODE_F] = 0;
}


//Attente
Kr_Fps_Wait(pFPS, &iCurrentTime, &iPreviousTime, KR_FPS);

//L'affichage se fait en dernier, avec les HUDS
Kr_FPS_Show(pFPS);



*/
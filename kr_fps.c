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
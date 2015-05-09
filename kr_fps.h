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

#ifndef __KR_FPS_H__
#define __KR_FPS_H__

	#include "kr_common.h"
	#include "kr_log.h"
	#include "kr_text.h"
/*!
* \struct Kr_FPS
* \brief  Structure to handle the FPS.
*/
typedef struct
{
	SDL_Texture		*pTexture;		/*!< Texture of the FPS*/
	SDL_Renderer	*pRenderer;		/*!< The Renderer where the FPS will be print*/
	TTF_Font		*pFont;			/*!< The Font of the text */
	SDL_Rect		*pRectPosition;	/*!< Position of where the FPS will be print*/
	SDL_Color        color;         /*!< The color of the text*/
	Boolean          bMustShow;     /*!< Must we print the fps ?*/
} Kr_Fps;


Kr_Fps *Kr_Fps_Init(SDL_Renderer *pRenderer, TTF_Font *pFont, SDL_Rect *pRectPosition, SDL_Color color, Boolean bMustShow);
void    Kr_Fps_Wait(Kr_Fps *pFPS, Uint32 *iCurrentTime, Uint32 *iPreviousTime, Uint32 iFPS);
void    Kr_Fps_Free(Kr_Fps *pFPS);
void    Kr_FPS_Show(Kr_Fps *pFPS);

Uint32  PeriodicEvent(void);
#endif /* __KR_FPS_H__ */

/* ========================================================================= */


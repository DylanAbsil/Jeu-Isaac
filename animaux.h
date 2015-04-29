/* ========================================================================= */
/*!
* \file    animaux.h
* \brief   Contains the structure to handle the animal IA
* \author  Olivier Herrou
* \version 1.0
* \date    29 Avril 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 29/04/2015 | Creation.                                    */
/*               |            |                                              */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */


#ifndef __KR_ANIMAUX_H__
#define __KR_ANIMAUX_H__

	#include "kr_common.h"
	#include "kr_log.h"
	#include "entite.h"


Entity *ChargementOiseau(SDL_Renderer *pRenderer);
void CalculPassageOiseau(Entity *pEntity, Boolean bLaunch, Sint32 *movex, Sint32 *movey);
Boolean PassageOiseau(Entity *pEntity, Boolean bPassage, Sint32 movex, Sint32 movey, SDL_Renderer *pRenderer);
void FreeOiseau(Entity *pEntity);



#endif /* __KR_FPS_H__ */

/* ========================================================================= */


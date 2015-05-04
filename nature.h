/* ========================================================================= */
/*!
* \file    nature.h
* \brief   Contains the structure to handle the nature
* \author  Olivier Herrou
* \version 1.0
* \date    29 Avril 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 29/04/2015 | Creation, gestion des oiseaux                */
/* Herrou        | 01/05/2015 | Animation des buissons                       */
/* Herrou        | 02/05/2015 | Calcul du nombre de papillon                 */
/* Herrou        | 04/05/2015 | Renommer fichier en nature.h .c              */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */


#ifndef __KR_ANIMAUX_H__
#define __KR_ANIMAUX_H__

	#include "kr_common.h"
	#include "kr_log.h"
	#include "entite.h"
	#include "kr_level.h"
	#include "kr_sound.h"
	#include "level_state.h"


Entity *ChargementOiseau(SDL_Renderer *pRenderer, Uint32 iOiseau);
void CalculPassageOiseau(Entity *pEntity, Boolean bLaunch, Sint32 *movex, Sint32 *movey);
Uint32 CalculTypeOiseau(Boolean bCalculer, Kr_Level *pLevel);
Boolean PassageOiseau(Entity *pEntity, Boolean bPassage, Sint32 movex, Sint32 movey, SDL_Renderer *pRenderer, Kr_Sound *pSndOiseau);

Entity *ChargementBuisson(SDL_Renderer *pRenderer,Uint32 iBuisson);
Boolean AnimationBuisson(Entity *pEntity, Boolean bPassage, Uint32 x, Uint32 y, SDL_Renderer *pRenderer, Kr_Sound *pSndBuisson);
Uint32	DetectPassageBuisson(Kr_Level *pLevel, Entity *pPlayer, Uint32 *px, Uint32 *py);

Entity *ChargementPapillon(SDL_Renderer *pRenderer);
Boolean CalculApparitionPapillon(Boolean bCalculer, Kr_Level *pLevel, Uint32 *iNumber);

Entity *ChargementPigeonVolant(SDL_Renderer *pRenderer);
Boolean PigeonVol(Entity *pPigeonVol, Boolean bActiver, SDL_Renderer *pRenderer, Kr_Level *pLevel, Kr_Sound *pSndPigeon ,Uint32 xDebut, Uint32 yDebut,Uint32 *xFin, Uint32 *yFin);

#endif /* __KR_FPS_H__ */

/* ========================================================================= */


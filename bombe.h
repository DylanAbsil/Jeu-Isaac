/* ========================================================================= */
/*!
* \file    bombe.h
* \brief   Contains the structure to handle the weapon bomb.
* \author  Olivier Herrou
* \version 1.0
* \date    17 Mars 2015
*/
/* ================================================================================================================ */
/* Developers    | Date       | Comments																			*/
/* --------------+------------+------------------------------------------------------------------------------------ */
/* Herrou        | 17/05/2015 | Création																			*/
/* ===============================================================================================================  */

#ifndef __BOMBE_H__
#define __BOMBE_H__

#include "kr_common.h"
#include "kr_util.h"
#include "kr_log.h"
#include "entite.h"
#include "kr_sound.h"
#include "kr_fps.h"

#define BOMBE_DMG 600
/*!
* \struct Bombe
* \brief  Structure to handle the bomb.
*/
typedef struct
{
	Uint32		 iNumber;			/*!< The number of bomb the player has */
	Uint32		 iCooldown;			/*!< The cooldown between each ticks of the bomb */
	Entity		*pEntExplosion;		/*!< The Entity to handle the animation of the explosion */
	Entity		*pEntBomb;			/*!< The Entity to handle the animation of the bomb */
	Kr_Sound    *pSndBombSet;		/*!< The sound of the bomb when it's set */
	Kr_Sound	*pSndBombExplosion; /*!< The sound of the bomb when it's exploding */
	Boolean      bBombSet;			/*!< TRUE if the bomb is set */
	Boolean		 bBombExplosion;    /*!< TRUE if the explosion is set*/
	Uint32		 x;					/*!< Coordinate on x of the bomb on the ground*/
	Uint32		 y;					/*!< Coordinate on y of the bomb on the ground*/
}Bombe;

Bombe	*Bombe_Init(SDL_Renderer *pRenderer, Uint32 iNumber, Uint32 iCooldown, char *szSprExplosion, char *szSprBomb, char *szSndSet, char *szSndExplode);
void	 Bombe_Free(Bombe *pBombe);
Boolean  Bombe_Set(Bombe *pBombe, Boolean bStart, Uint32 x, Uint32 y, Boolean *bCheck);
Boolean  Bombe_Explosion(Bombe *pBombe, Boolean bStart, SDL_Renderer *pRenderer);


#endif /* __BOMBE_H__ */
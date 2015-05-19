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

/*!
* \struct Bombe
* \brief  Structure to handle the bomb.
*/
typedef struct
{
	Uint32		 iNumber;			/*!< The number of bomb the player has */
	Uint32		 iCooldown;			/*!< The cooldown between 2 bombs */
	Entity		*pEntExplosion;		/*!< The Entity to handle the animation of the explosion*/
	Entity		*pEntBomb;			/*!< The Entity to handle the animation of the bomb*/
	Kr_Sound    *pSndBombSet;		/*!< The sound of the bomb when it's set*/
	Kr_Sound	*pSndBombExplosion; /*!< The sound of the bomb when it's exploding*/
}Bombe;

Bombe	*Bombe_Init(SDL_Renderer *pRenderer, Uint32 iNumber, Uint32 iCooldown, char *szSprExplosion, char *szSprBomb, char *szSndSet, char *szSndExplode);
void	 Bombe_Free(Bombe *pBombe);
void	 Bombe_Update(Bombe *pBombe, Uint32 iNumber);
Boolean  Bombe_Set(Bombe *pBombe, Uint32 x, Uint32 y);
Boolean  Bombe_Explosion();


#endif /* __BOMBE_H__ */
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
#include "kr_sprite.h"
#include "kr_sound.h"

/*!
* \struct Bombe
* \brief  Structure to handle the bomb.
*/
typedef struct
{
	Uint32		 iNumber;			/*!< The number of bomb the player has */
	Uint32		 iCooldown;			/*!< The cooldown between 2 bombs */
	Uint32		 iDelaySet;			/*!< The delay between the time the bomb was set and the explosion*/
	Uint32		 iDelayAnimation;	/*!< The number of seconds during which the animation of the explosion is shown*/
	Kr_Sprite	*pSprExplosion;		/*!< The sprite of the explosion*/
	Kr_Sprite	*pSprBomb;			/*!< The sprite of the bomb*/
	Kr_Sound    *pSndBombSet;		/*!< The sound of the bomb when it's set*/
	Kr_Sound	*pSndBombExplosion; /*!< The sound of the bomb when it's exploding*/
}Bombe;

Bombe	*Bombe_Init(Uint32 iNumber, Uint32 iCooldown, Uint32 iDelaySet, Uint32 iDelayAnimation, char *szSprExplosion, char *szSprBomb, char *szSndSet, char *szSndExplode);
void	 Bombe_Free(Bombe *pBombe);
void	 Bombe_Update(Bombe *pBombe, Uint32 iNumber);
Boolean  Bombe_Set();
Boolean  Bombe_Explosion();


#endif /* __BOMBE_H__ */
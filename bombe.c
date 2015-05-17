/* ========================================================================= */
/*!
* \file    bombe.c
* \brief   Contains the functions to handle the weapon bomb.
* \author  Olivier Herrou
* \version 1.0
* \date    17 Mars 2015
*/
/* ================================================================================================================ */
/* Developers    | Date       | Comments																			*/
/* --------------+------------+------------------------------------------------------------------------------------ */
/* Herrou        | 17/05/2015 | Création																			*/
/* ===============================================================================================================  */

#include "bombe.h"



/*!
*  \fn     Bombe *Bombe_Init(Uint32 iNumber, Uint32 iCooldown, Uint32 iDelaySet, Uint32 iDelayAnimation, char *szSprExplosion, char *szSprBomb, char *szSndSet, char *szSndExplode)
*  \brief  Function to initialize a Bombe structure
*
*  \param  iNumber			the number of bomb the player has
*  \param  iCooldown		the Cooldown of the bomb
*  \param  iDelaySet		the delay before the bomb start to explode
*  \param  iDelayAnimation	the delay before the explosion of the bomb stop
*  \param  szSprExplosion	the path to the sprite of the explosion of the bomb
*  \param  szSprBomb		the path to the sprite of the bomb
*  \param  szSndSet			the path to the sound of the bomb when it's set
*  \param  szSndExplode		the path to the sound of the bomb when it's exploding
*  \return the initialize structure
*/
Bombe *Bombe_Init(Uint32 iNumber, Uint32 iCooldown, Uint32 iDelaySet, Uint32 iDelayAnimation, char *szSprExplosion, char *szSprBomb, char *szSndSet, char *szSndExplode)
{
	Uint32 iNameLen = 0;
	Bombe *pBombe = NULL;
	Kr_Sprite *pSprite = NULL;

	pBombe = (Bombe *)UTIL_Malloc(sizeof(Bombe));

	pBombe->pSndBombSet = Kr_Sound_Alloc(szSndSet);
	pBombe->pSndBombSet = Kr_Sound_Alloc(szSndExplode);


	pBombe->iNumber = iNumber;
	pBombe->iCooldown = iCooldown;
	pBombe->iDelayAnimation = iDelayAnimation;
	pBombe->iDelaySet = iDelaySet;

	return pBombe;
}

/*!
*  \fn     void Bombe_Free(Bombe *pBombe)
*  \brief  Function to free a Bombe structure
*
*  \param  pBombe the Bombe structure
*  \return none
*/
void Bombe_Free(Bombe *pBombe)
{
	Kr_Sound_Free(&pBombe->pSndBombSet);
	Kr_Sound_Free(&pBombe->pSndBombExplosion);
	Kr_Sprite_Free(pBombe->pSprBomb);
	Kr_Sprite_Free(pBombe->pSprExplosion);
	UTIL_Free(pBombe);
}

/*!
*  \fn     void Bombe_Update(Bombe *pBombe, Uint32 iNumber)
*  \brief  Function to update the number of bomb
*
*  \param  pBombe  the Bombe structure
*  \param  iNumber the number of bomb
*  \return none
*/
void Bombe_Update(Bombe *pBombe, Uint32 iNumber)
{
	pBombe->iNumber = iNumber;
}

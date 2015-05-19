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
*  \fn     Bombe *Bombe_Init(SDL_Renderer *pRenderer, Uint32 iNumber, Uint32 iCooldown, char *szSprExplosion, char *szSprBomb, char *szSndSet, char *szSndExplode)
*  \brief  Function to initialize a Bombe structure
*
*  \param  pRenderer		a pointer to the renderer
*  \param  iNumber			the number of bomb the player has
*  \param  iCooldown		the Cooldown of the bomb
*  \param  szSprExplosion	the path to the sprite of the explosion of the bomb
*  \param  szSprBomb		the path to the sprite of the bomb
*  \param  szSndSet			the path to the sound of the bomb when it's set
*  \param  szSndExplode		the path to the sound of the bomb when it's exploding
*  \return the initialize structure
*/
Bombe *Bombe_Init(SDL_Renderer *pRenderer, Uint32 iNumber, Uint32 iCooldown, char *szSprExplosion, char *szSprBomb, char *szSndSet, char *szSndExplode)
{
	Uint32	   iNameLen = 0;
	Bombe	  *pBombe = NULL;
	Kr_Sprite *pSprite = NULL;
	pBombe = (Bombe *)UTIL_Malloc(sizeof(Bombe));
	if (!pBombe) return NULL;

	SDL_Rect *pRectBomb = (SDL_Rect*)UTIL_Malloc(sizeof(SDL_Rect));
	pRectBomb->x = 0;
	pRectBomb->y = 0;
	pRectBomb->w = 20;
	pRectBomb->h = 28;


	/* Sprite */
	pSprite = Kr_Sprite_Init(szSprBomb);
	if (Kr_Sprite_Load(pSprite, unknown, 32, 128, 4, pRectBomb, pRenderer) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite SprBomb\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	/* Entité */
	pBombe->pEntBomb = Entity_Init("Bomb");
	if (Entity_Load(pBombe->pEntBomb, 100, 0, 0, noclip, TRUE, pSprite) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the entity Bomb!\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	pSprite = NULL;

	/* Sprite */
	pSprite = Kr_Sprite_Init(szSprExplosion);
	if (Kr_Sprite_Load(pSprite, unknown, 32, 128, 4, pRectBomb, pRenderer) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite SprExplosion\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	/* Entité */
	pBombe->pEntExplosion = Entity_Init("Bomb Explosion");
	if (Entity_Load(pBombe->pEntExplosion, 100, 0, 0, noclip, TRUE, pSprite) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the entity Bomb Explosion!\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	pBombe->pSndBombSet = Kr_Sound_Alloc(szSndSet);
	if (!pBombe->pSndBombSet)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't load the sound %s", szSndSet);
	}
	pBombe->pSndBombExplosion = Kr_Sound_Alloc(szSndExplode);
	if (!pBombe->pSndBombExplosion)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't load the sound %s", szSndExplode);
	}
	pBombe->iNumber = iNumber;
	pBombe->iCooldown = iCooldown;

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
	Entity_Free(pBombe->pEntBomb);
	Entity_Free(pBombe->pEntExplosion);
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


/*!
*  \fn     Boolean  Bombe_Set(Bombe *pBombe, Boolean bStart, Uint32 x, Uint32 y)
*  \brief  Function to set a bomb
*
*  \param  pBombe   the Bombe 
*  \param  bStart   TRUE to start
*  \param  x		Coord X
*  \param  y		Coord Y
*  \return none
*/
Boolean Bombe_Set(Bombe *pBombe, Boolean bStart, Uint32 x, Uint32 y)
{
	static Uint32 iDelay = 0;
	if (bStart && iDelay == 0)
	{
		//Play Sound set
	}

	iDelay++;

	



	return TRUE;
}
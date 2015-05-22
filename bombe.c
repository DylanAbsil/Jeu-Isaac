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
	if (Kr_Sprite_Load(pSprite, unknown, 28, 120, 6, pRectBomb, pRenderer) == FALSE)
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
	SDL_Rect *pRectExplosion = (SDL_Rect*)UTIL_Malloc(sizeof(SDL_Rect));
	pRectExplosion->x = 0;
	pRectExplosion->y = 0;
	pRectExplosion->w = 32;
	pRectExplosion->h = 32;
	/* Sprite */
	pSprite = Kr_Sprite_Init(szSprExplosion);
	if (Kr_Sprite_Load(pSprite, unknown, 32, 288, 9, pRectExplosion, pRenderer) == FALSE)
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
	pBombe->bBombSet = FALSE;
	pBombe->bBombExplosion = FALSE;
	pBombe->x = 0;
	pBombe->y = 0;
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
*  \fn     Boolean  Bombe_Set(Bombe *pBombe, Boolean bStart, Uint32 x, Uint32 y, Boolean *bCheck)
*  \brief  Function to set a bomb
*
*  \param  pBombe   the Bombe 
*  \param  bStart   TRUE to start
*  \param  x		Coord X
*  \param  y		Coord Y
*  \param  bCheck   a pointer, TRUE when the bomb is exploding and we need to check the entity
*  \return TRUE if the bomb is set, FALSE when the bomb is not set (or animation ended)
*/
Boolean Bombe_Set(Bombe *pBombe, Boolean bStart, Uint32 x, Uint32 y, Boolean *bCheck)
{
	if (!bStart) return FALSE;
	if (pBombe->iNumber <= 0) return FALSE;
	if (bStart && pBombe->bBombSet == FALSE)
	{
		pBombe->pEntBomb->pSprEntity->iCurrentFrame = 0;
		pBombe->pEntBomb->pSprEntity->pRectPosition->x = x;
		pBombe->pEntBomb->pSprEntity->pRectPosition->y = y;
		pBombe->x = x;
		pBombe->y = y;
		pBombe->bBombSet = TRUE;
		Kr_Sound_Play(pBombe->pSndBombSet, KR_SOUND_WEAPON1_CANAL, 100, 5);
	}
	
	pBombe->pEntBomb->mouvement = 1;
	pBombe->pEntBomb->iTempoAnim += 1;
	if (pBombe->pEntBomb->iTempoAnim == pBombe->iCooldown)
	{
		pBombe->pEntBomb->pSprEntity->iCurrentFrame += 1; //	- Frame suivante
		if (pBombe->pEntBomb->pSprEntity->iCurrentFrame == pBombe->pEntBomb->pSprEntity->iNbFrames) //Si l'animation est arrivée au bout 
		{
			pBombe->pEntBomb->pSprEntity->iCurrentFrame = 0;
			pBombe->pEntBomb->iTempoAnim = 0;
			pBombe->bBombSet = FALSE;
			*bCheck = TRUE;
			return FALSE;
		}
		pBombe->pEntBomb->iTempoAnim = 0;
	}
	return TRUE;
}


/*!
*  \fn     Boolean Bombe_Explosion(Bombe *pBombe, Boolean bStart)
*  \brief  Function to start the explosion
*
*  \param  pBombe    the Bombe
*  \param  bStart    TRUE to start
*  \param  pRenderer a pointer to the renderer
*  \return TRUE if the explosion is still in progress, FALSE otherwise
*/
Boolean Bombe_Explosion(Bombe *pBombe, Boolean bStart, SDL_Renderer *pRenderer)
{
	Sint32 i = 0, j = 0;
	Uint32 iContact = 0, x = 0, y = 0;
	if (!bStart) return FALSE;
	if (pBombe->iNumber <= 0) return FALSE;
	if (bStart && pBombe->bBombExplosion == FALSE)
	{
		pBombe->pEntExplosion->pSprEntity->iCurrentFrame = 0;
		pBombe->bBombExplosion = TRUE;
		Kr_Sound_Play(pBombe->pSndBombExplosion, KR_SOUND_WEAPON1_CANAL, 100, 0);
	}

	pBombe->pEntExplosion->mouvement = 1;
	pBombe->pEntExplosion->iTempoAnim += 1;
	if (pBombe->pEntExplosion->iTempoAnim == 7)
	{
		pBombe->pEntExplosion->pSprEntity->iCurrentFrame += 1; //	- Frame suivante
		if (pBombe->pEntExplosion->pSprEntity->iCurrentFrame == pBombe->pEntExplosion->pSprEntity->iNbFrames) //Si l'animation est arrivée au bout 
		{
			pBombe->pEntExplosion->pSprEntity->iCurrentFrame = 0;
			pBombe->pEntExplosion->iTempoAnim = 0;
			pBombe->bBombExplosion = FALSE;
			pBombe->iNumber -= 1;
			return FALSE;
		}
		pBombe->pEntExplosion->iTempoAnim = 0;
	}

	/* Màj des coordonées de l'explosion */
	for (i = -1; i <= 1; i++)
	{
		for (j = -1; j <= 1; j++)
		{
			pBombe->pEntExplosion->pSprEntity->pRectPosition->y = pBombe->y + i * pBombe->pEntExplosion->pSprEntity->pRectPosition->h;
			pBombe->pEntExplosion->pSprEntity->pRectPosition->x = pBombe->x + j * pBombe->pEntExplosion->pSprEntity->pRectPosition->w;
			Entity_Draw(pRenderer, pBombe->pEntExplosion);
		}
	}
	/* // Affichage de l'explosion en étoile
	pBombe->pEntExplosion->pSprEntity->pRectPosition->y = pBombe->y;
	pBombe->pEntExplosion->pSprEntity->pRectPosition->x = pBombe->x - 2 * pBombe->pEntExplosion->pSprEntity->pRectPosition->w ;
	Entity_Draw(pRenderer, pBombe->pEntExplosion);
	pBombe->pEntExplosion->pSprEntity->pRectPosition->x = pBombe->x - 1 * pBombe->pEntExplosion->pSprEntity->pRectPosition->w;
	Entity_Draw(pRenderer, pBombe->pEntExplosion);
	pBombe->pEntExplosion->pSprEntity->pRectPosition->x = pBombe->x;
	Entity_Draw(pRenderer, pBombe->pEntExplosion);
	pBombe->pEntExplosion->pSprEntity->pRectPosition->x = pBombe->x + 1 * pBombe->pEntExplosion->pSprEntity->pRectPosition->w;
	Entity_Draw(pRenderer, pBombe->pEntExplosion);
	pBombe->pEntExplosion->pSprEntity->pRectPosition->x = pBombe->x + 2 * pBombe->pEntExplosion->pSprEntity->pRectPosition->w;
	Entity_Draw(pRenderer, pBombe->pEntExplosion);

	pBombe->pEntExplosion->pSprEntity->pRectPosition->x = pBombe->x;
	pBombe->pEntExplosion->pSprEntity->pRectPosition->y = pBombe->y - 2 * pBombe->pEntExplosion->pSprEntity->pRectPosition->h;
	Entity_Draw(pRenderer, pBombe->pEntExplosion);
	pBombe->pEntExplosion->pSprEntity->pRectPosition->y = pBombe->y - 1 * pBombe->pEntExplosion->pSprEntity->pRectPosition->h;
	Entity_Draw(pRenderer, pBombe->pEntExplosion);
	pBombe->pEntExplosion->pSprEntity->pRectPosition->y = pBombe->y;
	Entity_Draw(pRenderer, pBombe->pEntExplosion);
	pBombe->pEntExplosion->pSprEntity->pRectPosition->y = pBombe->y + 1 * pBombe->pEntExplosion->pSprEntity->pRectPosition->h;
	Entity_Draw(pRenderer, pBombe->pEntExplosion);
	pBombe->pEntExplosion->pSprEntity->pRectPosition->y = pBombe->y + 2 * pBombe->pEntExplosion->pSprEntity->pRectPosition->h;
	Entity_Draw(pRenderer, pBombe->pEntExplosion);

	pBombe->pEntExplosion->pSprEntity->pRectPosition->x = pBombe->x + 1 * pBombe->pEntExplosion->pSprEntity->pRectPosition->w;;
	pBombe->pEntExplosion->pSprEntity->pRectPosition->y = pBombe->y + 1 * pBombe->pEntExplosion->pSprEntity->pRectPosition->h;
	Entity_Draw(pRenderer, pBombe->pEntExplosion);

	pBombe->pEntExplosion->pSprEntity->pRectPosition->y = pBombe->y - 1 * pBombe->pEntExplosion->pSprEntity->pRectPosition->h;
	Entity_Draw(pRenderer, pBombe->pEntExplosion);

	pBombe->pEntExplosion->pSprEntity->pRectPosition->x = pBombe->x - 1 * pBombe->pEntExplosion->pSprEntity->pRectPosition->w;;
	Entity_Draw(pRenderer, pBombe->pEntExplosion);

	pBombe->pEntExplosion->pSprEntity->pRectPosition->y = pBombe->y + 1 * pBombe->pEntExplosion->pSprEntity->pRectPosition->h;
	Entity_Draw(pRenderer, pBombe->pEntExplosion);
	*/
	return TRUE;
}
/* ========================================================================= */
/*!
* \file    kr_sprite.c
* \brief   Contains the structure to handle the sprite.
* \author  Olivier Herrou
* \version 1.0
* \date    21 Février 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 21/02/2015 | Creation.                                    */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#include "kr_sprite.h"

/*!
*  \fn     Kr_Sprite* Kr_Sprite_Alloc(Sint32 x, Sint32 y, Sint32 w, Sint32 h, char *szFile)
*  \brief  Function to initialize a Kr_Sprite structure
*
*  \param  x      the x position of the rectangle
*  \param  y      the y position of the rectangle
*  \param  w      the width of the rectangle
*  \param  h      the height of the rectangle
*  \param  szFile name of the sprite file (with the extension)
*  \return The initialized structure if everything is OK, NULL otherwise
*/
Kr_Sprite* Kr_Sprite_Alloc(Sint32 x, Sint32 y, Sint32 w, Sint32 h, char *szFile)
{
	Kr_Sprite *pSprite = NULL;
	SDL_Texture *pTexture = NULL;
	SDL_Surface *pSurface = NULL;
	char    szSpritePath[50];

	/* Vérification présence du fichier*/
	sprintf(szSpritePath, "sprites\\%s", szFile);
	Kr_Log_Print(KR_LOG_INFO, "Kr_Sprite_Alloc of the file %s\n", szSpritePath);
	if (UTIL_OpenFile(szSpritePath, "r") == NULL) 
	{
		return pSprite;
	}

	/* Création d'une surface à partir d'une image*/
	pSurface = IMG_Load(szSpritePath);
	if (pSurface == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Kr_Sprite_Alloc : Can't load an image!\n");
		Kr_Log_Print(KR_LOG_ERROR, ">> Path \"%s\".\n", szSpritePath);
		return pSprite;
	}
	/* Création de la texture à partir de la surface*/
	pTexture = SDL_CreateTextureFromSurface(gpRenderer, pSurface);
	SDL_FreeSurface(pSurface);
	if (pTexture == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Kr_Sprite_Alloc : Can't create a texture!\n");
		Kr_Log_Print(KR_LOG_ERROR, ">> Path \"%s\".\n", szSpritePath);
		return pSprite;
	}
	

	pSprite = (Kr_Sprite *)UTIL_Malloc(sizeof(Kr_Sprite));
	if (pSprite)
	{
		pSprite->rectPosition.x = x;
		pSprite->rectPosition.y = y;
		pSprite->rectPosition.w = w;
		pSprite->rectPosition.h = h;
		pSprite->iFrameWidth = 0; // A déterminer
		pSprite->iNbFrames = 0; // A déterminer
		pSprite->pTextureSprite = pTexture;
	}
	return pSprite;
}

void       Kr_Sprite_Free(Kr_Sprite *pSprite)
{
	UTIL_FreeTexture(&pSprite->pTextureSprite);
	UTIL_Free(pSprite);
}

Uint32     Kr_Sprite_Move(Kr_Sprite *pSprite, Uint32 iVectorX, Uint32 iVectorY)
{
	return 0;
}

void       Kr_Sprite_Draw(Kr_Sprite *pSprite, SDL_Renderer *pRenderer)
{

}
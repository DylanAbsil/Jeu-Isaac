/* ========================================================================= */
/*!
* \file    kr_sprite.c
* \brief   Contains the structure to handle the sprites.
* \author  Alexandre Robin
* \version 1.0
* \date    23 Mars 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Robin         | 23/03/2015 | Creation.                                    */
/*               |            |                                              */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#include "kr_sprite.h"

Kr_Sprite * init_Sprite(){
	Kr_Sprite * pSprite = UTIL_Malloc(sizeof(Kr_Sprite));
	pSprite->strName = "Nom indefini";
	pSprite->pTextureSprite = NULL;
	pSprite->iFrameHeight = 0;
	pSprite->iFrameWidth = 0;
	pSprite->iNbFrames = 0;
	pSprite->iCurrentFrame = 0;
	pSprite->pRectPosition = NULL;
	return pSprite;
}

void load_Sprite(Kr_Sprite *sprite, char *name, SDL_Texture *pTexture, Uint32 frameHeight, Uint32 frameWidth, Uint32 nbFrames, SDL_Rect *pRectPosition){
	// Creation d'un nouveau sprite et d'une nouvelle texture juste a partir du nom
	/*SDL_Texture *pSpriteEntite = NULL;
	char newSprFileName[SIZE_MAX_NAME];
	strcpy_s(newSprFileName, SIZE_MAX_NAME, name);
	strcat_s(newSprFileName, SIZE_MAX_NAME, "_sud");	
	pSpriteEntite = UTIL_LoadTexture(UTIL_BuildPath("sprite", newSprFileName, strlen(newSprFileName) - 1, "png"), NULL, pRectPosition );
	*/
	// Integration dans la structure
	sprite->strName = name;
	sprite->pTextureSprite = pTexture;
	sprite->iFrameHeight = frameHeight;
	sprite->iFrameWidth = frameWidth;
	sprite->iNbFrames = nbFrames;
	sprite->pRectPosition = pRectPosition;
}

void free_Sprite(Kr_Sprite *pSprite){
	UTIL_FreeTexture(&pSprite->pTextureSprite);
	free(pSprite);
}


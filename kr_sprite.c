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
	pSprite->strName = 0;
	pSprite->pTextureSprite = NULL;
	pSprite->iFrameWidth = 0;
	pSprite->iNbFrames = 0;
	pSprite->iCurrentFrame = 0;
	pSprite->pRectPosition = NULL;
	return pSprite;
}

void load_Sprite(Kr_Sprite *sprite, char *name, SDL_Texture *pTextureSprite, Uint32 frameWidth, Uint32 nbFrames, Uint32 currentFrame, SDL_Rect *pRectPosition){
	sprite->strName = strcpy(sprite->strName, name);
	sprite->pTextureSprite = pTextureSprite;
	sprite->iFrameWidth = frameWidth;
	sprite->iNbFrames = nbFrames;
	sprite->iCurrentFrame = currentFrame;
	sprite->pRectPosition = pRectPosition;
}

void free_Sprite(Kr_Sprite *pSprite){
	free(pSprite);
}


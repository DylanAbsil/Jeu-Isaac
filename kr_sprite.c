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

/*!
*  \fn     Kr_Sprite * init_Sprite()
*  \brief  Function to init a sprite
*
*  \todo   use this function at first to create a sprite then load
*
*  \param  none
*  \return Kr_Sprite* a pointer to the empty created entity
*/
Kr_Sprite * init_Sprite(){
	Kr_Sprite * pSprite = UTIL_Malloc(sizeof(Kr_Sprite));		//allocation mémoire
	pSprite->strName = "Nom indefini";
	pSprite->pTextureSprite = NULL;
	pSprite->iFrameHeight = 0;
	pSprite->iFrameWidth = 0;
	pSprite->iNbFrames = 0;
	pSprite->iCurrentFrame = 0;
	pSprite->pRectPosition = NULL;
	return pSprite;
}

/*!
*  \fn     void load_Sprite(Kr_Sprite *sprite, char *name, Uint32 frameHeight, Uint32 frameWidth, Uint32 nbFrames, SDL_Rect *pRectPosition)
*  \brief  Function to load the sprite
*
*  \todo   use this function after the sprite had been inited
*
*  \param sprite a pointer to a sprite
*  \param name the name of the name (name of the path)
*  \param frameHeight the height of the image in pixel
*  \param frameWidth the width
*  \param nbFrames the numper of frames of the animation
*  \param *pRectPosition	the position and the size visible on the screen
*  \return boolean it verify if the load is correct or not
*/
Boolean load_Sprite(Kr_Sprite *sprite, char *name, Uint32 frameHeight, Uint32 frameWidth, Uint32 nbFrames, SDL_Rect *pRectPosition){
	// Creation d'un nouveau sprite et d'une nouvelle texture juste a partir du nom
	SDL_Texture *pSpriteEntite = NULL;
	char newSprFileName[SIZE_MAX_NAME];

	sprintf(newSprFileName, "sprites/%s_sud.png", name);

	pSpriteEntite = UTIL_LoadTexture(newSprFileName, NULL, NULL);
	if (pSpriteEntite == NULL){
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the texture associate !\n", name);
		return FALSE;
	}

	// Integration dans la structure
	sprite->strName = newSprFileName;
	sprite->pTextureSprite = pSpriteEntite;
	if (sprite->pTextureSprite == NULL){
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the texture associate with %s !\n", name);
		return FALSE;
	}
	sprite->iFrameHeight = frameHeight;
	sprite->iFrameWidth = frameWidth;
	sprite->iNbFrames = nbFrames;
	sprite->pRectPosition = pRectPosition;

	Kr_Log_Print(KR_LOG_INFO, "Sprite %s of %d by %d has been loaded !\n", sprite->strName, sprite->iFrameWidth, sprite->iFrameHeight);
	return TRUE;
}

void free_Sprite(Kr_Sprite *pSprite){
	UTIL_FreeTexture(&pSprite->pTextureSprite);
	free(pSprite);
}


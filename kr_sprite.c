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
/* Herrou        | 28/03/2015 | Harmoniser les prototypes avec le projet     */
/*               |            | Mise en forme et corrections                 */
/*               |            | Kr_Sprite_Load add param SDL_Renderer        */
/*               |            | Prévention des inclusions multiples          */
/* Herrou        | 04/04/2015 | Initialisation du nom faite par strcpy       */
/*               |            | Le nom du sprite est donnée à Sprite_Init    */
/*               |            |    et non pas à Sprite_Load                  */
/*               |            | Le nom du sprite n'est plus alloué dynamiquement*/
/* ========================================================================= */

#include "kr_sprite.h"

/*!
*  \fn     Kr_Sprite *Kr_Sprite_Init(char *szFileName)
*  \brief  Function to init a sprite
*
*  \todo   use this function at first to create a sprite then load
*
*  \param szFileName the name of the sprite file must be allocated in the initialization
*  \return Kr_Sprite* a pointer to the empty created entity
*/
Kr_Sprite * Kr_Sprite_Init(char *szFileName)
{

	Kr_Sprite * pSprite = UTIL_Malloc(sizeof(Kr_Sprite));		//allocation mémoire

	strcpy(pSprite->strName, szFileName);
	pSprite->pTextureSprite = NULL;
	pSprite->iFrameHeight = 0;
	pSprite->iFrameWidth = 0;
	pSprite->iNbFrames = 0;
	pSprite->iCurrentFrame = 0;
	pSprite->pRectPosition = NULL;
	return pSprite;
}

/*!
*  \fn     Boolean Kr_Sprite_Load(Kr_Sprite *sprite, Uint32 frameHeight, Uint32 frameWidth, Uint32 nbFrames, SDL_Rect *pRectPosition)
*  \brief  Function to load the sprite
*
*  \todo   use this function after the sprite had been initialized
*
*  \param  sprite         a pointer to a sprite
*  \param  frameHeight    the height of the frame
*  \param  frameWidth     the width of the frame
*  \param  nbFrames       the numper of frames of the animation
*  \param  pRectPosition  a pointer to the position and the size of the sprite
*  \param  pRenderer      a pointer to the renderer  
*  \return TRUE if everything is ok, FALSE otherwise
*/
Boolean Kr_Sprite_Load(Kr_Sprite *sprite, Uint32 frameHeight, Uint32 frameWidth, Uint32 nbFrames, SDL_Rect *pRectPosition, SDL_Renderer *pRenderer){
	// Creation d'un nouveau sprite et d'une nouvelle texture juste a partir du nom
	SDL_Texture *pSpriteEntite = NULL;
	char newSprFileName[SIZE_MAX_NAME];

	sprintf(newSprFileName, "sprites/%s_sud.png", sprite->strName);

	pSpriteEntite = UTIL_LoadTexture(pRenderer, newSprFileName, NULL, NULL);
	if (pSpriteEntite == NULL){
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the texture of the sprite : %s!\n", newSprFileName);
		return FALSE;
	}

	// Integration dans la structure
	strcpy(sprite->strName, newSprFileName);
	sprite->pTextureSprite = pSpriteEntite;
	sprite->iFrameHeight   = frameHeight;
	sprite->iFrameWidth    = frameWidth;
	sprite->iNbFrames      = nbFrames;
	sprite->pRectPosition  = pRectPosition;

	Kr_Log_Print(KR_LOG_INFO, "Sprite %s of %d by %d has been loaded !\n", sprite->strName, sprite->iFrameWidth, sprite->iFrameHeight);
	return TRUE;
}

/*!
*  \fn     void Kr_Sprite_Free(Kr_Sprite *pSprite)
*  \brief  Function to freed a Kr_Sprite texture
*
*  \param  pSprite a pointer to the Kr_Sprite texture to free
*  \return none
*/
void Kr_Sprite_Free(Kr_Sprite *pSprite){
	UTIL_FreeTexture(&pSprite->pTextureSprite);
	UTIL_Free(pSprite);
}

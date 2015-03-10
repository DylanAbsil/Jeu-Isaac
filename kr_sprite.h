/* ========================================================================= */
/*!
* \file    kr_sprite.h
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

#ifndef __KR_SPRITE_H__
#define __KR_SPRITE_H__

#include "kr_common.h"
#include "kr_util.h"
#include "kr_log.h"


// http://openclassrooms.com/courses/tile-mapping/simple-personnage
// http://www.gnurou.org/writing/linuxmag/sdl/partie3
/*!
* \struct Kr_Sprite
* \brief  Structure to handle the Sprite.
*/
typedef struct
{
	char        *szSprName;		 /*!< Name of the sprite  */
	SDL_Texture *pTextureSprite; /*!< Texture of the sprite */
	Uint32       iFrameWidth;	 /*!< Frame width of the sprite  */
	Uint32       iNbFrames;		 /*!< Number of frames in the sprite  */
	SDL_Rect     rectPosition;	 /*!< Position of the sprite  */    // ON METTRA SUREMENT PAS CA LA APRES	
}Kr_Sprite;

Kr_Sprite* Kr_Sprite_Alloc(Sint32 x, Sint32 y, Sint32 w, Sint32 h, char *szFile);
void       Kr_Sprite_Free(Kr_Sprite *pSprite);
Uint32     Kr_Sprite_Move(Kr_Sprite *pSprite, Uint32 iVectorX, Uint32 iVectorY);
void       Kr_Sprite_Draw(Kr_Sprite *pSprite, SDL_Renderer *pRenderer );
#endif /* __KR_SPRITE_H__ */





/* ========================================================================= */
/*!
* \file    kr_entite.h
* \brief   Contains the structure to manage the sprites.
* \author  Alexandre Robin
* \version 1.0
* \date    23 Mars 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Robin         | 23/03/2015 | Cr�ation.                                    */
/*               |            |                                              */
/* ========================================================================= */

#include "kr_util.h"
#include "kr_log.h"

typedef struct {
	char		*strName;			/* Name of the sprite (contenant la direction ex : zelda_sud) */
	SDL_Texture *pTextureSprite;	/* Texture of the sprite */
	Uint32		iFrameHeight;		/* Frame height of the sprite */
	Uint32		iFrameWidth;		/* Frame width of the sprite */  // Un sprite n'a qu'une seule largeur
	Uint32		iNbFrames;			/* Number of frames in the sprite */
	Uint32		iCurrentFrame;		/* The current frame */
	SDL_Rect	*pRectPosition;		/* His position on the window */
}Kr_Sprite;


Kr_Sprite * Sprite_init();
Boolean Sprite_load(Kr_Sprite *sprite, char *name, Uint32 frameHeight, Uint32 frameWidth, Uint32 nbFrames, SDL_Rect *pRectposition);
void Sprite_free(Kr_Sprite *pSprite);
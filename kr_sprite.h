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
/* Robin         | 23/03/2015 | Création.                                    */
/* Herrou        | 28/03/2015 | Harmoniser les prototypes avec le projet     */
/*               |            | Mise en forme et corrections                 */
/*               |            | Kr_Sprite_Load add param SDL_Renderer        */
/*               |            | Prévention des inclusions multiples          */
/* Herrou        | 04/04/2015 | Initialisation du nom faite par strcpy       */
/*               |            | Le nom du sprite est donnée à Sprite_Init    */
/*               |            |    et non pas à Sprite_Load                  */
/*               |            | Le nom du sprite n'est plus alloué dynamiquement*/
/* ========================================================================= */

#ifndef __KR_SPRITE_H__
#define __KR_SPRITE_H__

#include "kr_util.h"
#include "kr_log.h"

typedef struct {
	char		strName[SIZE_MAX_NAME];		/*< Name of the sprite (contenant la direction ex : zelda_sud) >*/
	SDL_Texture *pTextureSprite;			/*< Texture of the sprite >*/
	Uint32		iFrameHeight;				/*< Frame height of the sprite >*/
	Uint32		iFrameWidth;				/*< Frame width of the sprite >*/  // Un sprite n'a qu'une seule largeur
	Uint32		iNbFrames;					/*< Number of frames in the sprite >*/
	Uint32		iCurrentFrame;				/*< The current frame >*/
	SDL_Rect	*pRectPosition;				/*< Sprite position on the window >*/
}Kr_Sprite;


Kr_Sprite *Kr_Sprite_Init(char* szFileName);
Boolean    Kr_Sprite_Load(Kr_Sprite *sprite, Direction dir, Uint32 frameHeight, Uint32 frameWidth, Uint32 nbFrames, SDL_Rect *pRectposition, SDL_Renderer *pRenderer);
void       Kr_Sprite_Free(Kr_Sprite *pSprite);


#endif /* __KR_SPRITE_H__ */
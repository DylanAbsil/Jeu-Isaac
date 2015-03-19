/* ========================================================================= */
/*!
* \file    kr_HUD.h
* \brief   Contains the structure to of the HUD.
* \author  Guillaume de Ferrand	
* \version 1.0
* \date    18 Mars 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Guillaume     | 18/03/2015 | Création.                                    */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */


#include "kr_util.h"
#include "kr_common.h"



/*===========================================================================*/
/*	  				       GESTION DES HUD  								 */
/*===========================================================================*/

typedef struct {
	char*		HUDName;			/*!< Name of the HUD */
	SDL_Rect	RectDest;			/*!< Rectangle of destination */
	SDL_Texture *pTexture;			/*!< Texture of the HUD */
	Boolean		estAffiche;			/*!< Boolean telling if the HUD is displayed */
} Kr_Hud;


Kr_Hud * Kr_HUD_Init(char *HUDName);
Kr_Hud * Kr_HUDSet(Kr_Hud *HUD, int x, int y, int w, int h, const char *HUDPath);
void Kr_HUDAffiche(SDL_Renderer * renderer, Kr_Hud *HUD, int NbRepet);
/* ========================================================================= */
/*!
* \file    HUD.h
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
#include "kr_log.h"


#define HUD_ESPACEMENT 4


/*===========================================================================*/
/*	  				       GESTION DES HUD  								 */
/*===========================================================================*/

typedef struct {
	char*		HUDName;			/*!< Name of the HUD */
	SDL_Rect	RectDest;			/*!< Rectangle of destination */
	SDL_Texture *pTexture;			/*!< Texture of the HUD */
	Boolean		estAffiche;			/*!< Boolean telling if the HUD is displayed */
} HUD;



HUD * HUD_Init(char *HUDName);
void Kr_HUDSet(HUD *pHUD, int x, int y, int w, int h, const char *HUDPath);
void Kr_HUDAffiche(SDL_Renderer * renderer, HUD *pHUD, int NbRepet);
void HUD_free(HUD *pHUD);





/*
Pour menu : 
	-> 2 fichiers différents
	-> Faire structure bouton (Nom, Rect)
	-> Faire structure Menu : Tableau de Rect
	-> Fonctions init, load, draw, free

*/
/* ========================================================================= */
/*!
* \file    HUD.h
* \brief   Contains the structure of the HUD.
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

#ifndef HUD_H
#define HUD_H

#define HUD_ESPACEMENT 4


/*===========================================================================*/
/*	  				       STRUCTURE DES HUD  								 */
/*===========================================================================*/

typedef struct {
	char*		HUDName;			/*!< Name of the HUD */
	SDL_Rect	RectDest;			/*!< Rectangle of destination */
	SDL_Texture *pTexture;			/*!< Texture of the HUD */
	Boolean		estAffiche;			/*!< Boolean telling if the HUD is displayed */
	Boolean     estTexte;           /*!< Boolean telling if the HUD contains text */
} HUD;


/*===========================================================================*/
/*	  				       FONCTIONS DES HUD  								 */
/*===========================================================================*/

HUD * HUD_Init(char *HUDName, Boolean estTexte);
void HUD_Load(HUD *pHUD, SDL_Rect rRect);
void HUD_Draw(SDL_Renderer * renderer, HUD *pHUD, Uint32 NbRepet);
void HUD_free(HUD *pHUD);
void HUD_Update(HUD *pHUD, SDL_Texture *pTexture);


#endif


/*
Pour menu : 
	-> 2 fichiers différents
	-> Faire structure bouton (Nom, Rect)
	-> Faire structure Menu : Tableau de boutton
	-> Fonctions init, load, draw, free

*/
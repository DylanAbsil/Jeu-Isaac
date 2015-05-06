/* ========================================================================= */
/*!
* \file    Menu.h
* \brief   Contains the structures of the menus.
* \author  Guillaume de Ferrand
* \version 1.0
* \date    22 Mars 2015
*/
/* ============================================================================= */
/* Developers    | Date       | Comments                                         */
/* --------------+------------+------------------------------------------------- */
/* Guillaume     | 22/03/2015 | Création                                         */
/* Guillaume     | 23/03/2015 | Modification struture - ajout image de fond      */
/*													  - ajout booleen selection  */
/* Herrou        | 06/05/2015 | Modification                                     */
/*																			     */
/* ============================================================================= */



#include "kr_util.h"
#include "kr_common.h"
#include "kr_log.h"
#include "kr_text.h"
#include "kr_input.h"


#ifndef MENU_H
#define MENU_H


/*===========================================================================*/
/*	  				      STRUCTURE POUR LES BOUTONS  	   					 */
/*===========================================================================*/

typedef struct
{
	SDL_Renderer   *pRenderer;				/*!< A pointer to the renderer */
	char		   *szBoutonName;			/*!< Name of the button */
	SDL_Rect		RectImage;				/*!< Rectangle of the background of the button */
	SDL_Rect		RectTexteActive;		/*!< Rectangle of the text of the button when it's activated */
	SDL_Rect		RectTexteDesactive;		/*!< Rectangle of the text of the button when it's desactivated */
	SDL_Texture	   *pTextureImageActive;	/*!< Texture of the background of the button when it's activated */
	SDL_Texture	   *pTextureImageDesactive;	/*!< Texture of the background of the button when it's not activated */
	SDL_Texture	   *pTextureSelection;		/*!< Texture to apply above the button when the mouse cursor is on it */
	SDL_Texture	   *pTextureActive;			/*!< Texture of the text of the button when it's activated */
	SDL_Texture	   *pTextureDesactive;		/*!< Texture of the text of the button when it's desactivated */
	Boolean			bSurvole;				/*!< Boolean telling if the button is selected */
	Boolean			bActive;				/*!< Boolean telling if the button is validated */
	Boolean			bMustShow;				/*!< Boolean telling if the button can be displayed */
} Bouton;




/*===========================================================================*/
/*	  				      FONCTIONS POUR LES BOUTONS  	   					 */
/*===========================================================================*/

/* Fonctions pour la structure bouton */
Bouton *Bouton_Init(char *szBoutonName, SDL_Renderer *pRenderer, char* szImageActive, char *szImageDesactive, char *szSelect);
Boolean	Bouton_Load(Bouton *pBouton, Boolean bMustShow, TTF_Font *pFont, SDL_Color Color, SDL_Rect RectImage, char* TexteActive, char* TexteNonActive);
void	Bouton_Draw(Bouton *pBouton);
void	Bouton_Free(Bouton *pBouton);
Boolean Bouton_estSurvole(Bouton *pBouton, Kr_Input myEvent);



#endif
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

typedef struct {
	char*		BoutonName;		    /*!< Name of the button */
	SDL_Rect	RectImage;			/*!< Rectangle of th background of the button */
	SDL_Rect    RectTexte;			/*!< Rectangle of the text of the button*/
	SDL_Texture *pTextureImage;		/*!< Texture of the background of the button */
	SDL_Texture *pTextureTexte;		/*!< Texture of the text of the button */
	char*       TexteActive;		/*!< Text to write on the button when it's validated */
	char*       TexteNonActive;		/*!< Text to write on the button when it's validated */
	Boolean     estSurvole;         /*!< Boolean telling if the button is selected */
	Boolean     estActive;          /*!< Boolean telling if the button is validated */
	Boolean		AffichageOK;		/*!< Boolean telling if the button can be displayed */
} Bouton;




/*===========================================================================*/
/*	  				      FONCTIONS POUR LES BOUTONS  	   					 */
/*===========================================================================*/

/* Fonctions pour la structure bouton */
Bouton * Bouton_Init(char *BoutonName);
void Bouton_Load(SDL_Renderer * renderer, Bouton *pBouton, TTF_Font *pFont, SDL_Color Color, SDL_Rect RectImage, char* TexteActive, char* TexteNonActive);
void Bouton_Draw(SDL_Renderer * renderer, Bouton *pBouton);
void Bouton_free(Bouton *pBouton);
void Bouton_UpdateImage(Bouton *pBouton);
Boolean Bouton_estSurvole(Bouton *pBouton, Kr_Input myEvent);
void Bouton_estActive(Bouton *pBouton);
void Bouton_UpdateTexte(SDL_Renderer * renderer, Bouton *pBouton, TTF_Font *pFont, SDL_Color Color);





/*===========================================================================*/
/*	  				      STRUCTURE POUR LES MENUS  	   					 */
/*===========================================================================*/

typedef struct {
	char*		 MenuName;		    /*!< Name of the menu */
	SDL_Rect	 RectFond;			/*!< Rectangle of th background of the menu */
	SDL_Texture  *pTextureFond;		/*!< texture of the background of the menu */
	//Uint32		 NbBouton;			/*!< Number of button present in this menu */
	//Bouton*		 TabBouton;			/*!< Array of the buttons displayed on the menu */
	Bouton       *pBouton1;
	Bouton       *pBouton2;
	Bouton       *pBouton3;
	Bouton       *pBouton4;
	Bouton       *pBouton5;
} Menu;




/*===========================================================================*/
/*	  				      FONCTIONS POUR LES MENUS  	   					 */
/*===========================================================================*/

Menu * Menu_Init(char *MenuName);
//void Menu_Load(Menu *pMenu, Uint32 NbBouton, ...);
//void Menu_Load(Menu *pMenu, Menu* TabMenu);
void Menu_Load(Menu *pMenu, Bouton *pBouton1, Bouton *pBouton2, Bouton *pBouton3, Bouton *pBouton4, Bouton *pBouton5);
void Menu_free(Menu *pMenu);
void Menu_Draw(SDL_Renderer * renderer, Menu *pMenu);




#endif
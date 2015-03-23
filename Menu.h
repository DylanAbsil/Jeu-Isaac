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





/*===========================================================================*/
/*	  				      STRUCTURE POUR LES BOUTONS  	   					 */
/*===========================================================================*/

typedef struct {
	char*		BoutonName;		    /*!< Name of the button */
	SDL_Rect	RectImage;			/*!< Rectangle of th background of the button */
	SDL_Rect    RectTexte;			/*!< Rectangle of the text of the button*/
	SDL_Texture *pTextureImage;		/*!< Texture of the background of the button */
	SDL_Texture *pTextureTexte;		/*!< Texture of the text of the button */
	Boolean     estSelectionne;     /*!< Boolean telling if the button is selected */
} Bouton;




/*===========================================================================*/
/*	  				      STRUCTURE POUR LES BOUTONS  	   					 */
/*===========================================================================*/

/* Fonctions pour la structure bouton */
Bouton * Bouton_Init(char *BoutonName);
void Bouton_Load(SDL_Renderer * renderer, Bouton *pBouton, TTF_Font *pFont, SDL_Color Color, SDL_Rect RectImage);
void Bouton_Draw(SDL_Renderer * renderer, Bouton *pBouton);
void Bouton_free(Bouton *pBouton);
void Bouton_Update(Bouton *pBouton);
void Bouton_estSelectionne(Bouton *pBouton);
void Bouton_nestPasSelectionne(Bouton *pBouton);




/*===========================================================================*/
/*	  				      STRUCTURE POUR LES MENUS  	   					 */
/*===========================================================================*/



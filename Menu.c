/* ========================================================================= */
/*!
* \file    Menu.c
* \brief   Contains the functions concerning the menus.
* \author  Guillaume de Ferrand
* \version 1.0
* \date    22 Mars 2015
*/
/* ================================================================================== */
/* Developers    | Date       | Comments                                              */
/* --------------+------------+------------------------------------------------------ */
/* Guillaume     | 22/03/2015 | Création                                              */
/* Guillaume     | 23/03/2015 | Ajout fonction Bouton_Update et Bouton_estSelectionne */
/* Guillaume     | 24/03/2015 | MAJ fonctions Update, estSelectionne                  */
/*																		       	      */
/* ================================================================================== */



#include "Menu.h"


/*!
*  \fn      Bouton * Bouton_Init(char *BoutonName)
*  \brief   Function to initiate a button
*
*  \param   BoutonName    name of the button structure
*
*  \return  the initialized structure
*/

Bouton * Bouton_Init(char *BoutonName)
{
	Bouton * pBouton = NULL;
	pBouton = (Bouton *)UTIL_Malloc(sizeof(Bouton));

	pBouton->BoutonName = BoutonName;

	pBouton->pTextureImage = NULL;
	pBouton->pTextureTexte = NULL;

	pBouton->RectImage.x = 0;
	pBouton->RectImage.y = 0;
	pBouton->RectImage.w = 0;
	pBouton->RectImage.h = 0;

	pBouton->RectTexte.x = 0;
	pBouton->RectTexte.y = 0;
	pBouton->RectTexte.w = 0;
	pBouton->RectTexte.h = 0;

	pBouton->estSelectionne = FALSE;

	return pBouton;
}




/*!
*  \fn      void Bouton_Load(SDL_Renderer * renderer, Bouton *pBouton, TTF_Font *pFont, SDL_Color Color, SDL_Rect RectImage)
*  \brief   Function to initiate a Bouton
*
*  \param   *renderer   the renderer to display the button
*  \param   *pBouton    the Bouton structure
*  \param   *pFont	    the font of the text of the button
*  \param   Color       the color of the text of the button 
*  \param   RectImage   the rectngle of the backgrund of the button
*
*  \return  void
*/

void Bouton_Load(SDL_Renderer * renderer, Bouton *pBouton, TTF_Font *pFont, SDL_Color Color, SDL_Rect RectImage)
{
	SDL_Rect RectTexteTampon = { 0, 0, 0, 0 };

	pBouton->RectImage.x = RectImage.x;
	pBouton->RectImage.y = RectImage.y;
	pBouton->RectImage.w = RectImage.w;
	pBouton->RectImage.h = RectImage.h;

	char BoutonPath[50];
	sprintf(BoutonPath, "Bouton\\%s.png", "Fond_Bouton");
	pBouton->pTextureImage = UTIL_LoadTexture(BoutonPath, NULL, NULL);
	if (pBouton->pTextureImage == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Impossible to load the sprite :%s\n", BoutonPath);
	}

	pBouton->pTextureTexte = Kr_Text_FontCreateTexture(renderer, pFont, pBouton->BoutonName, Color, TRUE, &RectTexteTampon);

	pBouton->RectTexte.x = pBouton->RectImage.x + ((pBouton->RectImage.w - RectTexteTampon.w) / 2);
	pBouton->RectTexte.y = pBouton->RectImage.y + ((pBouton->RectImage.h - RectTexteTampon.h) / 2);
	pBouton->RectTexte.w = RectTexteTampon.w;
	pBouton->RectTexte.h = RectTexteTampon.h;
}




/*!
*  \fn      void Bouton_Draw(SDL_Renderer * renderer, Bouton *pBouton)
*  \brief   Function to display the Bouton
*
*  \param   *renderer      the screen to display the Button
*  \param   *pBouton       the Bouton structure
*  \param   *pFont         the font used to create the texture of the button
*  \param   pBouton        the color of the font
*
*  \return  void
*/

void Bouton_Draw(SDL_Renderer * renderer, Bouton *pBouton)
{
	/*initialisation variable pour la texture du bouton*/
	//SDL_Texture *TextureBouton = NULL;
	/* remplissage de la texture */
	//TextureBouton = Kr_Text_FontCreateTexture(renderer, pFont, pBouton->BoutonName, Color, TRUE, &(pBouton->RectTexte));
	/* Affichage de l'image de fond du bouton */
	SDL_RenderCopy(renderer, pBouton->pTextureImage, NULL, &(pBouton->RectImage));
	/* Affichage de la texture texte */
	SDL_RenderCopy(renderer, pBouton->pTextureTexte, NULL, &(pBouton->RectTexte));
	/* Libération mémoire de la texture*/
	//UTIL_FreeTexture(&TextureBouton);
	return;
}




/*!
*  \fn      void Bouton_free(Bouton *pBouton)
*  \brief   Function to free the Bouton
*
*  \param   *pBouton      the bouton structure
*
*  \return  void
*/

void Bouton_free(Bouton *pBouton)
{
	UTIL_FreeTexture(&(pBouton->pTextureImage));
	UTIL_FreeTexture(&(pBouton->pTextureTexte));
	UTIL_Free(pBouton);
}




/*!
*  \fn      void Bouton_Update(Bouton *pBouton)
*  \brief   Function to update the texture of the background of the button
*
*  \param   *pBouton      the button structure
*
*  \return  void
*/

void Bouton_Update(Bouton *pBouton)
{
	char BoutonPath[50];
	if (pBouton->estSelectionne == TRUE)
	{
		UTIL_FreeTexture(&(pBouton->pTextureImage));
		sprintf(BoutonPath, "Bouton\\%s.png", "Fond_Bouton_Selectionne");
		pBouton->pTextureImage = UTIL_LoadTexture(BoutonPath, NULL, NULL);
		if (pBouton->pTextureImage == NULL)
		{
			Kr_Log_Print(KR_LOG_ERROR, "Impossible to load the sprite :%s\n", BoutonPath);
		}
		return;
	}
	else
	{
		UTIL_FreeTexture(&(pBouton->pTextureImage));
		sprintf(BoutonPath, "Bouton\\%s.png", "Fond_Bouton");
		pBouton->pTextureImage = UTIL_LoadTexture(BoutonPath, NULL, NULL);
		if (pBouton->pTextureImage == NULL)
		{
			Kr_Log_Print(KR_LOG_ERROR, "Impossible to load the sprite :%s\n", BoutonPath);
		}
		return;
	}
}




/*!
*  \fn      void Bouton_Selectionne(Bouton *pBouton)
*  \brief   Function to change the statute of estSelectionne into TRUE
*
*  \param   *pBouton      the bouton structure
*
*  \return  void
*/

void Bouton_estSelectionne(Bouton *pBouton)
{
		pBouton->estSelectionne = TRUE;
}




/*!
*  \fn      void Bouton_nestPasSelectionne(Bouton *pBouton)
*  \brief   Function to change the statute of estSelectionne into FALSE
*
*  \param   *pBouton      the bouton structure
*
*  \return  void
*/

void Bouton_nestPasSelectionne(Bouton *pBouton)
{
	pBouton->estSelectionne = FALSE;
}
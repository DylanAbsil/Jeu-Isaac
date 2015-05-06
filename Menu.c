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


/*===========================================================================*/
/*	  				      FONCTIONS POUR LES BOUTONS  	   					 */
/*===========================================================================*/

/*!
*  \fn      Bouton * Bouton_Init(char *BoutonName, SDL_Renderer *pRenderer, char* szImageActive, char *szImageDesactive, char *szSelect)
*  \brief   Function to initiate a button
*
*  \Remarks the size of all the image (activate, desactivate, selection) must be the same ! there's no control for it
*
*  \param   BoutonName			name of the button structure
*  \param   pRenderer			a pointer to the renderer
*  \param   pTextureImage		the name of the image of the button
*  \param   szImageActive		the name of the image of the button when it's activated
*  \param   szImageDesactive	the name of the image of the button when it's desactivated
*  \param   szSelect			the name of the image of the button when the mouse cursor is above it
*  \return  the initialized structure
*/
Bouton *Bouton_Init(char *BoutonName, SDL_Renderer *pRenderer, char* szImageActive, char *szImageDesactive, char *szSelect)
{
	Bouton	  *pBouton = NULL;
	Uint32     iNameLen = strlen(BoutonName);
	char szPath[60] = "";

	pBouton = (Bouton *)UTIL_Malloc(sizeof(Bouton));	

	pBouton->szBoutonName = UTIL_CopyStr(BoutonName, iNameLen);

	sprintf(szPath, "menu/%s.png", szImageActive);
	pBouton->pTextureImageActive = UTIL_LoadTexture(pRenderer, szPath, NULL, &(pBouton->RectImage));
	if (pBouton->pTextureImageActive == NULL) return pBouton = NULL;

	sprintf(szPath, "menu/%s.png", szImageDesactive);
	pBouton->pTextureImageDesactive = UTIL_LoadTexture(pRenderer, szPath, NULL, NULL);
	if (pBouton->pTextureImageDesactive == NULL) return pBouton = NULL;

	sprintf(szPath, "menu/%s.png", szSelect);
	pBouton->pTextureSelection = UTIL_LoadTexture(pRenderer, szPath, NULL, NULL);
	if (pBouton->pTextureSelection == NULL) return pBouton = NULL;

	pBouton->pRenderer = pRenderer;
	pBouton->pTextureActive = NULL;
	pBouton->pTextureDesactive = NULL;
	pBouton->RectTexteActive.x = 0;
	pBouton->RectTexteActive.y = 0;
	pBouton->RectTexteActive.w = 0;
	pBouton->RectTexteActive.h = 0;
	pBouton->RectTexteDesactive.x = 0;
	pBouton->RectTexteDesactive.y = 0;
	pBouton->RectTexteDesactive.w = 0;
	pBouton->RectTexteDesactive.h = 0;
	pBouton->bSurvole = FALSE;
	pBouton->bActive = FALSE;
	pBouton->bMustShow = FALSE;
	return pBouton;
}


/*!
*  \fn      void Bouton_Free(Bouton *pBouton)
*  \brief   Function to free a button structure
*
*  \param   pBouton  the structure to free
*  \return  none
*/
void Bouton_Free(Bouton *pBouton)
{
	UTIL_FreeTexture(&pBouton->pTextureImageActive);
	UTIL_FreeTexture(&pBouton->pTextureImageDesactive);
	UTIL_FreeTexture(&pBouton->pTextureSelection);
	UTIL_FreeTexture(&pBouton->pTextureActive);
	UTIL_FreeTexture(&pBouton->pTextureDesactive);
	UTIL_Free(pBouton);
}


/*!
*  \fn     Boolean Bouton_Load(Bouton *pBouton, Boolean bMustShow, TTF_Font *pFont, SDL_Color Color, SDL_Rect RectImage, char* TexteActive, char* TexteNonActive)
*  \brief  Function to load a bouton structure
*
*  \param   pBouton			the bouton structure
*  \param   bMustShow		TRUE to display the button
*  \param   Color			the font
*  \param   RectImage		the position of the button
*  \param   TexteActive		the string to display when the button is activated
*  \param   TexteNonActive	the string to display when the button is not activated
*  \return  TRUE if everything is ok, FALSE otherwise
*/
Boolean Bouton_Load(Bouton *pBouton, Boolean bMustShow, TTF_Font *pFont, SDL_Color Color, SDL_Rect RectImage, char* TexteActive, char* TexteNonActive)
{
	pBouton->bMustShow = bMustShow;

	pBouton->RectImage.x = RectImage.x;
	pBouton->RectImage.y = RectImage.y;

	UTIL_FreeTexture(&pBouton->pTextureActive);
	UTIL_FreeTexture(&pBouton->pTextureDesactive);

	// Activé
	pBouton->pTextureActive = Kr_Text_FontCreateTexture(pBouton->pRenderer, pFont, TexteActive, Color, TRUE, &pBouton->RectTexteActive);
	if (pBouton->pTextureActive == NULL)
	{
		Kr_Log_Print(KR_LOG_WARNING, "Can't update the text of the button structure (activate)!\n");
		return FALSE;
	}
	pBouton->RectTexteActive.x = pBouton->RectImage.x + ((pBouton->RectImage.w - pBouton->RectTexteActive.w) / 2);
	pBouton->RectTexteActive.y = pBouton->RectImage.y + ((pBouton->RectImage.h - pBouton->RectTexteActive.h) / 2);
	
	// Désactivé
	pBouton->pTextureDesactive = Kr_Text_FontCreateTexture(pBouton->pRenderer, pFont, TexteNonActive, Color, TRUE, &pBouton->RectTexteDesactive);
	if (pBouton->pTextureDesactive == NULL)
	{
		Kr_Log_Print(KR_LOG_WARNING, "Can't update the text of the button structure (desactivate)!\n");
		return FALSE;
	}
	pBouton->RectTexteDesactive.x = pBouton->RectImage.x + ((pBouton->RectImage.w - pBouton->RectTexteDesactive.w) / 2);
	pBouton->RectTexteDesactive.y = pBouton->RectImage.y + ((pBouton->RectImage.h - pBouton->RectTexteDesactive.h) / 2);

	return TRUE;
}




/*!
*  \fn     void Bouton_Draw(Bouton *pBouton)
*  \brief  Function to draw a button
*
*  \param   pBouton	the bouton structure
*  \return none
*/
void Bouton_Draw(Bouton *pBouton)
{
	if (!pBouton->bMustShow) return;

	if (pBouton->bActive)
	{
		SDL_RenderCopy(pBouton->pRenderer, pBouton->pTextureImageActive, NULL, &pBouton->RectImage);
		SDL_RenderCopy(pBouton->pRenderer, pBouton->pTextureActive, NULL, &pBouton->RectTexteActive);
	}
	else
	{
		SDL_RenderCopy(pBouton->pRenderer, pBouton->pTextureImageDesactive, NULL, &pBouton->RectImage);
		SDL_RenderCopy(pBouton->pRenderer, pBouton->pTextureDesactive, NULL, &pBouton->RectTexteDesactive);
	}
}

/*!
*  \fn     Boolean Bouton_estSurvole(Bouton *pBouton, Kr_Input myEvent)
*  \brief  Function to draw a button
*
*  \param   pBouton	the bouton structure
*  \param   myEvent	the Kr_Input structure to get access to the coordinate of the mouse
*  \return none
*/
Boolean Bouton_estSurvole(Bouton *pBouton, Kr_Input myEvent)
{
	if (!pBouton->bMustShow) return FALSE; // Pas de vérification si le bouton n'est pas affiché

	if (((Uint32)(pBouton->RectImage.x) <= myEvent.iMouseX) && (myEvent.iMouseX <= (Uint32)(pBouton->RectImage.x + pBouton->RectImage.w)) &&
		((Uint32)(pBouton->RectImage.y) <= myEvent.iMouseY) && (myEvent.iMouseY <= (Uint32)(pBouton->RectImage.y + pBouton->RectImage.h)))
	{
		pBouton->bSurvole = TRUE;
		SDL_RenderCopy(pBouton->pRenderer, pBouton->pTextureSelection, NULL, &pBouton->RectImage);
		return TRUE;
	}
	else
	{
		pBouton->bSurvole = FALSE;
		return FALSE;
	}
}
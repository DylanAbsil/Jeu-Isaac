/* ========================================================================= */
/*!
* \file    HUD.c
* \brief   Contains the functions to display the HUD.
* \author  Guillaume de Ferrand
* \version 1.0
* \date    18 Mars 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Guillaume     | 18/03/2015 | Création                                     */
/* Guillaume     | 19/03/2015 | Gestion erreur dans HUD_Load                 */
/* Guillaume	 | 20/03/2015 | Création HUD_Update							 */
/*																			 */
/* ========================================================================= */




#include "HUD.h"



/*!
*  \fn      HUD * HUD_Init(char *HUDName, Boolean estTexte)
*  \brief   Function to initiate a HUD
*
*  \param   HUDName    the HUD structure
*  \param   estTexte   a boolean to say if the HUD is composed of texte
*  \param   pRenderer  a pointer to the renderer
*  \return  the initialized structure
*/
HUD * HUD_Init(char *HUDName, Boolean estTexte, SDL_Renderer *pRenderer)
{
	HUD * pHUD = NULL;
	pHUD = (HUD *)UTIL_Malloc(sizeof(HUD));

	pHUD->pRenderer = pRenderer;
	pHUD->HUDName = HUDName;
	
	pHUD->pTexture = NULL;
	
	pHUD->RectDest.x = 0;
	pHUD->RectDest.y = 0;
	pHUD->RectDest.w = 0;
	pHUD->RectDest.h = 0;

	pHUD->estAffiche = TRUE;
	pHUD->estTexte = estTexte;
	return pHUD;
}



/*!
*  \fn      void HUD_Load(HUD *pHUD, SDL_Rect rRect)
*  \brief   Function to initiate a HUD
*
*  \param   *pHUD       the HUD structure
*  \param   rRect	    the rectangle of the HUD
*
*  \return  void
*/
void HUD_Load(HUD *pHUD, SDL_Rect rRect)
{
	pHUD->RectDest.x = rRect.x;
	pHUD->RectDest.y = rRect.y;
	pHUD->RectDest.w = rRect.w;
	pHUD->RectDest.h = rRect.h;

	char HUDPath[50];
	if (pHUD->estTexte == FALSE)
	{
		sprintf(HUDPath, "hud\\%s.png", pHUD->HUDName);
		pHUD->pTexture = UTIL_LoadTexture(pHUD->pRenderer,HUDPath, NULL, NULL);
		if (pHUD->pTexture == NULL)
		{
			Kr_Log_Print(KR_LOG_ERROR, "Impossible to load the sprite :%s\n", HUDPath);
		}
	}
}





/*!
*  \fn      void HUD_Draw(SDL_Renderer * renderer, HUD *pHUD, Uint32 NbRepet)
*  \brief   Function to display the HUD
*
*  \param   *renderer      the screen to display the HUD
*  \param   pHUD           the HUD structure
*  \param   NbRepet        the number of time the display of one sprite must be repeated
*
*  \return  void
*/
void HUD_Draw(SDL_Renderer * renderer, HUD *pHUD, Uint32 NbRepet)
{
	// test si on doit afficher
	if ((pHUD->estAffiche) == TRUE)
	// on affiche le HUD
	{
		Uint32 i = 0;
		SDL_Rect CopieRectDest;
		CopieRectDest = pHUD->RectDest;

		for (i = 0; i <= NbRepet; i++)
		{
			SDL_RenderCopy(renderer, pHUD->pTexture, NULL, &CopieRectDest);
			CopieRectDest.x -= (CopieRectDest.w + HUD_ESPACEMENT);
		}
		return;
	}
	// on n'affiche rien
	else return;
}




/*!
*  \fn      void HUD_free(HUD *pHUD)
*  \brief   Function to free the HUD
*
*  \param   *pHUD      the HUD structure
*
*  \return  void
*/
void HUD_free(HUD *pHUD)
{
	UTIL_FreeTexture(&(pHUD->pTexture));
	UTIL_Free(pHUD);
}
 




/*!
*  \fn      void HUD_Update(HUD *pHUD, SDL_Texture *pTexture)
*  \brief   Function to update the texture of the HUD
*
*  \param   *pHUD      the HUD structure
*  \param   *pTexture  the new texture
*
*  \return  void
*/
void HUD_Update(HUD *pHUD, SDL_Texture *pTexture)
{
	pHUD->pTexture = pTexture;
}
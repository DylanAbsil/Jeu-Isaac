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
/*																			 */
/* ========================================================================= */




#include "HUD.h"



/*!
*  \fn      HUD * HUD_Init(char *HUDName)
*  \brief   Function to initiate a HUD
*
*  \param   HUDName    the HUD structure
*  \return  a HUD structure
*/

HUD * HUD_Init(char *HUDName)
{
	HUD * pHUD = NULL;
	pHUD = (HUD *)UTIL_Malloc(sizeof(HUD));

	pHUD->HUDName = HUDName;
	
	pHUD->pTexture = NULL;
	
	pHUD->RectDest.x = 0;
	pHUD->RectDest.y = 0;
	pHUD->RectDest.w = 0;
	pHUD->RectDest.h = 0;

	pHUD->estAffiche = FALSE;
}



/*!
*  \fn      void HUD_Load(HUD *pHUD, int x, int y, int w, int h, const char *HUDPath)
*  \brief   Function to initiate a HUD
*
*  \param   pHUD               the HUD structure
*  \param   int x                 the x coordonate of the HUD on the screen
*  \param   int y                 the y coordonate of the HUD on the screen
*  \param   int w                 the width of the HUD
*  \param   int h                 the height of the HUD
*  \param   const char *HUDPath   the name of the sprite of the HUD
*
*  \return  a HUD structure
*/


// a changer : --> Envoyer rect dans la fonction


void HUD_Load(HUD *pHUD, int x, int y, int w, int h, const char *HUDPath)
{
	pHUD->RectDest.x = x;
	pHUD->RectDest.y = y;
	pHUD->RectDest.w = w;
	pHUD->RectDest.h = h;

	//HUD->RectDest.h = rRect.h;
	pHUD->pTexture = UTIL_LoadTexture(HUDPath, NULL, NULL);  //vérfier si renvoie nul et donc la fonction renvoie un entier
}





/*!
*  \fn      void HUD_Draw(SDL_Renderer * renderer, HUD *pHUD, int NbRepet)
*  \brief   Function to display the HUD
*
*  \param   pHUD           the HUD structure
*  \param   int NbRepet    the number of time the display of one sprite must be repeated
*
*  \return  void
*/

void HUD_Draw(SDL_Renderer * renderer, HUD *pHUD, int NbRepet)
{
	// test si on doit afficher
	if (pHUD->estAffiche == TRUE)

	// on affiche le HUD
	{
		int i = 0;
		for (i = 0; i <= NbRepet; i++)
		{
			SDL_RenderCopy(renderer, pHUD->pTexture, NULL, &(pHUD->RectDest));
			pHUD->RectDest.x += (pHUD->RectDest.w + HUD_ESPACEMENT);
			i++;
		}
	}
	// on n'affiche rien
	else return;
}



void HUD_free(HUD *pHUD)
{
	UTIL_FreeTexture(&(pHUD->pTexture));
	UTIL_Free(pHUD);
}
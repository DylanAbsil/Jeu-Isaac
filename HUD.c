/* ========================================================================= */
/*!
* \file    kr_HUD.c
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

//  --> Fontion Kr_HUDSet : un HUD en paramètre ???



#include "kr_HUD.h"



/*!
*  \fn      Kr_HUD * Kr_HUD_Init(char *HUDName)
*  \brief   Function to initiate a Kr_Level
*
*  \param   HUDName    the HUD structure
*  \return  a Kr_HUD structure
*/

Kr_Hud * Kr_HUD_Init(char *HUDName)
{
	Kr_Hud * HUD = NULL;
	HUD = (Kr_Hud *)UTIL_Malloc(sizeof(Kr_Hud));

	HUD->HUDName = HUDName;
	
	HUD->pTexture = NULL;
	
	HUD->RectDest.x = 0;
	HUD->RectDest.y = 0;
	HUD->RectDest.w = 0;
	HUD->RectDest.h = 0;

	HUD->estAffiche = FALSE;
}



/*!
*  \fn      Kr_HUD *Kr_HUDSet(Kr_Hud *HUD, int x, int y, int w, int h, const char *HUDPath)
*  \brief   Function to initiate a HUD
*
*  \param   HUDName               the HUD structure
*  \param   int x                 the x coordonate of the HUD on the screen
*  \param   int y                 the y coordonate of the HUD on the screen
*  \param   int w                 the width of the HUD
*  \param   int h                 the height of the HUD
*  \param   const char *HUDPath   the name of the sprite of the HUD
*
*  \return  a Kr_HUD structure
*/


//Problème --> Si c'est un HUD pour le nombre de vie ou munitation :
//             alors on a un texte qui donne le nombre de vie (ou de munition) et pas une image...

Kr_Hud * Kr_HUDSet(Kr_Hud *HUD, int x, int y, int w, int h, const char *HUDPath)
{
	HUD->RectDest.x = x;
	HUD->RectDest.y = y;
	HUD->RectDest.w = w;
	HUD->RectDest.h = h;

	HUD->pTexture = UTIL_LoadTexture(HUDPath, NULL, NULL);
}





/*!
*  \fn      void Kr_HUDAffiche(Kr_Hud *HUD, int NbRepet)
*  \brief   Function to display the HUD
*
*  \param   HUD            the HUD structure
*  \param   int NbRepet    the number of time the display of one sprite must be repeated
*
*  \return  void
*/

void Kr_HUDAffiche(SDL_Renderer * renderer, Kr_Hud *HUD, int NbRepet)
{
	// test si on doit afficher
	if (HUD->estAffiche == TRUE)

	// on affiche le HUD
	{
		int i = -1;
		for (i = -1; i = NbRepet; i++)
		{
			SDL_RenderCopy(renderer, HUD->pTexture, NULL, &(HUD->RectDest));
			HUD->RectDest.x += 32;
			i++;
		}
	}
	// on n'affiche rien
	else return;
}
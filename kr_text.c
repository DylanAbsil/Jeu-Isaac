/* ========================================================================= */
/*!
 * \file    kr_log.c
 * \brief   Contains the functions to handle the SDL_ttf.
 * \author  Olivier Herrou
 * \version 1.0
 * \date    8 Février 2015
 */
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 08/02/2015 | Creation.                                    */
/*               | 21/02/2015 | La police doit être dans le dossier font     */
/*               |            |                                              */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#include "kr_text.h"

/* ========================================================================= */

/*!
*  \fn     TTF_Font* Kr_Text_OpenFont(const char szFile, Uint32 iSize);
*  \brief  Function to open a font.
*
*  \param  iSize  Integer for the size of the font.
*  \param  szFile Path of the font to open.
*  \return A pointer on the loaded font, or NULL if error.
*/
TTF_Font* Kr_Text_OpenFont(const char *szFile, Uint32 iSize)
{
	TTF_Font *pFont = NULL;
	char     *szFontPath;
	size_t    iPathLen;

	iPathLen = strlen(szFile);
	szFontPath = UTIL_BuildPath("fonts", szFile, iPathLen, "ttf"); // Le fichier font doit être dans le répertoire "fonts" et avoir l'extension .ttf
	if (!szFontPath)
	{
		Kr_Log_Print(KR_LOG_ERROR, "The file %s.ttf is not in the folder \"fonts\"\n", szFile);
		return pFont;
	}
	pFont = TTF_OpenFont(szFontPath, iSize);
	if (!pFont)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't open a font !\n");
		Kr_Log_Print(KR_LOG_ERROR, ">> Path \"%s\".\n", szFontPath);
	}
	return pFont;
}

/*!
*  \fn     SDL_Texture* Kr_Text_FontCreateTexture(SDL_Renderer *pRenderer, TTF_Font *pFont, const char szText, SDL_Color Color, Boolean bBlended, SDL_Rect *pTextureSize);
*  \brief  Function to create a texture from a message
*
*  \param  pRenderer    Pointer to the renderer
*  \param  pFont        Pointer to the font
*  \param  szText       Message which will be added to the texture
*  \param  Color        Color of the message
*  \param  bBlended     Use Blended render if true and Solid if false
*  \param  pTextureSize Pointer to get the size of the texture loaded (Can be NULL).
*  \return The texture created from the message, or NULL if error
*/
SDL_Texture* Kr_Text_FontCreateTexture(SDL_Renderer *pRenderer, TTF_Font *pFont, const char* szText, SDL_Color Color, Boolean bBlended, SDL_Rect *pTextureSize)
{
	SDL_Surface *pSurface = NULL;
	SDL_Texture *pTexture = NULL;


	if (bBlended)
	{
		pSurface = TTF_RenderText_Blended(pFont, szText, Color);
	}
	else
	{
		pSurface = TTF_RenderText_Solid(pFont, szText, Color);
	}


	if (pSurface == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't create the surface for TTF, mode : %s  |  message : %s!\n", bBlended ? "Blended" : "Solid", szText);
	}
	else
	{
		pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
		if (pTexture == NULL)
		{
			Kr_Log_Print(KR_LOG_ERROR, "Can't create the texture TTF, mode : %s  |  message : %s!\n", bBlended ? "Blended" : "Solid", szText);
		}
		else
		{
			if (pTextureSize)
			{
				pTextureSize->w = pSurface->w;
				pTextureSize->h = pSurface->h;
			}
		}
		SDL_FreeSurface(pSurface);
	}
	return pTexture;
}


/*!
*  \fn     void Kr_Text_CloseFont( TTF_Font **ppFont );
*  \brief  Function to free a font.
*
*  \param  ppFont Pointer to pointer of the font to free.
*  \return None.
*/

void Kr_Text_CloseFont(TTF_Font **ppFont)
{
	TTF_CloseFont(*ppFont);
	*ppFont = NULL;
}
/* Utilisation de cette bibliothèque */
/* Remarque, ne pas compiler ce code, il faut l'adapter pour effectuer un code exploitable/analysable */
/*
void main(void)
{
	SDL_Rect     textPosition;
	SDL_Color    couleur = { 123, 255, 0 };
	SDL_Texture *pTextureText = NULL;
	TTF_Font    *pFont = NULL;
	char         szMaChaine[100] = "Phrase à afficher";

	textPosition.x = 0; // Position à laquelle on souhaitera afficher le texte
	textPosition.y = 0;
	pFont = Kr_Text_OpenFont("cour", 25); // Cette fonction renvoie une structure TTF_Font ou NULL si erreur
										  // 1er paramètre  : le nom de la police (sans extension, mais celle-ci doit etre .ttf) qui doit être dans le dossier fonts\
										  // 2eme paramètre : La taille de la police

	TTF_SetFontStyle(pFont, TTF_STYLE_BOLD); // Applique un style à la police si on souhaite
											 // Style possible : TTF_STYLE_NORMAL, TTF_STYLE_BOLD, TTF_STYLE_ITALIC, TTF_STYLE_UNDERLINE

	pTextureText = Kr_Text_FontCreateTexture(gpRenderer, pFont, szMaChaine, couleur, TRUE, &textPosition);
	// Création d'une texture contenant le szMaChaine d'une certaine couleur avec le mode Blended, plus d'info dans le prototype de la fonction  
	SDL_RenderCopy(gpRenderer, pTextureText, NULL, &textPosition); // On affiche ensuite la texture d'une manière classique

	Kr_Text_CloseFont(&pFont);			// Libération mémoire de la police
}*/
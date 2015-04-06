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



/*



/*!
*  \fn      Bouton * Bouton_Init(char *BoutonName)
*  \brief   Function to initiate a button
*
*  \param   BoutonName    name of the button structure
*
*  \return  the initialized structure
*/
/*
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

	pBouton->TexteActive = NULL;
	pBouton->TexteNonActive = NULL;

	pBouton->estSurvole = FALSE;
	pBouton->estActive = FALSE;
	pBouton->AffichageOK = FALSE;

	return pBouton;
}
*/



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
/*
void Bouton_Load(SDL_Renderer * renderer, Bouton *pBouton, TTF_Font *pFont, SDL_Color Color, SDL_Rect RectImage, char* TexteActive, char* TexteNonActive)
{
	SDL_Rect RectTexteTampon = { 0, 0, 0, 0 };

	pBouton->RectImage.x = RectImage.x;
	pBouton->RectImage.y = RectImage.y;
	pBouton->RectImage.w = RectImage.w;
	pBouton->RectImage.h = RectImage.h;

	pBouton->TexteActive = TexteActive;
	pBouton->TexteNonActive = TexteNonActive;

	char BoutonPath[50];
	sprintf(BoutonPath, "Bouton\\%s.png", "Fond_Bouton");
	pBouton->pTextureImage = UTIL_LoadTexture(BoutonPath, NULL, NULL);
	if (pBouton->pTextureImage == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Impossible to load the sprite :%s\n", BoutonPath);
	}

	pBouton->pTextureTexte = Kr_Text_FontCreateTexture(renderer, pFont, pBouton->TexteActive, Color, TRUE, &RectTexteTampon);

	pBouton->RectTexte.x = pBouton->RectImage.x + ((pBouton->RectImage.w - RectTexteTampon.w) / 2);
	pBouton->RectTexte.y = pBouton->RectImage.y + ((pBouton->RectImage.h - RectTexteTampon.h) / 2);
	pBouton->RectTexte.w = RectTexteTampon.w;
	pBouton->RectTexte.h = RectTexteTampon.h;

	pBouton->AffichageOK = TRUE;
	pBouton->estActive = TRUE;
}*/




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
/*
void Bouton_Draw(SDL_Renderer * renderer, Bouton *pBouton)
{
	// Test pour savoir si le bouton peut être affiché 
	if (pBouton->AffichageOK == TRUE)
	{
		// Affichage de l'image de fond du bouton
		SDL_RenderCopy(renderer, pBouton->pTextureImage, NULL, &(pBouton->RectImage));
		// Affichage de la texture texte 
		SDL_RenderCopy(renderer, pBouton->pTextureTexte, NULL, &(pBouton->RectTexte));
		return;
	}
	else{ return; }
}

*/


/*!
*  \fn      void Bouton_free(Bouton *pBouton)
*  \brief   Function to free the Bouton
*
*  \param   *pBouton      the bouton structure
*
*  \return  void
*/
/*
void Bouton_free(Bouton *pBouton)
{
	UTIL_FreeTexture(&(pBouton->pTextureImage));
	UTIL_FreeTexture(&(pBouton->pTextureTexte));
	UTIL_Free(pBouton);
}
*/


/*!
*  \fn      void Bouton_UpdateImage(Bouton *pBouton)
*  \brief   Function to update the texture of the background of the button
*
*  \param   *pBouton      the button structure
*
*  \return  void
*/
/*
void Bouton_UpdateImage(Bouton *pBouton)
{
	char BoutonPath[50];
	if (pBouton->estSurvole == TRUE)
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
}*/



/*!
*  \fn      void Bouton_ChangeEtatSurvol(Bouton *pBouton, Boolean Survole)
*  \brief   Function to change the statute of estSurvole
*
*  \param   *pBouton      the bouton structure
*  \param   Survole		  Boolean telling if the mouse is on the button
*
*  \return  void
*/

//void Bouton_ChangeEtatSurvol(Bouton *pBouton, Boolean Survole)
//{
//	pBouton->estSurvole = Survole;
//}

/*
Boolean Bouton_estSurvole(Bouton *pBouton, Kr_Input myEvent)
{
	if ((pBouton->RectImage.x <= myEvent.iMouseX) &&
		(myEvent.iMouseX <= (pBouton->RectImage.x + pBouton->RectImage.w)) &&
		(pBouton->RectImage.y <= myEvent.iMouseY) &&
		myEvent.iMouseY <= (pBouton->RectImage.y + pBouton->RectImage.h))
	{
		pBouton->estSurvole = TRUE;
		Bouton_UpdateImage(pBouton);
		return TRUE;
	}
	else
	{
		pBouton->estSurvole = FALSE;
		Bouton_UpdateImage(pBouton);
		return FALSE;
	}
}*/




/*!
*  \fn      void Bouton_estActive(Bouton *pBouton, Boolean Survole)
*  \brief   Function to change the statute of estSelectionne into TRUE
*
*  \param   *pBouton      the bouton structure
*  \param   Active		  Boolean telling if the mouse is on the button
*
*  \return  void
*/
/*
void Bouton_estActive(Bouton *pBouton)
{
	if (pBouton->estActive == TRUE)
	{
		pBouton->estActive = FALSE;
	}
	else
	{
		pBouton->estActive = TRUE;
	}
}*/




/*!
*  \fn      void Bouton_UpdateImage(Bouton *pBouton)
*  \brief   Function to update the texture of the background of the button
*
*  \param   *pBouton      the button structure
*
*  \return  void
*/
/*
void Bouton_UpdateTexte(SDL_Renderer * renderer, Bouton *pBouton, TTF_Font *pFont, SDL_Color Color)
{
	SDL_Rect RectTexteTampon = { 0, 0, 0, 0 };
	if (pBouton->estActive == TRUE)
	{
		UTIL_FreeTexture(&(pBouton->pTextureTexte));
		pBouton->pTextureTexte = Kr_Text_FontCreateTexture(renderer, pFont, pBouton->TexteActive, Color, TRUE, &RectTexteTampon);
		/* Calcul des nouveaux coordonnes de RectTexte */
/*		pBouton->RectTexte.x = pBouton->RectImage.x + ((pBouton->RectImage.w - RectTexteTampon.w) / 2);
		pBouton->RectTexte.y = pBouton->RectImage.y + ((pBouton->RectImage.h - RectTexteTampon.h) / 2);
		pBouton->RectTexte.w = RectTexteTampon.w;
		pBouton->RectTexte.h = RectTexteTampon.h;
		return;
	}
	else
	{
		UTIL_FreeTexture(&(pBouton->pTextureTexte));
		pBouton->pTextureTexte = Kr_Text_FontCreateTexture(renderer, pFont, pBouton->TexteNonActive, Color, TRUE, &RectTexteTampon);
		/* Calcul des nouveaux coordonnes de RectTexte */
/*		pBouton->RectTexte.x = pBouton->RectImage.x + ((pBouton->RectImage.w - RectTexteTampon.w) / 2);
		pBouton->RectTexte.y = pBouton->RectImage.y + ((pBouton->RectImage.h - RectTexteTampon.h) / 2);
		pBouton->RectTexte.w = RectTexteTampon.w;
		pBouton->RectTexte.h = RectTexteTampon.h;
		return;
	}
}*/




/*===========================================================================*/
/*	  				      FONCTIONS POUR LES MENUS  	   					 */
/*===========================================================================*/


/*!
*  \fn      Menu * Menu_Init(char *MenuName, Uint32 NbBouton)
*  \brief   Function to initiate a Menu
*
*  \param   MenuName    name of the Menu
*  \param   NbBouton	Number of button persent in the Menu
*
*  \return  the initialized structure
*/
/*
Menu * Menu_Init(char *MenuName)
{
	Menu * pMenu = NULL;
	pMenu = (Menu *)UTIL_Malloc(sizeof(Menu));

	pMenu->MenuName = MenuName;

	pMenu->RectFond.x = 0;
	pMenu->RectFond.y = 0;
	pMenu->RectFond.w = 0;
	pMenu->RectFond.h = 0;

	//pMenu->NbBouton = NbBouton;

	pMenu->pTextureFond = NULL;

	pMenu->pBouton1 = NULL;
	pMenu->pBouton2 = NULL;
	pMenu->pBouton3 = NULL;
	pMenu->pBouton4 = NULL;
	pMenu->pBouton5 = NULL;


	//pMenu->TabBouton = malloc(NbBouton * sizeof(Bouton));

	return pMenu;
}*/




/*!
*  \fn      void Menu_Load(Menu *pMenu, ...)
*  \brief   Function to load a Menu
*
*  \param   *pMenu    a menu structure
*
*  \return  void
*/

/*void Menu_Load(Menu *pMenu, Uint32 NbBouton, ...)*/
/*
void Menu_Load(Menu *pMenu, Bouton *pBouton1, Bouton *pBouton2, Bouton *pBouton3, Bouton *pBouton4, Bouton *pBouton5)
{
	//Remplissage de TabBouton ?????

	//Uint32 i = 0;
/*	Bouton parametre;
	va_list ap;
	va_start(ap, NbBouton);
	for (i = 0; i < NbBouton; i++)
	{ 
		pMenu->TabBouton[i] = parametre;
		parametre = va_arg(ap, Bouton);
	}
	va_end(ap);
*/
/*
//	pMenu->TabBouton[0] = pBouton0;
//	pMenu->TabBouton = TabMenu;

	pMenu->RectFond.w = 1280;
	pMenu->RectFond.h = 704;

	pMenu->pBouton1 = pBouton1;
	pMenu->pBouton2 = pBouton2;
	pMenu->pBouton3 = pBouton3;
	pMenu->pBouton4 = pBouton4;
	pMenu->pBouton5 = pBouton5;

	char MenuPath[50];
	sprintf(MenuPath, "Bouton\\%s.png", "Fond_Menu");
	pMenu->pTextureFond = UTIL_LoadTexture(MenuPath, NULL, NULL);
	if (pMenu->pTextureFond == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Impossible to load the sprite :%s\n", MenuPath);
	}
}


*/

/*!
*  \fn      void Menu_free(Menu *pMenu)
*  \brief   Function to free the menu
*
*  \param   *pMenu      the Menu structure
*
*  \return  void
*/
/*
void Menu_free(Menu *pMenu)
{
	// Ca ou Free(pMenu->TabBouton)     ???
	//Uint32 i = 0;
/*	for (i = 0; i < pMenu->NbBouton; i++)
	{
		Bouton_free(&(pMenu->TabBouton[i]));
	}
*/
	//Bouton_free(pMenu->pBouton1);
	//Bouton_free(pMenu->pBouton2);
	//Bouton_free(pMenu->pBouton3);
	//Bouton_free(pMenu->pBouton4);
	//Bouton_free(pMenu->pBouton5);
/*
	UTIL_FreeTexture(&(pMenu->pTextureFond));
	UTIL_Free(pMenu);
}*/




/*!
*  \fn      void Menu_Draw(SDL_Renderer * renderer, Menu *pMenu)
*  \brief   Function to display the Menu
*
*  \param   *renderer      the screen to display the Menu
*  \param   *pMenu	       the Menu structure
*
*  \return  void
*/
/*
void Menu_Draw(SDL_Renderer * renderer, Menu *pMenu)
{
	//SDL_RenderClear(renderer);
//	Uint32 i = 0;
	/* Affichage de l'image de fond du Menu */
/*SDL_RenderCopy(renderer, pMenu->pTextureFond, NULL, &(pMenu->RectFond));*/
	/* Affichage des boutons */
/*	for (i = 0; i < pMenu->NbBouton; i++)
	{
		Bouton_Draw(renderer, &(pMenu->TabBouton[i]));
	}
*//*
	Bouton_Draw(renderer, pMenu->pBouton1);
	Bouton_Draw(renderer, pMenu->pBouton2);
	Bouton_Draw(renderer, pMenu->pBouton3);
	Bouton_Draw(renderer, pMenu->pBouton4);
	Bouton_Draw(renderer, pMenu->pBouton5);
	return;
}
*/


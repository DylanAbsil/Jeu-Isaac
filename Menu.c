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



/*!
*  \fn     Uint32 	Menu_Principal(SDL_Renderer *pRenderer, SDL_Window *pWindow)
*  \brief  Function to handle the main menu
*
*  \param   pRenderer	a pointer to the renderer
*  \param   pWindow		a pointer to the window
*  \return  the index of the button selected
			1 : The game
			2 : The level editor
			3 : The credit
*/
Uint32 	Menu_Principal(SDL_Renderer *pRenderer, SDL_Window *pWindow)
{

	Kr_Input inEvent; // Structure pour la gestion des événements
	InitEvents(&inEvent);

	/* Menu principal */
	Uint32 iRetour = 0;
	// Image de fond
	SDL_Texture *pBackgroundMP = NULL;
	SDL_Rect	 rectMP = { 0, 0, KR_WIDTH_WINDOW, KR_HEIGHT_WINDOW };
	pBackgroundMP = UTIL_LoadTexture(pRenderer, "menu/Fond_Menu.png", NULL, NULL);

	//Bouton Jouer
	Bouton    *pBoutonJouer = NULL;
	TTF_Font  *pFontBoutonJouer = NULL;
	SDL_Color  couleurBoutonJouer = { 0, 0, 0 };
	SDL_Rect   rPositionBoutonJouer = { 65, 550, 300, 120 };
	pFontBoutonJouer = Kr_Text_OpenFont("cour", 25);
	TTF_SetFontStyle(pFontBoutonJouer, TTF_STYLE_BOLD);
	pBoutonJouer = Bouton_Init("Bouton_Jouer", pRenderer, "Bouton1_Active", "Bouton1_Desactive", "Bouton1_Selection");
	if (!pBoutonJouer)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant initialize pBoutonJouer!\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	Bouton_Load(pBoutonJouer, TRUE, pFontBoutonJouer, couleurBoutonJouer, rPositionBoutonJouer, "Chargement", "Jouer");

	//Bouton Editeur
	Bouton    *pBoutonEditeur = NULL;
	TTF_Font  *pFontBoutonEditeur = NULL;
	SDL_Color  couleurBoutonEditeur = { 0, 0, 0 };
	SDL_Rect   rPositionBoutonEditeur = { 490, 550, 300, 120 };
	pFontBoutonEditeur = Kr_Text_OpenFont("cour", 25);
	TTF_SetFontStyle(pFontBoutonEditeur, TTF_STYLE_BOLD);
	pBoutonEditeur = Bouton_Init("Bouton_Editeur", pRenderer, "Bouton1_Active", "Bouton1_Desactive", "Bouton1_Selection");
	if (!pBoutonEditeur)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant initialize pBoutonEditeur!\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	Bouton_Load(pBoutonEditeur, TRUE, pFontBoutonEditeur, couleurBoutonEditeur, rPositionBoutonEditeur, "Chargement", "Level Editor");

	//Bouton Crédit
	Bouton    *pBoutonCredit = NULL;
	TTF_Font  *pFontBoutonCredit = NULL;
	SDL_Color  couleurBoutonCredit = { 0, 0, 0 };
	SDL_Rect   rPositionBoutonCredit = { 915, 550, 300, 120 };
	pFontBoutonCredit = Kr_Text_OpenFont("cour", 25);
	TTF_SetFontStyle(pFontBoutonCredit, TTF_STYLE_BOLD);
	pBoutonCredit = Bouton_Init("Bouton_Credit", pRenderer, "Bouton1_Active", "Bouton1_Desactive", "Bouton1_Selection");
	if (!pBoutonCredit)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant initialize pBoutonCredit!\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	Bouton_Load(pBoutonCredit, TRUE, pFontBoutonCredit, couleurBoutonCredit, rPositionBoutonCredit, "Chargement", "Crédit");

	//Music menu principal
	Kr_Music *pMusicMP = NULL;
	Boolean bMscPaused = FALSE;
	pMusicMP = Kr_Sound_InitMusic();
	if (!Kr_Sound_LoadMusic(pMusicMP, "title"))
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't load the mucis pMusicMP");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	Mix_VolumeMusic(25);

	//FPS
	/* Préparation de la gestion des FPS */
	SDL_Texture *pTextureFPS = NULL;
	TTF_Font	*pFontFPS = NULL;
	Kr_Fps		*pFPS = NULL;
	SDL_Color    colorFPS = { 0, 10, 220 };
	SDL_Rect	 rectPositionFPS;
	Uint32       iPreviousTime = 0, iCurrentTime = 0;

	rectPositionFPS.x = 1150;
	rectPositionFPS.y = 685;
	pFontFPS = Kr_Text_OpenFont("cour", 18);
	TTF_SetFontStyle(pFontFPS, TTF_STYLE_ITALIC);

	pFPS = Kr_Fps_Init(pRenderer, pFontFPS, &rectPositionFPS, colorFPS, TRUE);
	if (pFPS == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't initialize the FPS structure\n");
		exit(EXIT_FAILURE);
	}


	Mix_PlayMusic(pMusicMP->pMsc, -1);
	while (!inEvent.szKey[SDL_SCANCODE_ESCAPE] && !inEvent.bQuit && iRetour == 0)
	{
		/* Faire au début sinon crash*/
		UpdateEvents(&inEvent);

		/* Clique-Gauche */
		if (inEvent.szMouseButtons[0])
		{
			if (pBoutonJouer->bSurvole)
			{
				if (pBoutonJouer->bActive == TRUE) pBoutonJouer->bActive = FALSE;
				else
				{
					pBoutonJouer->bActive = TRUE;
					iRetour = 1;
				}
			}
			else if (pBoutonEditeur->bSurvole)
			{
				if (pBoutonEditeur->bActive == TRUE) pBoutonEditeur->bActive = FALSE;
				else
				{
					pBoutonEditeur->bActive = TRUE;
					iRetour = 2;
				}
			}
			else if (pBoutonCredit->bSurvole)
			{
				if (pBoutonCredit->bActive == TRUE) pBoutonCredit->bActive = FALSE;
				else
				{
					pBoutonCredit->bActive = TRUE;
					iRetour = 3;
				}
			}
			inEvent.szMouseButtons[0] = 0;
		}

		/* Pause la musique*/
		if (inEvent.szKey[SDL_SCANCODE_P])
		{
			if (bMscPaused) bMscPaused = FALSE;
			else bMscPaused = TRUE;
			inEvent.szKey[SDL_SCANCODE_P] = 0;
		}

		/* Menu et Interface */
		SDL_RenderClear(pRenderer);
		SDL_RenderCopy(pRenderer, pBackgroundMP, NULL, &rectMP);
		if (pBoutonJouer->bMustShow) Bouton_Draw(pBoutonJouer);
		if (pBoutonEditeur->bMustShow) Bouton_Draw(pBoutonEditeur);
		if (pBoutonCredit->bMustShow) Bouton_Draw(pBoutonCredit);
		Bouton_estSurvole(pBoutonJouer, inEvent);
		Bouton_estSurvole(pBoutonEditeur, inEvent);
		Bouton_estSurvole(pBoutonCredit, inEvent);
		Kr_FPS_Show(pFPS);
		SDL_RenderPresent(pRenderer); // Lorsque toutes les surfaces ont été placé on affiche le renderer (l'écran quoi...)

		Kr_Fps_Wait(pFPS, &iCurrentTime, &iPreviousTime, KR_FPS);
	}

	Bouton_Free(pBoutonJouer);
	Bouton_Free(pBoutonEditeur);
	Bouton_Free(pBoutonCredit);
	Kr_Text_CloseFont(&pFontFPS);		// Libération mémoire de la police
	Kr_Sound_FreeMusic(pMusicMP);
	Kr_Fps_Free(pFPS);
	return iRetour;
}

/*!
*  \fn     void Menu_Pause(SDL_Renderer *pRenderer);
*  \brief  Function to handle the main menu
*
*  \param   pRenderer	a pointer to the renderer
*  \param   pWindow		a pointer to the window
*  \return  none
*/
void Menu_Pause(SDL_Renderer *pRenderer)
{
	Kr_Input inEvent; // Structure pour la gestion des événements
	InitEvents(&inEvent);

	//FPS
	/* Préparation de la gestion des FPS */
	SDL_Texture *pTextureFPS = NULL;
	TTF_Font	*pFontFPS = NULL;
	Kr_Fps		*pFPS = NULL;
	SDL_Color    colorFPS = { 0, 10, 220 };
	SDL_Rect	 rectPositionFPS;
	Uint32       iPreviousTime = 0, iCurrentTime = 0;

	rectPositionFPS.x = 1150;
	rectPositionFPS.y = 685;
	pFontFPS = Kr_Text_OpenFont("cour", 18);
	TTF_SetFontStyle(pFontFPS, TTF_STYLE_ITALIC);

	pFPS = Kr_Fps_Init(pRenderer, pFontFPS, &rectPositionFPS, colorFPS, TRUE);
	if (pFPS == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't initialize the FPS structure\n");
		exit(EXIT_FAILURE);
	}

	//Bouton Editeur
	Bouton    *pBoutonPause = NULL;
	TTF_Font  *pFontBoutonPause = NULL;
	SDL_Color  couleurBoutonPause = { 0, 0, 0 };
	SDL_Rect   rPositionBoutonPause = { 490, 550, 300, 120 };
	pFontBoutonPause = Kr_Text_OpenFont("cour", 19);
	TTF_SetFontStyle(pFontBoutonPause, TTF_STYLE_BOLD);
	pBoutonPause = Bouton_Init("Bouton_Pause", pRenderer, "Bouton1_Active", "Bouton1_Desactive", "Bouton1_Selection");
	if (!pBoutonPause)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant initialize pBoutonEditeur!\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	Bouton_Load(pBoutonPause, TRUE, pFontBoutonPause, couleurBoutonPause, rPositionBoutonPause, "Chargement", "P pour reprendre");
	Bouton_Draw(pBoutonPause);
	SDL_RenderPresent(pRenderer);
	while (!inEvent.szKey[SDL_SCANCODE_P])
	{
		UpdateEvents(&inEvent);
		Kr_Fps_Wait(pFPS, &iCurrentTime, &iPreviousTime, KR_FPS);
	}
	Bouton_Free(pBoutonPause);
	Kr_Text_CloseFont(&pFontFPS);
	Kr_Fps_Free(pFPS);
}
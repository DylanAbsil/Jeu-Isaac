/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 06/04/2015 | Main pour le jeu Isaac		                 */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#pragma comment( lib, "SDL2.lib" ) 
#pragma comment( lib, "SDL2main.lib" )
#pragma comment( lib, "SDL2_mixer.lib" )
#pragma comment( lib, "SDL2_ttf.lib" )
#pragma comment( lib, "SDL2_image.lib" )

#include "kr_common.h"
#include "kr_log.h"
#include "kr_util.h"
#include "kr_input.h"
#include "kr_text.h"
#include "kr_level.h"
#include "kr_map.h"
#include "kr_config.h"
#include "kr_sound.h"
#include "kr_fps.h"
#include "entite.h"
#include "kr_sprite.h"
#include "HUD.h"
#include "Menu.h"
#include "level_editor.h"
#include "level_state.h"
#include "weapon.h"
#include "nature.h"
#include "message.h"
#include "bombe.h"
 

Uint32 Isaac(SDL_Renderer *pRenderer, SDL_Window *pWindow, Boolean bLoadBackup);
int main(int argc, char** argv)
{
	Uint32 iRetourMP = 0;
	SDL_Window *pWindow = NULL;

	Kr_Log_Init(KR_LOG_INFO); // Mise en place d'un fichier de log


	if (!Kr_Init())
	{
		exit(EXIT_FAILURE);
	}
	/* Création de la fenêtre */
	pWindow = SDL_CreateWindow("World of Isaac", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, KR_WIDTH_WINDOW, KR_HEIGHT_WINDOW, SDL_WINDOW_SHOWN); // SDL_WINDOW_FULLSCREEN
	if (pWindow == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't create the Window : %s\n", SDL_GetError());
		SDL_Quit();
	}
	SDL_Surface *pscreenSurface = SDL_GetWindowSurface(pWindow);
	if (pscreenSurface == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't create the Surface Window : %s\n", SDL_GetError());
		SDL_Quit();
	}
	/* Création du renderer */
	SDL_Renderer *pRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	if (pRenderer == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't create the Renderer", SDL_GetError());
		SDL_Quit();
	}
	SDL_Surface *pSurfaceCursor = IMG_Load("menu/cursor.png");
	SDL_Cursor  *pCursor = NULL;
	if (pSurfaceCursor)
	{
		pCursor = SDL_CreateColorCursor(pSurfaceCursor, 0, 0);
		SDL_SetCursor(pCursor);
	}

	do
	{
		iRetourMP = Menu_Principal(pRenderer,pWindow);
		if (iRetourMP == 1) Isaac(pRenderer,pWindow,FALSE);
		if (iRetourMP == 2) Editor(pRenderer, pWindow);
		if (iRetourMP == 3) Isaac(pRenderer, pWindow, TRUE);
	} while (iRetourMP != 0);

	if(pSurfaceCursor) UTIL_FreeTexture(pSurfaceCursor);
	if(pCursor) SDL_FreeCursor(pCursor);

	SDL_DestroyRenderer(pRenderer);	    // Libération mémoire du renderer
	SDL_DestroyWindow(pWindow);			// Libération mémoire de la fenetre
	Mix_CloseAudio();	// On quitte SDL_MIXER
	TTF_Quit();			// On quitte SDL_TTF
	SDL_Quit();			// On quitte SDL
	Kr_Log_Quit();		// On ferme les logs

	return EXIT_SUCCESS;
}






/*!
*  \fn     int Isaac(SDL_Renderer *pRenderer, SDL_Window *pWindow, Boolean bLoadBackup)
*  \brief  Function to start the game
*
*  \param   pRenderer	a pointer to the renderer
*  \param   pWindow		a pointer to the window
*  \param   bLoadBackup TRUE to replay and load the backup
*  \return EXIT_SUCCESS if everything is ok, EXIT_FAILURE otherwise
*/
Uint32 Isaac(SDL_Renderer *pRenderer, SDL_Window *pWindow, Boolean bLoadBackup)
{
	/* ========================================================================= */
	/*                           CONFIGURATION GENERALE                          */
	/* ========================================================================= */

	Kr_Input inEvent; // Structure pour la gestion des événements
	InitEvents(&inEvent);

	/* Copie des fichiers maps au besoin */
	if (Kr_Map_CopyLevelFiles(bLoadBackup) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't copy all the levels from /maps/backup");
		SDL_Quit();
	}
	/* ========================================================================= */
	/*                                  CHARGEMENT                               */
	/* ========================================================================= */

	/* ========================================================================= */
	/*                                  PLAYER                                   */
	/* ========================================================================= */

	/* Préparation d'images que l'on souhaitera afficher */
	Kr_Sprite	 *pSpritePlayer = NULL;
	Entity		 *pPlayer = NULL;
	SDL_Rect     *pRectPositionPlayer = (SDL_Rect*)malloc(sizeof(SDL_Rect));

	pRectPositionPlayer->x = 250;
	pRectPositionPlayer->y = 250;
	pRectPositionPlayer->w = 32; //Il est nécessaire de fournir la taille de l'image avec .w et .h sinon rien n'apparaitra
	pRectPositionPlayer->h = 32;

	/* Chargement des sprites */
	pSpritePlayer = Kr_Sprite_Init("zelda");			//D'abord création et load du sprite (ici le nom du sprite est "sprites/zelda_sud.png"
	if (Kr_Sprite_Load(pSpritePlayer, sud, 26, 136, 8, pRectPositionPlayer, pRenderer) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite !\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	/* Chargement du personnage principale */
	pPlayer = Entity_Init("zelda");				//Ensuite création et load du sprite (il faut préciser la taille de l'image png)
	if (Entity_Load(pPlayer, 100, 50, MOVESPEED, normal, FALSE, pSpritePlayer) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the entity !\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	/*Chargement de l'arme */
	Weapon *pistoletLumiere = Weapon_Init("pistolet lumiere");

	Weapon_Load(pistoletLumiere, "bullet", 100, 50, 500);
	changeWeapon(pPlayer, pistoletLumiere);

	/* Bombe */
	Bombe *pBombe = NULL;
	Boolean bBombSet = FALSE; // Vrai quand la bombe est posée
	Boolean bBombDraw = FALSE; // Vrai quand il faut afficher la bombe au sol en train de se consumer
	Boolean bStartExplosion = FALSE; // Vrai quand la bombe est dans sa phase d'explosion
	Boolean bCheckBomb = FALSE; // Vrai lorsque la bombe a fini d'explosé et qu'il faut aller vérifier si une entité était dans l'explosion
	char szNbBomb[10] = "0";
	pBombe = Bombe_Init(pRenderer, 2, 20, "Bombe_sol_explosion", "Bombe_sol", "bomb_set", "bomb_explosion");
	if (!pBombe)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't initialize the Bomb structure \n");
	}



	/* ========================================================================= */
	/*                                 MAP/LEVEL                                 */
	/* ========================================================================= */

	/* Chargement de la map */
	Kr_Map *pMap = NULL;

	pMap = Kr_Map_Init("world");
	if (!pMap)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't Load the map\n");
		exit(EXIT_FAILURE);
	}

	/* Chargement du niveau */
	Boolean   bChangeLevel = TRUE;
	Kr_Level *pCurrentLevel = NULL;
	Uint32    iCurrentLevelNumber = 1;
	Level_State *pCurrentLevelState = NULL;
	pCurrentLevelState = Level_State_Init(pPlayer);
	
	/* Musique */
	 // Pas encore géré la continuité de la musique
	Boolean bMscPaused = FALSE;
	Mix_VolumeMusic(5);		// Réglage du volume de la musique (0 à 128) innefficace ?



	/* ========================================================================= */
	/*                                   NATURE                                  */
	/* ========================================================================= */

	/* Oiseau */
	Sint32 movex = 0, movey = 0;
	Boolean bCheckTypeOiseau = FALSE, bActiverCalculOiseau = FALSE, bDrawOiseau = FALSE;
	Entity *pOiseau1 = NULL;
	Entity *pOiseau2 = NULL;
	Kr_Sound *pSndOiseau1 = NULL;
	Kr_Sound *pSndOiseau2 = NULL;
	Uint32 iTypeOiseau = 1;
	pOiseau1 = ChargementOiseau(pRenderer, 1);
	pOiseau2 = ChargementOiseau(pRenderer, 2);
	pSndOiseau1 = Kr_Sound_Alloc("seagull");
	pSndOiseau2 = Kr_Sound_Alloc("eagle");

	/* Buisson */
	Boolean bDrawBuisson = FALSE;
	Entity *pBuisson1 = NULL;
	Entity *pBuisson2 = NULL;
	Kr_Sound *pSndBuisson = NULL;
	Uint32 iPositionBuissonX = 0, iPositionBuissonY = 0, iValeurBuisson = 0;
	pSndBuisson = Kr_Sound_Alloc("buisson");
	pBuisson1 = ChargementBuisson(pRenderer, 1);
	pBuisson2 = ChargementBuisson(pRenderer, 2);
	
	/* Papillon */
	Boolean bCheckPapillon = FALSE, bDrawPapillon = FALSE;
	Uint32 iNumberPapillon = 0;
	Entity *pPapillon = NULL;
	Kr_Sound *pSndPapillon = NULL;
	pPapillon = ChargementPapillon(pRenderer);
	pSndPapillon = Kr_Sound_Alloc("butterfly");

	/* Pigeon effrayé */
	Entity *pPigeonVol = NULL;
	Boolean bFearedPigeon = FALSE, bDrawPigeonVol = FALSE, bOldPigeon = FALSE;
	Uint32  iIndexEntity = 0, iCoordXDebut = 0, iCoordYDebut = 0, iCoordXFin = 0, iCoordYFin = 0;
	Kr_Sound *pSndPigeon = NULL;
	pSndPigeon = Kr_Sound_Alloc("pigeon");
	pPigeonVol = ChargementPigeonVolant(pRenderer);

	/* Evenement périodique */
	Uint32 iPeriodicEvent = 0;
	Uint32 iInterractionLevel = 0;

	/* ========================================================================= */
	/*                                  MESSAGE                                  */
	/* ========================================================================= */
	Message   *pMessageLevel = NULL;
	SDL_Color  colorMessageLevel = { 50, 10, 130 };
	TTF_Font *pFontMessageLevel = NULL;

	pFontMessageLevel = Kr_Text_OpenFont("cour", 25);
	TTF_SetFontStyle(pFontMessageLevel, TTF_STYLE_ITALIC);
	pMessageLevel = Message_Init("message_level", pRenderer);
	if (!Message_Load(pMessageLevel, "bandeau_message", 5, colorMessageLevel, pFontMessageLevel))
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't Load pMessageLevel!\n");
	}
	Message_Update(pMessageLevel, FALSE, "Initialisation pMessageLevel");


	Message   *pMessageInfo = NULL;
	char szMessageInfo[250] = " ";
	SDL_Color  colorMessageInfo = { 30, 22, 250 };
	TTF_Font *pFontMessageInfo = NULL;

	pFontMessageInfo = Kr_Text_OpenFont("cour", 18);
	TTF_SetFontStyle(pFontMessageInfo, TTF_STYLE_BOLD);
	pMessageInfo = Message_Init("message_info", pRenderer);
	if (!Message_Load(pMessageInfo, "bandeau_info", 1, colorMessageInfo, pFontMessageInfo))
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't Load pMessageInfo!\n");
	}
	Message_Update(pMessageInfo, FALSE, "Initialisation pMessageInfo");

	/* ========================================================================= */
	/*                                  HUD                                      */
	/* ========================================================================= */
	TTF_Font *policeHUD = NULL;
	SDL_Color CouleurHUD = { 255, 255, 255 }; 

	/* Ouverture de la police */
	policeHUD = Kr_Text_OpenFont("upheavtt", 26);
	TTF_SetFontStyle(policeHUD, TTF_STYLE_BOLD);

	/* Initialisation des structures HUD */
	HUD *hVie = NULL; //HUD vie
	HUD *hBombeImage = NULL; //HUD Munitions (l'image seulement)
	HUD *hBombeTexte = NULL; //HUD Munitions (le nombre de munition)
	HUD *hCleImage = NULL; //HUD Bombe (l'image seulement)
	HUD *hCleTexte = NULL; //HUD Bombe (le nombre de bombes)

	/* Initialisation des positions des HUD */
	// Pour mettre texte et images à la même hauteru (pour les HUD Bombe et Cle) :
	// écart en x : les parties textes doivent être décalées de la largeur de l'image + l'espacement HUD_ESPACEMENT
	// écart en y : normalement 0 mais comme la police est plus grande -> décaler pour avoir le texte 5 pixels plus haut
	SDL_Rect RectVie = { 1050, 22, 16, 16 };
	SDL_Rect RectBombeImage = { 50, 35, 18, 18 };
	SDL_Rect RectBombeTexte = { 70, 30, 16, 16 };
	SDL_Rect RectCleImage = { 50, 12, 18, 18 };
	SDL_Rect RectCleTexte = { 70, 7, 16, 16 };

	/* Initialisation des textures pour les HUD "texte" */
	SDL_Texture *CleTexteTexture = NULL; //Texture pour le texte
	SDL_Texture *BombeTexteTexture = NULL; //Texture pour le texte

	/* Initialisation des différents HUD */
	/* Initialisation HUD Vie */
	hVie = HUD_Init("SDL_heat", FALSE, pRenderer);
	HUD_Load(hVie, RectVie);
	/* InitialisationHUD BombeImage */
	hBombeImage = HUD_Init("SDL_Bomb", FALSE, pRenderer);
	HUD_Load(hBombeImage, RectBombeImage);
	/* InitialisationHUD CleImage */
	hCleImage = HUD_Init("SDL_Key", FALSE, pRenderer);
	HUD_Load(hCleImage, RectCleImage);
	/* InitialisationHUD BombeTexte */
	hBombeTexte = HUD_Init("BombeImage", TRUE, pRenderer);
	HUD_Load(hBombeTexte, RectBombeTexte);
	/* InitialisationHUD CleTexte */
	hCleTexte = HUD_Init("CleImage", TRUE, pRenderer);
	HUD_Load(hCleTexte, RectCleTexte);

	/* ========================================================================= */
	/*                                    FPS                                    */
	/* ========================================================================= */
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

	/* ========================================================================= */
	/*                                  DIVERS                                   */
	/* ========================================================================= */

	/* Sound */
	Mix_AllocateChannels(10);
	Kr_Sound *pSndCoffre = Kr_Sound_Alloc("ouverture_coffre");


	/* Gestion des entitées */
	Uint32 iCodeUpdateEntity = 1;

	/* Préparation d'une Texture contenant un message via util.c*/
	SDL_Rect     textPosition;
	SDL_Color    couleur = { 123, 255, 0 };
	SDL_Texture *pTextureText = NULL;
	TTF_Font    *pFont = NULL;
	char         szDebug[100] = " ";

	textPosition.x = 0;
	textPosition.y = 0;
	pFont = Kr_Text_OpenFont("cour", 25);
	TTF_SetFontStyle(pFont, TTF_STYLE_BOLD);

	/* Mort du player */
	Boolean bPlayerDead = FALSE;

	/* Nombre de buisson détruit par la dernière bombe */
	Uint32 iNumberBushDestroy = 0, iRecompense = 0;
	Boolean bRandRecompense = FALSE;

	/* ========================================================================= */
	/*                            BOUCLE PRINCIPALE                              */
	/* ========================================================================= */
	while (!inEvent.szKey[SDL_SCANCODE_ESCAPE] && !inEvent.bQuit && !bPlayerDead)
	{
		/* Faire au début */
		if (bChangeLevel == TRUE)
		{
			bChangeLevel = FALSE;
			if (!Level_State_SaveLevel(pCurrentLevelState)) Kr_Log_Print(KR_LOG_WARNING, "Can't save the current data of the current level\n");
			pCurrentLevel = Kr_Level_Change(pCurrentLevel, iCurrentLevelNumber, pRenderer);
			Level_State_Free(pCurrentLevelState,FALSE);
			pCurrentLevelState = Level_State_Init(pPlayer);
			Level_State_Load(pCurrentLevelState, pCurrentLevel, pRenderer);
			Mix_PlayMusic(pCurrentLevel->pMusic->pMsc, -1);
			bCheckTypeOiseau = TRUE;
			bFearedPigeon = FALSE;
			bCheckPapillon = TRUE;
			bDrawPapillon = FALSE;
			PassageOiseau(pOiseau1, bActiverCalculOiseau, movex, movey, pRenderer, pSndOiseau1, TRUE); // On relance un passage pour indiquer qu'on veut stopper le passage
			PigeonVol(pPigeonVol, bFearedPigeon, pRenderer, pCurrentLevel, pSndPigeon, iCoordXDebut, iCoordYDebut, &iCoordXFin, &iCoordYFin);
			pBombe->bBombExplosion = FALSE;
			pBombe->bBombSet = FALSE;
			bBombDraw = FALSE;
			bBombSet = FALSE;
			bStartExplosion = FALSE;
		}

		UpdateEvents(&inEvent);
		
		/* Mise à jour des entitées (dont le player)*/
		iCodeUpdateEntity = updateAllEntities(pRenderer, pCurrentLevelState, inEvent);
		if (iCodeUpdateEntity == FALSE)
		{
			Kr_Log_Print(KR_LOG_ERROR, "Couldn't update an entity\n");
			SDL_Quit();
			return FALSE;
		}

		if (bCheckBomb == TRUE) // Lorsqu'une bombe a explosé 
		{
			bCheckBomb = FALSE; 
			iNumberBushDestroy = Level_State_Bomb_Detect(pCurrentLevelState, pBombe);
			if (iNumberBushDestroy > 0)	bRandRecompense = TRUE;

		}
		/* Controle du tir du personnage */
		shoot(inEvent, pPlayer, pRenderer);

		/* Mise à jour des projectiles du personnage */
		updateProjectilesWeapon(pRenderer, pCurrentLevelState, pCurrentLevelState->pPlayer->pWeapon);
		
		/* ========================================================================= */
		/*                           GESTIONS DES TOUCHES                            */
		/* ========================================================================= */

		if (inEvent.szMouseButtons[0]) // Clique Gauche
		{
			pPlayer->pSprEntity->pRectPosition->x = inEvent.iMouseX;
			pPlayer->pSprEntity->pRectPosition->y = inEvent.iMouseY;
			inEvent.szMouseButtons[0] = 0;
			
		}
		if (inEvent.szMouseButtons[2]) // Clique droit
		{
			inEvent.szMouseButtons[2] = 0; // Un seul clique
		}
		if (inEvent.szKey[SDL_SCANCODE_F]) // Afficher les FPS
		{
			if (pFPS->bMustShow) pFPS->bMustShow = FALSE;
			else pFPS->bMustShow = TRUE;
			inEvent.szKey[SDL_SCANCODE_F] = 0;
		}
		if (inEvent.szKey[SDL_SCANCODE_O]) // Mettre en pause la music
		{
			if (bMscPaused) bMscPaused = FALSE;
			else bMscPaused = TRUE;
			inEvent.szKey[SDL_SCANCODE_O] = 0;
		}
		if (inEvent.szKey[SDL_SCANCODE_P]) // Mettre en pause le jeu
		{
			Menu_Pause(pRenderer, "Appuyer sur P pour reprendre");
			inEvent.szKey[SDL_SCANCODE_P] = 0;
		}
		if (inEvent.szKey[SDL_SCANCODE_E]) // Interragir avec le niveau (panneau, coffre)
		{
			iInterractionLevel = Kr_Level_Interraction(pCurrentLevel, pPlayer);
			if (iInterractionLevel == 2) // Lecture d'un panneau
			{
				Message_Update(pMessageLevel, TRUE, pCurrentLevel->szLevelMessage);
			}
			if (iInterractionLevel == 1) // Ouverture d'un coffre
			{
				iRecompense = Level_State_Recompense(pCurrentLevelState, TRUE, FALSE, 0);
				Kr_Sound_Play(pSndCoffre, 0, 100, 0);				
			}
			inEvent.szKey[SDL_SCANCODE_E] = 0;
		}
		if (inEvent.szKey[SDL_SCANCODE_TAB]) // Affichage des informations sur le niveau
		{
			sprintf(szMessageInfo, "Level %d : %s",pCurrentLevel->iLevelNum, pCurrentLevel->szLevelName);
			Message_Update(pMessageInfo, TRUE, szMessageInfo);
		}
		if (inEvent.szKey[SDL_SCANCODE_B]) // Poser une bombe
		{
			if (bStartExplosion == FALSE) bBombSet = TRUE; // On ne peut poser de bombe que si l'explosion précédente a eu lieu
			inEvent.szKey[SDL_SCANCODE_B] = 0;
		}

		/* ========================================================================= */
		/*                              FPS & EVENEMENT                              */
		/* ========================================================================= */

		iCurrentLevelNumber = Kr_Map_ShouldChangeLevel(pMap, pCurrentLevel, pPlayer);
		if (iCurrentLevelNumber)
		{
			bChangeLevel = TRUE;
		}

		/* Gestion de la musique des niveaux*/
		if (bMscPaused)	Mix_PauseMusic();
		else Mix_ResumeMusic();

		Kr_Fps_Wait(pFPS, &iCurrentTime, &iPreviousTime, KR_FPS);
		iPeriodicEvent = PeriodicEvent();
		switch (iPeriodicEvent)
		{
			case 0: // Aucun événement
				bActiverCalculOiseau = FALSE;
				break;
			case 1:
				if (bDrawOiseau == FALSE) bActiverCalculOiseau = TRUE;
				break;
			default :
				bActiverCalculOiseau = FALSE;
				break;
		}

		/* ========================================================================= */
		/*                                   NATURE                                  */
		/* ========================================================================= */

		/* Gestion des oiseaux*/
		if (bCheckTypeOiseau == TRUE)
		{
			iTypeOiseau = CalculTypeOiseau(bCheckTypeOiseau, pCurrentLevel);
			bCheckTypeOiseau = FALSE;
		}		
		if (iTypeOiseau == 1)
		{
			CalculPassageOiseau(pOiseau1, bActiverCalculOiseau, &movex, &movey);
			bDrawOiseau = PassageOiseau(pOiseau1, bActiverCalculOiseau, movex, movey, pRenderer, pSndOiseau1, FALSE);
		}
		if (iTypeOiseau == 2)
		{
			CalculPassageOiseau(pOiseau2, bActiverCalculOiseau, &movex, &movey);
			bDrawOiseau = PassageOiseau(pOiseau2, bActiverCalculOiseau, movex, movey, pRenderer, pSndOiseau2, FALSE);
		}

		/* Gestion des buissons*/
		iValeurBuisson = DetectPassageBuisson(pCurrentLevel, pPlayer, &iPositionBuissonX, &iPositionBuissonY);
		if (iValeurBuisson == 1) bDrawBuisson = AnimationBuisson(pBuisson1, TRUE, iPositionBuissonX, iPositionBuissonY, pRenderer,pSndBuisson);
		if (iValeurBuisson == 2) bDrawBuisson = AnimationBuisson(pBuisson2, TRUE, iPositionBuissonX, iPositionBuissonY, pRenderer,pSndBuisson);

		/* Gestion des papillons*/
		if (bCheckPapillon == TRUE)
		{
			CalculApparitionPapillon(bCheckPapillon, pCurrentLevel, pPapillon, &iNumberPapillon);
			if (iNumberPapillon > 0) bDrawPapillon = TRUE;
			bCheckPapillon = FALSE;
		}
		UpdateButterfly(pPapillon, bDrawPapillon, pRenderer, pCurrentLevel, pSndPapillon);

		/* Gestion des pigeons */
		if (iCodeUpdateEntity == 2) // Oiseau effrayé
		{
			setOnFirstEnt(pCurrentLevelState->plEnt);
			while (pCurrentLevelState->plEnt->current != NULL)
			{
				if ((bDrawPigeonVol == FALSE) && (strcmp(pCurrentLevelState->plEnt->current->e->strEntityName, "pigeon1") == 0))
				{
					pCurrentLevelState->plEnt->current->e->state = invisible;
					bFearedPigeon = TRUE;
					iCoordXDebut = pCurrentLevelState->plEnt->current->e->pSprEntity->pRectPosition->x;
					iCoordYDebut = pCurrentLevelState->plEnt->current->e->pSprEntity->pRectPosition->y;
				}
				nextEnt(pCurrentLevelState->plEnt);
			}				
		}
		else bFearedPigeon = FALSE;

		bOldPigeon = bDrawPigeonVol;
		bDrawPigeonVol = PigeonVol(pPigeonVol, bFearedPigeon, pRenderer, pCurrentLevel, pSndPigeon, iCoordXDebut, iCoordYDebut, &iCoordXFin, &iCoordYFin);
		if ((bDrawPigeonVol == FALSE) && (bOldPigeon == TRUE)) // Atterissage de l'oiseau
		{
			setOnFirstEnt(pCurrentLevelState->plEnt);
			while (pCurrentLevelState->plEnt->current != NULL)
			{
				if (strcmp(pCurrentLevelState->plEnt->current->e->strEntityName, "pigeon1") == 0)
				{
					pCurrentLevelState->plEnt->current->e->state = normal;
					bFearedPigeon = TRUE;
					pCurrentLevelState->plEnt->current->e->pSprEntity->pRectPosition->x = iCoordXFin;
					pCurrentLevelState->plEnt->current->e->pSprEntity->pRectPosition->y = iCoordYFin;
				}
				nextEnt(pCurrentLevelState->plEnt);
			}
		}

		/* ========================================================================= */
		/*                                RECOMPENSE                                 */
		/* ========================================================================= */

		/* Gestion des récompenses*/
		if (bRandRecompense) // Récompense pour la destruction d'un buisson coloré, on vérifie qu'un coffre n'a pas été ouvert auparavant
		{
			iRecompense = Level_State_Recompense(pCurrentLevelState, bRandRecompense, TRUE, iNumberBushDestroy);
			bRandRecompense = FALSE;
		}
		if (iRecompense != 0) // Attribution des récompenses
		{
			if (iRecompense == 1)
			{
				Message_Update(pMessageLevel, TRUE, "Vous trouvez 5 bombes et 50hp !");
				pBombe->iNumber += 5;
				pPlayer->iEntityLife += 50;
				iRecompense = 0;
			}
			else if (iRecompense == 2)
			{
				Message_Update(pMessageLevel, TRUE, "Vous trouvez 50hp et 50ap !");
				pPlayer->iArmor += 50;
				pPlayer->iEntityLife += 50;
				iRecompense = 0;
			}
			else if (iRecompense == 3)
			{
				Message_Update(pMessageLevel, TRUE, "Vous trouvez 2 bombes et 20hp !");
				pBombe->iNumber += 2;
				pPlayer->iEntityLife += 20;
				iRecompense = 0;
			}
			else if (iRecompense == 4)
			{
				Message_Update(pMessageLevel, TRUE, "Vous trouvez 2 bombes !");
				pBombe->iNumber += 2;
				iRecompense = 0;
			}
		}


		/* ========================================================================= */
		/*                                  DIVERS                                   */
		/* ========================================================================= */
		/* Création des textures à partir du texte pour le nombre de munition ou de clés*/
		sprintf(szNbBomb, "%d", pBombe->iNumber);
		CleTexteTexture = Kr_Text_FontCreateTexture(pRenderer, policeHUD, "4", CouleurHUD, TRUE, &(hCleTexte->RectDest));
		BombeTexteTexture = Kr_Text_FontCreateTexture(pRenderer, policeHUD, szNbBomb, CouleurHUD, TRUE, &(hBombeTexte->RectDest));

		/* Mise à jour des HUD "texte" avec la nouvelle texture générée */
		HUD_Update(hCleTexte, CleTexteTexture);
		HUD_Update(hBombeTexte, BombeTexteTexture);

		/* Gestion des bombes */
		bBombDraw = Bombe_Set(pBombe, bBombSet, pPlayer->pSprEntity->pRectPosition->x, pPlayer->pSprEntity->pRectPosition->y, &bCheckBomb);
		if (bBombDraw == FALSE && bBombSet == TRUE) bStartExplosion = TRUE; // Cas d'une explosion
		else bStartExplosion = FALSE;
		bBombSet = bBombDraw;

		/* Mort du player*/
		if (pPlayer->iEntityLife <= 0)
		{
			bPlayerDead = TRUE;
			Message_Update(pMessageInfo, TRUE, "Vous êtes mort, la partie s'achève !");
		}

		/* Texte pour le debug */
		pTextureText = Kr_Text_FontCreateTexture(pRenderer, pFont, szDebug, couleur, TRUE, &textPosition); // Création d'une texture contenant le texte d'une certaine couleur avec le mode Blended  
		sprintf(szDebug, "Cursor : X : %d Y : %d   %s %d | Tile %d %d", pPlayer->pSprEntity->pRectPosition->x, pPlayer->pSprEntity->pRectPosition->y, pCurrentLevel->szLevelName, pCurrentLevel->iLevelNum, Kr_Level_GetTile(pCurrentLevel, inEvent.iMouseX, inEvent.iMouseY),iNumberPapillon);//)pMonLevel->rScrollWindow->x, pMonLevel->rScrollWindow->y // Affichage coordonnée de la map

		/* ========================================================================= */
		/*                                AFFICHAGE                                  */
		/* ========================================================================= */
		
		SDL_RenderClear(pRenderer); // Dans un premier temps on clear le renderer

		Kr_Level_Draw(pRenderer, pCurrentLevel); //Affichage du niveau
		if ((bDrawBuisson == TRUE) && (iValeurBuisson == 1)) Entity_Draw(pRenderer, pBuisson1); // Affichage de l'animation des buissons
		if ((bDrawBuisson == TRUE) && (iValeurBuisson == 2)) Entity_Draw(pRenderer, pBuisson2);// Affichage de l'animation des buissons
		if (bBombDraw) Entity_Draw(pRenderer, pBombe->pEntBomb);// Affichage de la bombe au sol
		if (bStartExplosion || pBombe->bBombExplosion)
		{
			bStartExplosion = Bombe_Explosion(pBombe, TRUE, pRenderer); // Affichage de l'explosion de la bombe
		}

		drawAllEntities(pCurrentLevelState, pRenderer); 
		drawProjectilesWeapon(pPlayer->pWeapon->plProjectile, pRenderer);
		if (bDrawPapillon == TRUE) Entity_Draw(pRenderer, pPapillon);
		if ((bDrawPigeonVol == TRUE)) Entity_Draw(pRenderer, pPigeonVol);
		if ((bDrawOiseau == TRUE) && (iTypeOiseau == 1)) Entity_Draw(pRenderer, pOiseau1);
		if ((bDrawOiseau == TRUE) && (iTypeOiseau == 2)) Entity_Draw(pRenderer, pOiseau2);
		Message_Draw(pMessageLevel);
		Message_Draw(pMessageInfo);

		// HUD
		HUD_Draw(pRenderer, hVie, Entity_NumberHP(pPlayer)); 
		HUD_Draw(pRenderer, hBombeImage, 0);
		HUD_Draw(pRenderer, hBombeTexte, 0);
		HUD_Draw(pRenderer, hCleImage, 0);
		HUD_Draw(pRenderer, hCleTexte, 0);

		// Divers
		if (pMessageInfo->bMustShow == TRUE)  SDL_RenderCopy(pRenderer, pTextureText, NULL, &textPosition);
		Kr_FPS_Show(pFPS);
		SDL_RenderPresent(pRenderer); // Lorsque toutes les surfaces ont été placé on affiche le renderer (l'écran quoi...)

		// Libération des texture
		UTIL_FreeTexture(&pTextureText); // Comme on recréé la texture en permanence dans la boucle il faut la free également dans la boucle
		UTIL_FreeTexture(&CleTexteTexture); 
		UTIL_FreeTexture(&BombeTexteTexture);
		
		//Gestion de la mort du player
		if (bPlayerDead)
		{
			SDL_Delay(5000);
		}
	}

	/* ========================================================================= */
	/*                            LIBERATION MEMOIRE                             */
	/* ========================================================================= */

	UTIL_FreeTexture(&pTextureText);
	UTIL_FreeTexture(&CleTexteTexture);
	UTIL_FreeTexture(&BombeTexteTexture);
	Kr_Text_CloseFont(&pFont);
	Kr_Level_Free(pCurrentLevel);
	Message_Free(pMessageLevel);
	Message_Free(pMessageInfo);
	Entity_Free(pOiseau1);
	Entity_Free(pOiseau2);
	Entity_Free(pBuisson1);
	Entity_Free(pBuisson2);
	Entity_Free(pPapillon);
	Entity_Free(pPigeonVol);
	Kr_Sound_Free(&pSndOiseau1);
	Kr_Sound_Free(&pSndOiseau2);
	Kr_Sound_Free(&pSndBuisson);
	Kr_Sound_Free(&pSndPigeon);
	Kr_Sound_Free(&pSndPapillon);
	Kr_Sound_Free(&pSndCoffre);
	Kr_Fps_Free(pFPS);
	HUD_free(hVie);
	HUD_free(hCleImage);
	HUD_free(hBombeImage);
	HUD_free(hBombeTexte);
	HUD_free(hCleTexte);
	Bombe_Free(pBombe);
	Level_State_Free(pCurrentLevelState, TRUE);
	Kr_Map_Free(pMap);

	return EXIT_SUCCESS;
}




/* ========================================================================= */
/*!
* \file    nature.c
* \brief   Contains the function to handle the nature
* \author  Olivier Herrou
* \version 1.0
* \date    29 Avril 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 29/04/2015 | Creation, gestion des oiseaux                */
/*               | 01/05/2015 | Animation des buissons                       */
/*               | 02/05/2015 | Calcul du nombre de papillon                 */
/* Herrou        | 04/05/2015 | Renommer fichier en nature.h .c              */
/*               |            |                                              */
/* ========================================================================= */

#include "nature.h"

/*!
*  \fn     CalculPassageOiseau(Entity *pEntity, Boolean bLaunch, Sint32 *movex, Sint32 *movey)
*  \brief  Function to compute the movement of the entity
*
*  \param  pEntity Pointeur vers l'entité
*  \param  bLaunch lancer un calcul de vecteurs
*  \param  movex   movement vector on X
*  \param  movey   movement vector on X
*  \return none
*/
void CalculPassageOiseau(Entity *pEntity, Boolean bLaunch, Sint32 *movex, Sint32 *movey)
{
	Uint32 iDir = 0, iMaxSpeed = 1, iTmp = 0, xMin = 0, xMax = 0, yMin = 0, yMax = 0;

	if (!bLaunch) return;
	*movex = *movey = 0;
	//Choix de la direction vers laquelle l'oiseau se dirigera
	iDir = rand() % 4;
	// Préparation de la position initial de l'oiseau
	switch (iDir)
	{
	case 0: // nord
		xMin = 0;
		xMax = KR_WIDTH_WINDOW - pEntity->pSprEntity->pRectPosition->w;
		yMin = yMax = KR_HEIGHT_WINDOW - pEntity->pSprEntity->pRectPosition->h;
		break;
	case 1: // est
		xMin = xMax = KR_WIDTH_WINDOW - pEntity->pSprEntity->pRectPosition->w;
		yMin = 0;
		yMax = KR_HEIGHT_WINDOW - pEntity->pSprEntity->pRectPosition->h;
	case 2: // sud
		xMin = 0;
		xMax = KR_WIDTH_WINDOW - pEntity->pSprEntity->pRectPosition->w;
		yMin = yMax = 0;
		break;
	case 3: // ouest
		xMin = xMax = 0;
		yMin = 0;
		yMax = KR_HEIGHT_WINDOW - pEntity->pSprEntity->pRectPosition->h;
		break;
		break;
	default: return;
	}
	// Choix d'une position aléatoire du début
	Uint32 x = 0, y = 0;
	x = (rand() % (xMax - xMin + 1)) + xMin;
	y = (rand() % (yMax - yMin + 1)) + yMin;
	pEntity->pSprEntity->pRectPosition->x = x;
	pEntity->pSprEntity->pRectPosition->y = y;

	//Calcul des vecteurs 
	*movex = rand() % (iMaxSpeed + 1);
	*movey = rand() % (iMaxSpeed + 1);
	// Si le random génčre un vecteur nul sur x et y alors on doit quand męme le faire se déplacer de maničre cohérente
	if (iDir == 0 && *movey == 0) *movey = 1;
	if (iDir == 1 && *movex == 0) *movex = 1;
	if (iDir == 2 && *movey == 0) *movey = 1;
	if (iDir == 3 && *movex == 0) *movex = 1;
	if (*movex == 0 && *movey == 0) *movex = 1;


	//Calcul du nombre de répétition du déplacement avant que l'entité soit sortie de l'écran
	if (iDir == 0) *movey = *movey * -1; // Il doit descendre s'il commence au nord
	if (iDir == 1) *movex = *movex * -1; // s'il commence ŕ l'est
	//Kr_Log_Print(KR_LOG_INFO, "Début du passage avec %d %d\n", *movex, *movey);

}

/*!
*  \fn     Entity *ChargementOiseau(SDL_Renderer *pRenderer, Uint32 iOiseau)
*  \brief  Function to load the entities
*
*  \param  pRenderer a pointer to the renderer
*  \param  iOiseau the number of the entity to spawn
*  \return none
*/
Entity *ChargementOiseau(SDL_Renderer *pRenderer, Uint32 iOiseau)
{
	Entity *pEntity = NULL;
	char szName[40] = "";
	Kr_Sprite *pSprite = NULL;
	SDL_Rect *pRectMouette = NULL;
	pRectMouette = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (!pRectMouette) return  NULL;
	pRectMouette->x = 0;
	pRectMouette->y = 0;
	pRectMouette->w = 32;
	pRectMouette->h = 27;

	if (iOiseau == 1 || iOiseau == 2) sprintf(szName, "oiseau%d", iOiseau);
	else
	{
		Kr_Log_Print(KR_LOG_ERROR, "Invalid number for iOiseau\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	/* Chargement des sprites */
	pSprite = Kr_Sprite_Init(szName);
	if (Kr_Sprite_Load(pSprite, sud, 27, 128, 4, pRectMouette, pRenderer) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite %s !\n",szName);
		SDL_Quit();
		exit(EXIT_FAILURE);
	}


	/* Chargement des personnages */
	pEntity = Entity_Init(szName);
	if (Entity_Load(pEntity, 100, 0, MOVESPEED, noclip, TRUE, pSprite) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the entity !\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	return pEntity;
}


/*!
*  \fn     Uint32 CalculTypeOiseau(Boolean bCalculer, Kr_Level *pLevel)
*  \brief  Function to check which type of bird we must spawn according to the level

*  \param  bCalculer When this value is set to TRUE, the function will check the level
*  \param  pLevel    a pointer to the level
*  \param  iNumber	 
*  \return the number of the entity that the level should spawn
	       1 : Mouette
		   2 : Aigle
*/
Uint32 CalculTypeOiseau(Boolean bCalculer, Kr_Level *pLevel)
{
	Sint32 i = 0, j = 0, iValue = 0, iNumTile = 0;

	for (i = 0; i < pLevel->iLevel_TileWidth; i++)
	{
		for (j = 0; j < pLevel->iLevel_TileHeight; j++)
		{
			iNumTile = pLevel->szLayout[i][j];
			if (pLevel->pLevel_Tileset->pTilesProp[iNumTile].iWater)
			{
				iValue++;
			}
		}
	}
	
	// Calcul des ratios de tile "arbre" par rapport au nombre de tile totale du level
	if (iValue > ((pLevel->iLevel_TileWidth * pLevel->iLevel_TileHeight) / 10))
	{
		return 1;
	}
	return 2;
}

/*!
*  \fn     Boolean PassageOiseau(Entity *pEntity, Boolean bPassage, Sint32 movex, Sint32 movey, SDL_Renderer *pRenderer, Kr_Sound *pSndOiseau)
*  \brief  Function to compute the movement of the entity
*
*  \param  pEntity    a pointer to the entity
*  \param  bPassage   start the travelling of a bird
*  \param  movex      movement vector on X
*  \param  movey      movement vector on X
*  \param  pRenderer  a pointer to the renderer
*  \param  pSndOiseau a pointer to the sound
*  \param  bStop      TRUE to stop
*  \return TRUE if the entity is still on the screen, FALSE otherwise
*/
Boolean PassageOiseau(Entity *pEntity, Boolean bPassage, Sint32 movex, Sint32 movey, SDL_Renderer *pRenderer, Kr_Sound *pSndOiseau, Boolean bStop)
{
	static Boolean bPassageEnCours = FALSE;
	Direction newDir = sud;
	if (bStop)
	{
		bPassageEnCours = FALSE;
		return FALSE;
	}
	if (bPassage || bPassageEnCours) bPassageEnCours = TRUE;
	else return FALSE;
	if (bPassage) Kr_Sound_Play(pSndOiseau, KR_SOUND_ANIMAL_CANAL, 100, 0); // On le joue une seule fois
	newDir = foundDirection(movex, movey, pEntity);
	switchTextureFromDirection(pEntity, newDir, pRenderer);
	pEntity->mouvement = 1;
	pEntity->iTempoAnim += 1;
	if (pEntity->iTempoAnim == 20)	//Si la tempo est arrivée ŕ son terme :
	{
		pEntity->pSprEntity->iCurrentFrame += 1; //	- Frame suivante
		if (pEntity->pSprEntity->iCurrentFrame == pEntity->pSprEntity->iNbFrames) //Si l'animation est arrivée au bout 
		{
			pEntity->pSprEntity->iCurrentFrame = 0;
		}
		pEntity->iTempoAnim = 0;
	}

	pEntity->pSprEntity->pRectPosition->x += movex;
	pEntity->pSprEntity->pRectPosition->y += movey;


	if (pEntity->pSprEntity->pRectPosition->x < 0 || pEntity->pSprEntity->pRectPosition->x > KR_WIDTH_WINDOW ||
		pEntity->pSprEntity->pRectPosition->y < 0 || pEntity->pSprEntity->pRectPosition->y > KR_HEIGHT_WINDOW)
	{
		//Kr_Log_Print(KR_LOG_INFO, "Fin du passage\n");
		bPassageEnCours = FALSE;
		return FALSE;
	}
	return TRUE;
}



/*!
*  \fn     Entity ChargementBuisson(SDL_Renderer *pRenderer, Uint32 iBuisson)
*  \brief  Function to load the bush

*  \param pRenderer a pointer to to the renderer
*  \param iBuisson  the number of the bush to load (1 or 2)
*  \return the loaded entity
*/
Entity *ChargementBuisson(SDL_Renderer *pRenderer, Uint32 iBuisson)
{
	char szName[40] = "";
	Entity *pEntity = NULL;
	Kr_Sprite *pSprite1 = NULL;
	SDL_Rect *pRectBuisson1 = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (!pRectBuisson1) return NULL;
	pRectBuisson1->x = 0;
	pRectBuisson1->y = 0;
	pRectBuisson1->w = 32;
	pRectBuisson1->h = 32;

	if (iBuisson == 1 || iBuisson == 2) sprintf(szName, "buisson%d", iBuisson);
	else
	{
		Kr_Log_Print(KR_LOG_ERROR, "Invalid number for iBuisson\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	/* Sprites */
	pSprite1 = Kr_Sprite_Init(szName);
	if (Kr_Sprite_Load(pSprite1, unknown, 32, 128, 4, pRectBuisson1, pRenderer) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite buisson %d!\n",iBuisson);
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	/* Entité */
	pEntity = Entity_Init(szName);
	if (Entity_Load(pEntity, 100, 0, MOVESPEED, noclip, TRUE, pSprite1) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the entity !\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	return pEntity;
}





/*!
*  \fn     Boolean AnimationBuisson(Entity *pEntity, Boolean bPassage, Uint32 x, Uint32 y, SDL_Renderer *pRenderer, Kr_Sound *pSndBuisson)
*  \brief  Function to start the animation sequence of the bush
*
*  \param  pEntity   a pointer to the entity
*  \param  bPassage  When this boolean is TRUE, the animation sequence start
*  \param  x		 X coordinate of the bush
*  \param  y		 Y coordinate of the bush
*  \param  pRenderer a pointer to the renderer
*  \param  pSndBuisson a pointer to the  allocated sound
*  \return TRUE if the sequence is not finished, FALSE otherwise
*/
Boolean AnimationBuisson(Entity *pEntity, Boolean bPassage, Uint32 x, Uint32 y, SDL_Renderer *pRenderer, Kr_Sound *pSndBuisson)
{
	
	static Boolean bAnimationEnCours = FALSE;
	if (bPassage && !bAnimationEnCours) Kr_Sound_Play(pSndBuisson, KR_SOUND_NATURE_CANAL, 25, 0); // On le joue une seule fois
	Direction newDir = sud;
	if (bPassage || bAnimationEnCours) bAnimationEnCours = TRUE;
	else return FALSE;


	pEntity->pSprEntity->pRectPosition->x = x;
	pEntity->pSprEntity->pRectPosition->y = y;

	pEntity->mouvement = 1;
	pEntity->iTempoAnim += 1;
	if (pEntity->iTempoAnim == 7)	//Si la tempo est arrivée ŕ son terme :
	{
		pEntity->pSprEntity->iCurrentFrame += 1; //	- Frame suivante
		if (pEntity->pSprEntity->iCurrentFrame == pEntity->pSprEntity->iNbFrames) //Si l'animation est arrivée au bout 
		{
			pEntity->pSprEntity->iCurrentFrame = 0;
			pEntity->iTempoAnim = 0;
			return bAnimationEnCours = FALSE;
		}
		pEntity->iTempoAnim = 0;
	}

	return TRUE;
}


/*!
*  \fn     Uint32 DetectPassageBuisson(Kr_Level *pLevel, Entity *pPlayer, Uint32 *px, Uint32 *py)
*  \brief  Function to check if the player is moving in a bush (2 types of bush)
*
*  \param  pLevel   a pointer to the level
*  \param  pPlayer  a pointer to the entity
*  \param  px		coordinate of the bush
*  \param  py		coordinate of the bush
*  \return	0 : Nothing
			1 : Bush n°1
			2 : Bush n°2
*/
Uint32 DetectPassageBuisson(Kr_Level *pLevel, Entity *pPlayer, Uint32 *px, Uint32 *py)
{
	Uint32 iRetour = 0;
	Uint32 iPlayerMidX = 0, iPlayerMidY = 0;

	iPlayerMidX = pPlayer->pSprEntity->pRectPosition->x + (pPlayer->pSprEntity->pRectPosition->w / 2);
	iPlayerMidY = pPlayer->pSprEntity->pRectPosition->y + (pPlayer->pSprEntity->pRectPosition->h / 2);
	if (Kr_Level_GetTile(pLevel, iPlayerMidX, iPlayerMidY) == 9) iRetour = 1;
	else if (Kr_Level_GetTile(pLevel, iPlayerMidX, iPlayerMidY) == 40) iRetour = 2;
	*px = (iPlayerMidX / pLevel->pLevel_Tileset->iTilesWidth) * pLevel->pLevel_Tileset->iTilesWidth;
	*py = (iPlayerMidY / pLevel->pLevel_Tileset->iTilesHeight) * pLevel->pLevel_Tileset->iTilesHeight;
	return iRetour;
}



/*!
*  \fn     Entity *ChargementPapillon(SDL_Renderer *pRenderer)
*  \brief  Function to load the entity "papillon"

*  \param pRenderer a pointer to to the renderer
*  \return the loaded entity
*/
Entity *ChargementPapillon(SDL_Renderer *pRenderer)
{
	Entity *pEntity = NULL;
	Kr_Sprite *pSprite1 = NULL;
	SDL_Rect  *pPapillon = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (!pPapillon) return NULL;
	pPapillon->x = 0;
	pPapillon->y = 0;
	pPapillon->w = 13;
	pPapillon->h = 13;

	/* Chargement des sprites */
	pSprite1 = Kr_Sprite_Init("papillon");
	if (Kr_Sprite_Load(pSprite1, nord, 13, 52, 4, pPapillon, pRenderer) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite papillon!\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	/* Chargement des personnages */
	pEntity = Entity_Init("papillon");
	if (Entity_Load(pEntity, 100, 0, MOVESPEED, noclip, TRUE, pSprite1) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the entity !\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	return pEntity;
}


/*!
*  \fn     Boolean CalculApparitionPapillon(Boolean bCalculer, Kr_Level *pLevel, Entity *pEntity, Uint32 *iNumber);
*  \brief  Function to check if the level can spawn Papillon on the map

*  \param  bCalculer When this value is set to TRUE, the function will check the level
*  \param  pLevel    a pointer to the level
*  \param  pEntity   a pointer to the entity
*  \param  iNumber	 this value will return the number of entity that the level should spawn

*  \return none
*/
void CalculApparitionPapillon(Boolean bCalculer, Kr_Level *pLevel, Entity *pEntity, Uint32 *iNumber)
{
	Sint32 i = 0, j = 0, iValue = 0, iNumTile = 0, iTmp = 0;

	for (i = 0; i < pLevel->iLevel_TileWidth; i++)
	{
		for (j = 0; j < pLevel->iLevel_TileHeight; j++)
		{
			iNumTile = pLevel->szLayout[i][j];
			if (pLevel->pLevel_Tileset->pTilesProp[iNumTile].iArbre)
			{
				iValue++;
			}
		}
	}
	*iNumber = 0;
	// Calcul des ratios de tile "arbre" par rapport au nombre de tile totale du level
	if (iValue > ((pLevel->iLevel_TileWidth * pLevel->iLevel_TileHeight) / 4))
	{
		*iNumber = 4;
	}
	else if (iValue > ((pLevel->iLevel_TileWidth * pLevel->iLevel_TileHeight) / 8))
	{
		*iNumber = 2;
	}
	else if (iValue > ((pLevel->iLevel_TileWidth * pLevel->iLevel_TileHeight) / 16))
	{
		*iNumber = 1;
	}

	if (*iNumber > 0)
	{
		iTmp = rand() % iValue;
		iValue = 0;
		for (i = 0; i < pLevel->iLevel_TileWidth; i++)
		{
			for (j = 0; j < pLevel->iLevel_TileHeight; j++)
			{
				iNumTile = pLevel->szLayout[i][j];
				if (pLevel->pLevel_Tileset->pTilesProp[iNumTile].iArbre)
				{
					iValue++;
				}
				if (iTmp == iValue)
				{
					pEntity->pSprEntity->pRectPosition->x = i * pLevel->pLevel_Tileset->iTilesWidth;
					pEntity->pSprEntity->pRectPosition->y = j * pLevel->pLevel_Tileset->iTilesWidth;
					iTmp--;// On bloque ainsi tout retour dans cette boucle
				}
			}
		}
	}
}



/*!
*  \fn     Boolean UpdateButterfly(Entity *pButterfly, Boolean bActiver, SDL_Renderer *pRenderer, Kr_Level *pLevel, Kr_Sound *pSndButterfly)
*  \brief  Function to update the behaviour of a butterfly

*  \param  pButterfly a pointer to the entity
*  \param  bActiver   TRUE if we must start a sequence of flying bird
*  \param  pRenderer  a pointer to the renderer
*  \param  pLevel	  a pointer to the level
*  \param  pButterfly a pointer to the sound
*  \return ?????????????
*/
Boolean UpdateButterfly(Entity *pButterfly, Boolean bActiver, SDL_Renderer *pRenderer, Kr_Level *pLevel, Kr_Sound *pSndButterfly)
{
	static Boolean bAnimationEnCours = FALSE; // Permet de savoir si une animation de pigeon dans les airs a lieu
	static Uint32 iTime = 1545; // durée de l'animation (tour de boucle)

	Sint32 movex = 0, movey = 0;
	Direction newDir = sud;
	if (!bActiver) return FALSE;
	if (bActiver && iTime == 0) Kr_Sound_Play(pSndButterfly, KR_SOUND_NATURE_CANAL, 25, 0); // On le joue une seule fois
		GenerateRandomVector(&movex, &movey, 1, 2, pButterfly, NULL, NULL, 25, 60);
		newDir = foundDirection(movex, movey, pButterfly);
		switchTextureFromDirection(pButterfly, newDir, pRenderer);
		if (ButterflyCollision(pLevel, pButterfly->pSprEntity->pRectPosition, movex, movey) == FALSE)
		{
			pButterfly->mouvement = 1;
			pButterfly->iTempoAnim += 1;
			if (pButterfly->iTempoAnim == 7)	//Si la tempo est arrivée ŕ son terme :
			{
				pButterfly->pSprEntity->iCurrentFrame += 1; //	- Frame suivante
				if (pButterfly->pSprEntity->iCurrentFrame == pButterfly->pSprEntity->iNbFrames) //Si l'animation est arrivée au bout 
				{
					pButterfly->pSprEntity->iCurrentFrame = 0;
					pButterfly->iTempoAnim = 0;
				}
				pButterfly->iTempoAnim = 0;
			}
			pButterfly->pSprEntity->pRectPosition->x += movex;
			pButterfly->pSprEntity->pRectPosition->y += movey;
			pButterfly->iCurrentMoveX = movex;
			pButterfly->iCurrentMoveY = movey;
		}
		else
		{
			pButterfly->mouvement = 0;
			pButterfly->iTempoAnim = 0;
			pButterfly->iCurrentMoveX = 0;
			pButterfly->iCurrentMoveY = 0;
		}
	if (iTime == 0)
	{
		iTime = 1545;
	}
	iTime--;
	return TRUE;
}

Boolean ButterflyCollision(Kr_Level *pLevel, SDL_Rect *pRect, Uint32 vx, Uint32 vy)
{
	Uint32 iMinX, iMinY, iMaxX, iMaxY, i, j, iNumTile;
	SDL_Rect pRect1;
	pRect1 = *pRect;
	pRect1.x += vx;
	pRect1.y += vy;
	// Verifie si on est pas déjŕ hors map
	if (pRect1.x < 0 || ((pRect1.x + pRect1.w - 1) >= pLevel->iLevel_TileWidth  * pLevel->pLevel_Tileset->iTilesWidth) ||
		pRect1.y < 0 || ((pRect1.y + pRect1.h - 1) >= pLevel->iLevel_TileHeight * pLevel->pLevel_Tileset->iTilesHeight))
	{
		return TRUE;
	}

	// Détermine les tiles ŕ controler
	iMinX = pRect1.x / pLevel->pLevel_Tileset->iTilesWidth;
	iMinY = pRect1.y / pLevel->pLevel_Tileset->iTilesHeight;
	iMaxX = (pRect1.x + pRect1.w - 1) / pLevel->pLevel_Tileset->iTilesWidth;
	iMaxY = (pRect1.y + pRect1.h - 1) / pLevel->pLevel_Tileset->iTilesHeight;

	for (i = iMinX; i <= iMaxX; i++)
	{
		for (j = iMinY; j <= iMaxY; j++)
		{
			iNumTile = pLevel->szLayout[i][j];
			if (pLevel->pLevel_Tileset->pTilesProp[iNumTile].iArbre == 0)
			{
				return TRUE;
			}
		}
	}
	return FALSE;
}





/*!
*  \fn     Entity *ChargementPigeonVolant(SDL_Renderer *pRenderer)
*  \brief  Function to load the flying bird

*  \param pRenderer a pointer to to the renderer
*  \return the loaded entity
*/

Entity *ChargementPigeonVolant(SDL_Renderer *pRenderer)
{
	Entity *pEntity = NULL;
	Kr_Sprite *pSprite1 = NULL;
	SDL_Rect *pRectPigeonVolant = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	if (!pRectPigeonVolant) return  NULL;
	pRectPigeonVolant->x = 0;
	pRectPigeonVolant->y = 0;
	pRectPigeonVolant->w = 32;
	pRectPigeonVolant->h = 32;


	/* Chargement des sprites */
	pSprite1 = Kr_Sprite_Init("pigeon2");
	if (Kr_Sprite_Load(pSprite1, sud, 23, 116, 4, pRectPigeonVolant, pRenderer) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite pigeon2!\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	/* Chargement des personnages */
	pEntity = Entity_Init("pigeon2");
	if (Entity_Load(pEntity, 100, 0, MOVESPEED, noclip, TRUE, pSprite1) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the entity !\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	return pEntity;
}




/*!
*  \fn     Boolean PigeonVol(Entity *pPigeonVol, Boolean bActiver, SDL_Renderer *pRenderer, Kr_Level *pLevel, Kr_Sound *pSndPigeon, Uint32 xDebut, Uint32 yDebut, Uint32 *xFin, Uint32 *yFin)
*  \brief  Function to update the behaviour of the flying bird

*  \param  pPigeonVol When this value is set to TRUE, the function will check the level
*  \param  bActiver   TRUE if we must start a sequence of flying bird
*  \param  pRenderer  a pointer to the renderer
*  \param  pLevel	  a pointer to the level
*  \param  pSndPigeon a pointer to the sound
*  \param  xDebut	  coordinate of the bird when it was on the ground
*  \param  yDebut	  coordinate of the bird when it was on the ground
*  \param  xFin		  coordinate of the bird when it land on the ground
*  \param  yFin		  coordinate of the bird when it land on the ground
*  \return TRUE if a bird is still flying, FALSE otherwise
*/
Boolean PigeonVol(Entity *pPigeonVol, Boolean bActiver, SDL_Renderer *pRenderer, Kr_Level *pLevel, Kr_Sound *pSndPigeon, Uint32 xDebut, Uint32 yDebut, Uint32 *xFin, Uint32 *yFin)
{
	static Boolean bAnimationEnCours = FALSE; // Permet de savoir si une animation de pigeon dans les airs a lieu
	static Uint32 iTime = 600; // durée de l'animation (tour de boucle)

	Sint32 movex = 0, movey = 0;
	Direction newDir = sud; 
	
	if (bActiver && !bAnimationEnCours) Kr_Sound_Play(pSndPigeon, KR_SOUND_ANIMAL_CANAL, 25, 0); // On le joue une seule fois
	if (bActiver == TRUE)
	{
		bAnimationEnCours = TRUE;
		pPigeonVol->pSprEntity->pRectPosition->x = xDebut;
		pPigeonVol->pSprEntity->pRectPosition->y = yDebut;
	}
	if (bAnimationEnCours == FALSE) return FALSE;

	if (iTime > 0)
	{
		GenerateRandomVector(&movex, &movey, 2, 3, pPigeonVol, pLevel, NULL, 25, 40);
		newDir = foundDirection(movex, movey, pPigeonVol);
		switchTextureFromDirection(pPigeonVol, newDir, pRenderer);

		pPigeonVol->mouvement = 1;
		pPigeonVol->iTempoAnim += 1;
		if (pPigeonVol->iTempoAnim == 7)	//Si la tempo est arrivée ŕ son terme :
		{
			pPigeonVol->pSprEntity->iCurrentFrame += 1; //	- Frame suivante
			if (pPigeonVol->pSprEntity->iCurrentFrame == pPigeonVol->pSprEntity->iNbFrames) //Si l'animation est arrivée au bout 
			{
				pPigeonVol->pSprEntity->iCurrentFrame = 0;
				pPigeonVol->iTempoAnim = 0;
			}
			pPigeonVol->iTempoAnim = 0;
		}
		pPigeonVol->pSprEntity->pRectPosition->x += movex;
		pPigeonVol->pSprEntity->pRectPosition->y += movey;
		pPigeonVol->iCurrentMoveX = movex;
		pPigeonVol->iCurrentMoveY = movey;
	}
	else
	{
		*xFin = pPigeonVol->pSprEntity->pRectPosition->x;
		*yFin = pPigeonVol->pSprEntity->pRectPosition->y;
		iTime = 600;
		bAnimationEnCours = FALSE;
		return FALSE;
	}
	iTime--;
	return TRUE;
}

/* ========================================================================= */
/*!
* \file    kr_level.c
* \brief   Contains the functions to handle the level.
* \author  Olivier Herrou
* \version 1.0
* \date    01 Mars 2015
*/
/* ================================================================================================================ */
/* Developers    | Date       | Comments																			*/
/* --------------+------------+------------------------------------------------------------------------------------ */
/* Herrou        | 01/03/2015 | Création																			*/
/* Herrou        | 08/03/2015 | Gestion du Scrolling en cours														*/
/* Herrou        | 18/03/2015 | Arret du scrolling, map en 40x22 en tiles de 32										*/
/* Herrou        | 19/03/2015 | Détection collision d'un rectangle avec la carte									*/
/*               |            | Gestion des grandes vitesses de déplacement											*/
/* Herrou        | 21/03/2015 | MAJ szLayout unsigned char => Uint32												*/
/* Herrou        | 22/03/2015 | Fonction Kr_Level_GetBlock OK														*/
/* Herrou        | 23/03/2015 | Renommer GetBlock en GetTile			     										*/
/*               |            | Kr_Level_Event, OK                           										*/
/* Herrou        | 24/03/2015 | MAJ Free, Add Change																*/
/*               |            | MAJ Event: détection du milieu du rectangle pour le changement de niveau			*/
/* ===============================================================================================================  */

/*
Commentaire : 


*/
#include "kr_level.h"

#define CACHE_SIZE 15000

/*!
*  \fn     Kr_Tileset *Kr_Level_Init(const char *szFileName)
*  \brief  Function to load initialize a Level thanks to the level file
*
*  \param  szFileName the name of the level file
*  \return the initialize structure
*/
Kr_Level *Kr_Level_Init(const char *szFileName)
{
	Kr_Level *pLevel = NULL;
	pLevel = (Kr_Level *)UTIL_Malloc(sizeof(Kr_Level));

	pLevel->szLevelName = szFileName;
	pLevel->iLevel_TileWidth = 0;
	pLevel->iLevel_TileHeight = 0;
	pLevel->pLevel_Tileset = NULL;
	pLevel->szLayout = NULL;
	return pLevel;
}


/*!
*  \fn     Boolean Kr_Level_Load(Kr_Level *pLevel)
*  \brief  Function to load a Kr_Level structure via a level file
*
*  \param  pLevel    a pointer to a the level structure
*  \param  pRenderer a pointer to the renderer
*  \return TRUE if everything is ok, NULL otherwise
*/
Boolean   Kr_Level_Load(Kr_Level *pLevel, SDL_Renderer *pRenderer)
{
	char         szBuf[CACHE_SIZE];  // Buffer
	char         szBuf2[CACHE_SIZE]; // Buffer2
	char         szLevelPath[50];
	FILE        *pFile;

	/* Ouverture du fichier leveàl */
	sprintf(szLevelPath, "maps\\%s.txt", pLevel->szLevelName);
	Kr_Log_Print(KR_LOG_INFO, "Opening level file %s\n", szLevelPath);
	pFile = UTIL_OpenFile(szLevelPath, "r"); // Ouverture du level en read
	if (!pFile) return FALSE;

	/* Vérification de la version du fichier tileset */
	fgets(szBuf, CACHE_SIZE, pFile);
	if (strstr(szBuf, KR_LEVEL_VERSION) == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Wrong version of the level file, expected :%s\n"KR_LEVEL_VERSION);
		Kr_Log_Print(KR_LOG_ERROR, ">> Level \"%s\".\n", szLevelPath);
		return FALSE;
	}

	do // Lecture ligne par ligne du fichier
	{
		fgets(szBuf, CACHE_SIZE, pFile);
		if (strstr(szBuf, "#tileset")) // Identification de la ligne tileset
		{
			fscanf(pFile, "%s",szBuf2); // Lecture de la ligne suivante qui indique le fichier .tls
	
			pLevel->pLevel_Tileset = Kr_Tileset_Init(szBuf2);
			if (!Kr_Tileset_Load(pLevel->pLevel_Tileset, pRenderer))
			{
				Kr_Log_Print(KR_LOG_ERROR, "Can't load \"%s\" in the level, must quit !\n", szBuf2);
				return FALSE;
			}
		}
		if (strstr(szBuf, "#layout")) // Identification de la ligne layout
		{
			if (!Kr_Level_Layout(pLevel, pFile))
			{
				Kr_Log_Print(KR_LOG_ERROR, "Can't load the layout, must quit !\n");
				return FALSE;
			}

		}
	} while (strstr(szBuf, "#end") == NULL); // Identification de la fin du fichier level

	UTIL_CloseFile(&pFile);
	Kr_Log_Print(KR_LOG_INFO, "tiles %d %d !\n", pLevel->iLevel_TileHeight, pLevel->pLevel_Tileset->iTilesHeight);
	Kr_Log_Print(KR_LOG_INFO, "Level : %s has been loaded !\n", pLevel->szLevelName);
	return TRUE;
}


/*!
*  \fn     void Kr_Level_Free(Kr_Level *pLevel)
*  \brief  Function to free a Kr_Level structure
*
*  \param  pLevel a pointer to a the level structure
*  \return none
*/
void Kr_Level_Free(Kr_Level *pLevel)
{
	Sint32 i;
	if (pLevel == NULL) return;
	Kr_Tileset_Free(pLevel->pLevel_Tileset);
	for (i = 0; i< pLevel->iLevel_TileHeight; i++)
		free(pLevel->szLayout[i]);
	UTIL_Free(pLevel->szLayout);
	UTIL_Free(pLevel);
}


/*!
*  \fn     Boolean Kr_Level_Layout(Kr_Level *pLevel, FILE *pFile)
*  \brief  Function to load the layout in the Kr_Level structure
*
*  \param  pLevel a pointer to a the level structure
*  \param  pFile  a pointer to the file
*  \return TRUE if everything is OK, FALSE otherwise
*/
Boolean Kr_Level_Layout(Kr_Level *pLevel, FILE *pFile)
{
	Sint32 i, j, iTmp;

	fscanf(pFile, "%d %d", &pLevel->iLevel_TileWidth, &pLevel->iLevel_TileHeight);

	/* Allocation du tableau 2D szLayout */
	pLevel->szLayout = malloc(pLevel->iLevel_TileWidth*sizeof(Uint32*));
	for (i = 0; i<pLevel->iLevel_TileWidth; i++)
		pLevel->szLayout[i] = malloc(pLevel->iLevel_TileHeight*sizeof(Uint32));

	/* Affectation des données level au schema */
	for (j = 0; j<pLevel->iLevel_TileHeight; j++)
	{
		for (i = 0; i<pLevel->iLevel_TileWidth; i++)
		{
			fscanf(pFile, "%d", &iTmp); 
			if (iTmp >= pLevel->pLevel_Tileset->iNbTilesX * pLevel->pLevel_Tileset->iNbTilesY) // On vérifie que le codage du tile existe
			{
				Kr_Log_Print(KR_LOG_ERROR, "Can't load a tile in the layout because it's out of the tileset file ");
				Kr_Log_Print(KR_LOG_ERROR, ">> Tile n°\"%d\", but iNbTilesX = %d and iNbTilesY = %d", iTmp, pLevel->pLevel_Tileset->iNbTilesX, pLevel->pLevel_Tileset->iNbTilesY);
				return FALSE;
			}
			pLevel->szLayout[i][j] = iTmp;
		}
	}
	Kr_Log_Print(KR_LOG_INFO, "Kr_Level_Layout: Done\n");
	return TRUE;
}


/*!
*  \fn     void Kr_Level_Draw(SDL_Renderer *pRenderer, Kr_Level *pLevel)
*  \brief  Function to draw a level on a renderer
*
*  \param  pRenderer a pointer to the renderer
*  \param  pLevel    a pointer to a the level structure
*  \return none
*/
void Kr_Level_Draw(SDL_Renderer *pRenderer, Kr_Level *pLevel)
{
	Sint32   i, j;
	SDL_Rect Rect_dest;
	Sint32   iNumTile;


	for (i = 0; i <= pLevel->iLevel_TileWidth; i++)
	{
		for (j = 0; j <= pLevel->iLevel_TileHeight; j++)
		{
			Rect_dest.x = i*pLevel->pLevel_Tileset->iTilesWidth;
			Rect_dest.y = j*pLevel->pLevel_Tileset->iTilesHeight;
			Rect_dest.h = pLevel->pLevel_Tileset->iTilesHeight;
			Rect_dest.w = pLevel->pLevel_Tileset->iTilesWidth;
			if (i < 0 || i >= pLevel->iLevel_TileWidth || j < 0 || j >= pLevel->iLevel_TileHeight)
				iNumTile = 0;
			else
			{
				iNumTile = pLevel->szLayout[i][j];
				
			}
			SDL_RenderCopy(pRenderer, pLevel->pLevel_Tileset->pTextureTileset, &(pLevel->pLevel_Tileset->pTilesProp[iNumTile].rTile), &Rect_dest); // En arrière plan si la fonction Kr_Map_Draw est appelé au tout début
		}
	}
}

/*!
*  \fn     Uint32 Kr_Collision_Move(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy)
*  \brief  Function to move a rectangle on a level (Recursif !)
*
*  \TODO : Code de retour d'erreur savoir s'il a échoué tout, s'il a échoué mais affiné, etc
*
*  \param  pLevel a pointer to a the level structure
*  \param  pRect1 a pointer to the rectangle you want to move
*  \param  vx     the vector on X
*  \param  vy     the vector on Y
*  \return 1, 2 or 3 (debuging only)
*/
Uint32 Kr_Collision_Move(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy)
{
	/* Gestion des dépassements très rapides, pour ne pas passer au dela du mur*/
	if (UTIL_ABS(vx) >= pLevel->pLevel_Tileset->iTilesWidth || UTIL_ABS(vy) >= pLevel->pLevel_Tileset->iTilesHeight) // Prendre valeur absolu sinon cela fonctionnera pas pour des vecteurs négatifs
	{
		Kr_Collision_Move(pLevel, pRect1, vx / 2, vy / 2);
		Kr_Collision_Move(pLevel, pRect1, vx - vx / 2, vy - vy / 2);
		return 1;
	}
	if (Kr_Collision_TryMove(pLevel, pRect1, vx, vy) == TRUE) return 2;
	
	Kr_Collision_Affine(pLevel, pRect1, vx, vy);
	return 3;
}

/*!
*  \fn     Kr_Collision_IsCollisionDecor(Kr_Level *pLevel, SDL_Rect *pRect1)
*  \brief  Function to detect if the rectangle is colliding with the level tiles
*
*  \param  pLevel a pointer to a the level structure
*  \param  pRect1  a pointer to the rectangle you want to test
*  \return TRUE if the two rectangle are colliding, FALSE otherwise
*/
Boolean Kr_Collision_IsCollisionDecor(Kr_Level *pLevel, SDL_Rect *pRect1)
{
	Uint32 iMinX, iMinY, iMaxX, iMaxY, i, j, iNumTile;

	// Verifie si on est pas déjà hors map
	if (pRect1->x < 0 || ((pRect1->x + pRect1->w - 1) >= pLevel->iLevel_TileWidth  * pLevel->pLevel_Tileset->iTilesWidth) ||
		pRect1->y < 0 || ((pRect1->y + pRect1->h - 1) >= pLevel->iLevel_TileHeight * pLevel->pLevel_Tileset->iTilesHeight))
	{
		return TRUE;
	}

	// Détermine les tiles à controler
	iMinX = pRect1->x / pLevel->pLevel_Tileset->iTilesWidth;
	iMinY = pRect1->y / pLevel->pLevel_Tileset->iTilesHeight;
	iMaxX = (pRect1->x + pRect1->w - 1) / pLevel->pLevel_Tileset->iTilesWidth;
	iMaxY = (pRect1->y + pRect1->h - 1) / pLevel->pLevel_Tileset->iTilesHeight;

	for (i = iMinX; i <= iMaxX; i++)
	{
		for (j = iMinY; j <= iMaxY; j++)
		{
			iNumTile = pLevel->szLayout[i][j];

			if (pLevel->pLevel_Tileset->pTilesProp[iNumTile].iPlein)
			{
				//(KR_LOG_WARNING, "CollisionDecor:  Collision avec la Tile : %d %d \n",i,j); 
				return TRUE;
			}
		}
	}
	//Kr_Log_Print(KR_LOG_WARNING, "CollisionDecor: %d tiles analysées \n", i*j);
	return FALSE;
}

/*!
*  \fn     Boolean Kr_Collision_TryMove(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy)
*  \brief  Function to try to move a rectangle with a certain vector speed and check if it's colliding with the level tiles
*
*  \param  pLevel a pointer to a the level structure
*  \param  pRect1  a pointer to the first rectangle you want to test
*  \param  vx      the vector on X
*  \param  vy      the vector on Y
*  \return TRUE if the two rectangle are NOT colliding, FALSE otherwise
*/
Boolean Kr_Collision_TryMove(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy)
{
	SDL_Rect test;
	test = *pRect1;
	test.x += vx;
	test.y += vy;
	if (Kr_Collision_IsCollisionDecor(pLevel, &test) == FALSE)
	{
		*pRect1 = test;
		return TRUE;
	}
	return FALSE;
}

/*!
*  \fn     void Kr_Collision_Affine(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy)
*  \brief  Function to reduce the vector speed to check if there is a collision between the rectangle and the level tiles
*
*  \param  pLevel a pointer to a the level structure
*  \param  pRect1  a pointer to the first rectangle you want to test
*  \param  vx      the vector on X
*  \param  vy      the vector on Y
*  \return none
*/
void Kr_Collision_Affine(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy)
{
	Sint32 i;
	for (i = 0; i<UTIL_ABS(vx); i++)
	{
		if (Kr_Collision_TryMove(pLevel, pRect1, UTIL_SGN(vx), 0) == FALSE)
			break;
	}
	for (i = 0; i<UTIL_ABS(vy); i++)
	{
		if (Kr_Collision_TryMove(pLevel, pRect1, 0, UTIL_SGN(vy)) == FALSE)
			break;
	}
}



/*!
*  \fn     Uint32 Kr_Level_Event(Kr_Level *pLevel, SDL_Rect *pRect)
*  \brief  Function to check some event
*
*  \param  pLevel a pointer to a the level structure
*  \param  pRect1 a pointer to the rectangle of the player
*  \return 1 if we must change the level, 0 otherwise
*/
Uint32 Kr_Level_Event(Kr_Level *pLevel, SDL_Rect *pRect)
{
	Uint32 x, y, i,j, iTmp;
	Sint32 iTilesID;

	/* événement Changement de Level*/
	iTmp = 0;
	// Calcule des coordonnées du milieu du rectangle
	x = pRect->x + pRect->w / 2;
	y = pRect->y + pRect->h / 2;
	iTilesID = Kr_Level_GetTile(pLevel, x, y);
	if (pLevel->pLevel_Tileset->pTilesProp[iTilesID].iPorteLevel && iTilesID != -1) return 1;// Le tile est-il un Tile pour changer de level ?


	/* Autre événement */

	return 0;
}

/*!
*  \fn     Sint32 Kr_Level_GetTile(Kr_Level *pLevel, Uint32 x, Uint32 y)
*  \brief  Function to get the block at a coordinate
*
*  \param  pLevel a pointer to a the level structure
*  \param  x      x coordinate
*  \param  y      y coordinate
*  \return the id of the block (cf the tileset), -1 if error
*/
Sint32 Kr_Level_GetTile(Kr_Level *pLevel, Uint32 x, Uint32 y)
{
	Uint32 iTilesID, iNumTilesX, iNumTilesY;
	// Obtenir les numéros des tiles
	if ((x >= (pLevel->iLevel_TileWidth * pLevel->pLevel_Tileset->iTilesWidth)) || (y >= (pLevel->iLevel_TileHeight * pLevel->pLevel_Tileset->iTilesHeight)))
	{
		//Kr_Log_Print(KR_LOG_WARNING, "GetTile : Out of level X: %d, Y: %d!!! \n",x,y);
		return iTilesID = -1;
	}
	iNumTilesX = x / pLevel->pLevel_Tileset->iTilesWidth;
	iNumTilesY = y / pLevel->pLevel_Tileset->iTilesHeight;
	iTilesID = pLevel->szLayout[iNumTilesX][iNumTilesY];
	//Kr_Log_Print(KR_LOG_INFO, "Tiles %d   |  X: %d   Y: %d  |   PorteLEvel : %d \n", iTilesID,x,y, pLevel->pLevel_Tileset->pTilesProp[iTilesID].iPorteLevel);
	return iTilesID;
}



/*!
*  \fn     Kr_Level *Kr_Level_Change(Kr_Level *pCurrentLevel, char* szLevelName, SDL_Renderer *pRenderer)
*  \brief  Function to change the level
*
*  \param  pCurrentLevel  a pointer to the current Level which must be freed
*  \param  szLevelName    the name of the new Level to load
*  \param  pRenderer      a pointer to the Renderer
*  \return the initialised level structure, NULL otherwise
*/
Kr_Level *Kr_Level_Change(Kr_Level *pCurrentLevel, char* szLevelName, SDL_Renderer *pRenderer)
{
	Kr_Level_Free(pCurrentLevel);
	Kr_Level *pNewLevel = Kr_Level_Init(szLevelName);
	if (!Kr_Level_Load(pNewLevel, pRenderer))
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't Load a level\n");
		return NULL;
	}
	return pNewLevel;
}
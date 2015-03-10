/* ========================================================================= */
/*!
* \file    kr_map.c
* \brief   Contains the functions to handle the map.
* \author  Olivier Herrou
* \version 1.0
* \date    18 Février 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 18/02/2015 | Creation.                                    */
/*               | 21/02/2015 | Ajout de kr_quit                             */
/*               |            | TODO : Sortir proprement en cas de probleme  */
/*               | 24/02/2015 |                                              */
/* ========================================================================= */

#include "kr_map.h"

#define CACHE_SIZE 5000


/*!
*  \fn     void Kr_Map_LoadTileset(FILE* pFile, Kr_Map* pMap)
*  \brief  Function  to load the tileset properties in the structure Kr_Map
*
*  \param  pFile a pointer to the opened file
*  \param  pMap  a pointer to a Map Structure
*  \return none
*/
void Kr_Map_LoadTileset(FILE *pFile, Kr_Map *pMap)
{
	Uint32 iNumTile;
	Sint32 i, j;
	char         szBuf[CACHE_SIZE];  // Buffer
	char         szBuf2[CACHE_SIZE]; // Buffer2
	SDL_Surface *pSurfTileset = NULL;
	char         szTilePath[50];

	/* Lecture nom du tileset et chargement de celui-ci*/
	fscanf(pFile, "%s", szBuf); 
	sprintf(szTilePath, "tilesets\\%s", szBuf);
	Kr_Log_Print(KR_LOG_INFO, "Kr_Map_LoadTileset of the file %s\n", szTilePath);

	if (UTIL_OpenFile(szTilePath, "r") == NULL) // Vérifier que le fichier est présent
	{
		Kr_Quit();// QUITTER ICI, le message d'erreur est géré par util
	}

	pSurfTileset = IMG_Load(szTilePath);// Chargement du tileset depuis le répertoire tilesets

	/* Lecture iNbTilesX et iNbTilesY */
	fscanf(pFile, "%d %d", &pMap->iNbTilesX, &pMap->iNbTilesY); 

	/* Calcul iTilesWidth et iTilesHeight*/
	pMap->iTilesWidth = pSurfTileset->w / pMap->iNbTilesX;  
	pMap->iTilesHeight = pSurfTileset->h / pMap->iNbTilesY;  

	/* TextureTileSet */
	pMap->pTextureTileset = SDL_CreateTextureFromSurface(gpRenderer, pSurfTileset);
	SDL_FreeSurface(pSurfTileset);
	if (pMap->pTextureTileset == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't create a texture from Kr_Map_LoadTileset!\n");
		Kr_Quit();// QUITTER ICI
	}

	pMap->tp_Props = malloc(pMap->iNbTilesX*pMap->iNbTilesY*sizeof(Kr_TileProp)); // Allocation dynamique des propriétés du tileset

	/* Envoie de donnée au fichier Log */
	Kr_Log_Print(KR_LOG_INFO, "Loading tileset : %s\n", szBuf);
	Kr_Log_Print(KR_LOG_INFO, "iNbTilesX   : %d  |  iNbTilesY    : %d\n", pMap->iNbTilesX, pMap->iNbTilesY);
	Kr_Log_Print(KR_LOG_INFO, "iTilesWidth : %d  |  iTilesHeight : %d\n", pMap->iTilesWidth, pMap->iTilesHeight);

	/* Affectation des caractéristiques à chaque tile*/
	for (j = 0, iNumTile = 0; j<pMap->iNbTilesY; j++) // Sur Y
	{
		for (i = 0; i<pMap->iNbTilesX; i++, iNumTile++) // Sur X
		{
			pMap->tp_Props[iNumTile].R.w = pMap->iTilesWidth; //Donnée du SDL_RECT  width
			pMap->tp_Props[iNumTile].R.h = pMap->iTilesHeight; //Donnée du SDL_RECT  height
			pMap->tp_Props[iNumTile].R.x = i*pMap->iTilesWidth; //Donnée du SDL_RECT  x
			pMap->tp_Props[iNumTile].R.y = j*pMap->iTilesHeight; //Donnée du SDL_RECT  y
			//Kr_Log_Print(KR_LOG_INFO, "\n");
			//Kr_Log_Print(KR_LOG_INFO, "iNumTile : %d\n",iNumTile);
			//Kr_Log_Print(KR_LOG_INFO, "R.w : %d\tR.h : %d\tR.x : %d\tR.y : %d\n", pMap->tp_Props[iNumTile].R.w, pMap->tp_Props[iNumTile].R.h, pMap->tp_Props[iNumTile].R.x, pMap->tp_Props[iNumTile].R.y);
			
			/* Attribut du tile*/ 
			fscanf(pFile, "%s %s", szBuf, szBuf2); // Rappel : fscanf s'arrete au premier espace rencontré ou \n
			//Kr_Log_Print(KR_LOG_INFO, "%s %s\n", szBuf, szBuf2);
			pMap->tp_Props[iNumTile].iPlein = 0;
			if (strcmp(szBuf2, "plein") == 0) pMap->tp_Props[iNumTile].iPlein = 1;
		}
	}
	Kr_Log_Print(KR_LOG_INFO, "Kr_Map_LoadTileset : Done\n");
}



/*!
*  \fn     void Kr_Map_LoadTileset(FILE* pFile, Kr_Map* pMap)
*  \brief  Function to load the level properties in the structure Kr_Map
*
*  \param  pFile a pointer to the opened file
*  \param  pMap  a pointer to a Map Structure
*  \return none
*/
void Kr_Map_LoadLevel(FILE *pFile, Kr_Map *pMap)
{
	Sint32 i, j, iTmp;

	fscanf(pFile, "%d %d", &pMap->iNbTiles_WidthLevel, &pMap->iNbTiles_HeightLevel);

	/* Allocation du tableau 2D m->schema */
	pMap->szSchema = malloc(pMap->iNbTiles_WidthLevel*sizeof(unsigned char*));
	for (i = 0; i<pMap->iNbTiles_WidthLevel; i++)
		pMap->szSchema[i] = malloc(pMap->iNbTiles_HeightLevel*sizeof(unsigned char));

	/* Affectation des données level au schema */
	for (j = 0; j<pMap->iNbTiles_HeightLevel; j++)
	{
		for (i = 0; i<pMap->iNbTiles_WidthLevel; i++)
		{
			fscanf(pFile, "%d", &iTmp); // Stocke la valeur
			if (iTmp >= pMap->iNbTilesX*pMap->iNbTilesY) // On vérifie que le codage du tile existe
			{
				Kr_Log_Print(KR_LOG_ERROR, "Kr_Map_LoadLevel : Can't load a tile because it's out of the tileset file ");
				Kr_Log_Print(KR_LOG_ERROR, "Kr_Map_LoadLevel : Tile n°\"%d\", but iNbTilesX = %d and iNbTilesY = %d", iTmp, pMap->iNbTilesX, pMap->iNbTilesY);

			}
			pMap->szSchema[i][j] = iTmp;
			//Kr_Log_Print(KR_LOG_INFO, "pMap->schema[%d][%d] = %d \n", i, j, pMap->szSchema[i][j]);
		}
	}
   Kr_Log_Print(KR_LOG_INFO, "Kr_Map_LoadLevel : WidthLevel %d  |  HeightLevel %d\n", pMap->iNbTiles_WidthLevel*pMap->iTilesWidth, pMap->iNbTiles_HeightLevel*pMap->iTilesHeight);
   Kr_Log_Print(KR_LOG_INFO, "Kr_Map_LoadLevel : Done\n");
}

/*!
*  \fn     Kr_Map* Kr_Map_LoadMap(const char *szLevel, Uint32 iWidth_ScrollWindow, Uint32 iHeight_ScrollWindow)
*  \brief  Function to initialize a Kr_Map structure from the data of the szLevel.txt file
*
*  \param  szLevel				name of the file from which the data will be load (Without the extension !)
*  \param  iWidth_ScrollWindow  width of the scroll window
*  \param  iHeight_ScrollWindow height of the scroll window
*  \return The initialized structure if everything is ok, NULL otherwise
*/
Kr_Map* Kr_Map_LoadMap(const char *szLevel, Uint32 iWidth_ScrollWindow, Uint32 iHeight_ScrollWindow)
{
	FILE   *pFile = NULL;
	Kr_Map *pMap = NULL;
	char    szBuf[CACHE_SIZE]; 
	char    szLevelPath[50];
	
	sprintf(szLevelPath, "maps\\%s.txt", szLevel);
	Kr_Log_Print(KR_LOG_INFO, "Kr_Map_LoadMap of the file %s\n", szLevelPath);
	pFile = UTIL_OpenFile(szLevelPath, "r"); // Ouverture du niveau en read
	if (!pFile)
	{
		return pMap;
	}
	/* Information Version */
	fgets(szBuf, CACHE_SIZE, pFile);
	if (strstr(szBuf, "Tilemapping Version 1.0") == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Wrong version of the level file, expected 1.0\n");
		Kr_Log_Print(KR_LOG_ERROR, ">> Level \"%s\".\n", szLevelPath);
		return pMap;
	}
	Kr_Log_Print(KR_LOG_INFO, "%s", szBuf);
		
	pMap = (Kr_Map *)UTIL_Malloc(sizeof(Kr_Map));
	do // Lecture ligne par ligne du fichier
	{
		fgets(szBuf, CACHE_SIZE, pFile); 
		if (strstr(szBuf, "#tileset")) // Si cette ligne est #tileset alors on charge le tileset
			Kr_Map_LoadTileset(pFile, pMap);
		if (strstr(szBuf, "#level")) // Si cette ligne est #level alors on charge le level
			Kr_Map_LoadLevel(pFile, pMap);
	} while (strstr(szBuf, "#fin") == NULL);

	pMap->iHeight_MapWindow = iHeight_ScrollWindow;
	pMap->iWidth_MapWindow = iWidth_ScrollWindow;
	pMap->iScrollX = 0;
	pMap->iScrollY = 0;

	UTIL_CloseFile(&pFile);
	Kr_Log_Print(KR_LOG_INFO, "Kr_Map_LoadMap : Done\n");
	return pMap;
}

/*!
*  \fn     void Kr_Map_Draw(SDL_Renderer *pRenderer, Kr_Map *pMap)
*  \brief  Function to draw the map 
*
*  \param  pRenderer a pointer to a renderer
*  \param  pMap      a pointer to a Kr_Map Structure
*  \return none
*/
/* With scrolling*/
void Kr_Map_Draw(SDL_Renderer *pRenderer, Kr_Map *pMap) // Il faudra préciser ici en plus comme paramètre le iScrollX et iScrollY qui seront les coordonnées de notre perso
{
	Sint32   i, j;
	SDL_Rect Rect_dest;
	Sint32   iNumTile;
	Sint32   iMinX, iMaxX, iMinY, iMaxY;
	Kr_Map_UpdateScroll2(pMap);

	iMinX = pMap->iScrollX / pMap->iTilesWidth;// -1;
	iMinY = pMap->iScrollY / pMap->iTilesHeight;// -1;
	iMaxX = (pMap->iScrollX + pMap->iWidth_MapWindow) / pMap->iTilesWidth;
	iMaxY = (pMap->iScrollY + pMap->iHeight_MapWindow) / pMap->iTilesHeight;

	for (i = iMinX; i <= iMaxX; i++)
	{
		for (j = iMinY; j <= iMaxY; j++)
		{
			Rect_dest.x = i*pMap->iTilesWidth - pMap->iScrollX;
			Rect_dest.y = j*pMap->iTilesHeight - pMap->iScrollY;
			Rect_dest.h = pMap->iTilesHeight;
			Rect_dest.w = pMap->iTilesWidth;
			if (i < 0 || i >= pMap->iNbTiles_WidthLevel || j < 0 || j >= pMap->iNbTiles_HeightLevel)
				iNumTile = 0;
			else
			{
				iNumTile = pMap->szSchema[i][j];
			}
			SDL_RenderCopy(pRenderer, pMap->pTextureTileset, &(pMap->tp_Props[iNumTile].R), &Rect_dest); // En arrière plan si la fonction Kr_Map_Draw est appelé au tout début

		}
	}
}


/*!
*  \fn     void Kr_Map_Free(Kr_Map* pMap)
*  \brief  Function free a Kr_Map structure

*  \param  pMap a pointer to the Kr_Map structure we want to free
*  \return None
*/
void Kr_Map_Free(Kr_Map *pMap)
{
	Sint32 i;
	UTIL_FreeTexture(&pMap->pTextureTileset);
	for (i = 0; i<pMap->iNbTiles_HeightLevel; i++)
		free(pMap->szSchema[i]);
	UTIL_Free(pMap->szSchema);
	UTIL_Free(pMap->tp_Props);
	UTIL_Free(pMap);
}

/*!
*  \fn     void Kr_Map_Free(Kr_Map* pMap, Kr_Input *inEvent)
*  \brief  Function to handle the scrolling

*  \param  pMap    a pointer to the Kr_Map structure
*  \param  inEvent a pointer to the Kr_Input structure
*  \return None
*/
void Kr_Map_UpdateScroll(Kr_Map *pMap, Kr_Input *inEvent)
{
	if (inEvent->szKey[SDL_SCANCODE_UP]) 
	{
		pMap->iScrollY -= MOVESPEED;

	}
	if (inEvent->szKey[SDL_SCANCODE_DOWN])
	{
		pMap->iScrollY += MOVESPEED;
	}
	if (inEvent->szKey[SDL_SCANCODE_LEFT])
	{
		pMap->iScrollX -= MOVESPEED;
	}
	if (inEvent->szKey[SDL_SCANCODE_RIGHT])
	{
		pMap->iScrollX += MOVESPEED;
	}
	// limitation
	
	if (pMap->iScrollX < 0)
	{
		Kr_Log_Print(KR_LOG_WARNING, " 1 Reach limit on X : %d\n", pMap->iScrollX);
		pMap->iScrollX = 0;
	}
	
	 if (pMap->iScrollY < 0)
	{
		 Kr_Log_Print(KR_LOG_WARNING, " 2 Reach limit on Y : %d\n", pMap->iScrollY);
		pMap->iScrollY = 0;
	}		
	 if (pMap->iScrollX > pMap->iNbTiles_WidthLevel *pMap->iTilesWidth - pMap->iWidth_MapWindow - 1)
	{
	Kr_Log_Print(KR_LOG_WARNING, " 3 Reach limit on X : %d\n", pMap->iScrollX);
	pMap->iScrollX = pMap->iNbTiles_WidthLevel *pMap->iTilesWidth - pMap->iWidth_MapWindow - 1;
	}
	 if (pMap->iScrollY > pMap->iNbTiles_HeightLevel*pMap->iTilesHeight - pMap->iHeight_MapWindow - 1)
	{ 
		Kr_Log_Print(KR_LOG_WARNING, " 4 Reach limit on Y : %d\n", pMap->iScrollY);
		pMap->iScrollY = pMap->iNbTiles_HeightLevel*pMap->iTilesHeight - pMap->iHeight_MapWindow - 1;
	}

	 if (pMap->iWidth_MapWindow >= pMap->iNbTiles_WidthLevel *pMap->iTilesWidth)
	{
		Kr_Log_Print(KR_LOG_WARNING, " 5 Reach limit on X : %d\n", pMap->iScrollX);
		pMap->iScrollX = 0;
	}

	 if (pMap->iHeight_MapWindow >= pMap->iNbTiles_HeightLevel*pMap->iTilesHeight)
	{
		Kr_Log_Print(KR_LOG_WARNING, " 6 Reach limit on Y : %d\n", pMap->iScrollY);
		pMap->iScrollY = 0;
	}
}



void Kr_Map_FocusScrollBox(Kr_Map *pMap, SDL_Rect *rectToFocus, SDL_Rect rectLimitScroll)
{
	pMap->rectToFocus = rectToFocus;
	pMap->rectLimitScroll.x = rectLimitScroll.x;
	pMap->rectLimitScroll.y = rectLimitScroll.y;
	pMap->rectLimitScroll.h = rectLimitScroll.h;
	pMap->rectLimitScroll.w = rectLimitScroll.w;
}

void Kr_Map_UpdateScroll2(Kr_Map *pMap)
{
	Sint32 iCenterXEntity, iCenterYEntity, iLimitXMin, iLimitXMax, iLimitYMin, iLimitYMax;

	//if (pMap->rectToFocus == NULL) // Sortir
	iCenterXEntity = pMap->rectToFocus->x + pMap->rectToFocus->w / 2;
	iCenterYEntity = pMap->rectToFocus->y + pMap->rectToFocus->h / 2;
	iLimitXMin = pMap->iScrollX + pMap->rectLimitScroll.x;
	iLimitYMin = pMap->iScrollY + pMap->rectLimitScroll.y;
	iLimitXMax = iLimitXMin + pMap->rectLimitScroll.w;
	iLimitYMax = iLimitYMin + pMap->rectLimitScroll.h;

	//Kr_Log_Print(KR_LOG_INFO, "ScrollX   : %d |ScrollY    : %d \n",pMap->iScrollX, pMap->iScrollY);
	//Kr_Log_Print(KR_LOG_INFO, "iLimitXMin: %d |iLimitXMax : %d \n", iLimitXMin, iLimitXMax);
	//Kr_Log_Print(KR_LOG_INFO, "iLimitYMin: %d |iLimitYMax : %d \n", iLimitYMin, iLimitYMax);
	//Kr_Log_Print(KR_LOG_INFO, "iCoordX   : %d |iCoordY    : %d \n", iCenterXEntity, iCenterYEntity);
	//Kr_Log_Print(KR_LOG_INFO, "\n");

	/* Gestion de la fenetre de scroll */
	if (iCenterXEntity < iLimitXMin)
	{
		pMap->iScrollX -= (iLimitXMin - iCenterXEntity);
	}
		
	if (iCenterYEntity < iLimitYMin)
	{
		pMap->iScrollY -= (iLimitYMin - iCenterYEntity);
	}
		
	if (iCenterXEntity > iLimitXMax)
	{
		pMap->iScrollX += (iCenterXEntity - iLimitXMax);
	}
		
	if (iCenterYEntity > iLimitYMax)
	{
		pMap->iScrollY += (iCenterYEntity - iLimitYMax);
	}

	Kr_Map_ClampScroll(pMap);
}

void Kr_Map_ClampScroll(Kr_Map *pMap)
{
	if (pMap->iScrollX < 0)
	{
		Kr_Log_Print(KR_LOG_WARNING, " 1 Reach limit on X : %d\n", pMap->iScrollX);
		pMap->iScrollX = 0;
	}
	if (pMap->iScrollY < 0)
	{
		Kr_Log_Print(KR_LOG_WARNING, " 2 Reach limit on Y : %d\n", pMap->iScrollY);
		pMap->iScrollY = 0;
	}
	if (pMap->iScrollX > pMap->iNbTiles_WidthLevel *pMap->iTilesWidth - pMap->iWidth_MapWindow - 1)
	{
		Kr_Log_Print(KR_LOG_WARNING, " 3 Reach limit on X : %d\n", pMap->iScrollX);
		pMap->iScrollX = pMap->iNbTiles_WidthLevel *pMap->iTilesWidth - pMap->iWidth_MapWindow - 1;
	}
	if (pMap->iScrollY > pMap->iNbTiles_HeightLevel*pMap->iTilesHeight - pMap->iHeight_MapWindow - 1)
	{
		Kr_Log_Print(KR_LOG_WARNING, " 4 Reach limit on Y : %d\n", pMap->iScrollY);
		pMap->iScrollY = pMap->iNbTiles_HeightLevel*pMap->iTilesHeight - pMap->iHeight_MapWindow - 1;
	}
	//
	if (pMap->iWidth_MapWindow >= pMap->iNbTiles_WidthLevel *pMap->iTilesWidth)
	{
		Kr_Log_Print(KR_LOG_WARNING, " 5 Reach limit on X : %d\n", pMap->iScrollX);
		pMap->iScrollX = 0;
	}

	if (pMap->iHeight_MapWindow >= pMap->iNbTiles_HeightLevel*pMap->iTilesHeight)
	{
		Kr_Log_Print(KR_LOG_WARNING, " 6 Reach limit on Y : %d\n", pMap->iScrollY);
		pMap->iScrollY = 0;
	}
}
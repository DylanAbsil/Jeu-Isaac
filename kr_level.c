/* ========================================================================= */
/*!
* \file    kr_level.c
* \brief   Contains the functions to handle the level.
* \author  Olivier Herrou
* \version 1.0
* \date    01 Mars 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 01/03/2015 | Création                                     */
/* ========================================================================= */

#include "kr_level.h"

#define CACHE_SIZE 5000

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

	pLevel->rToFocus = NULL;

	pLevel->rLimitation = NULL;

	pLevel->rScrollWindow.x = 0;
	pLevel->rScrollWindow.y = 0;
	pLevel->rScrollWindow.h = 0;
	pLevel->rScrollWindow.w = 0;

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
	Kr_Log_Print(KR_LOG_INFO, "Opening maps file %s\n", szLevelPath);
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
	Kr_Log_Print(KR_LOG_INFO, "%d %d !\n", pLevel->iLevel_TileHeight, pLevel->pLevel_Tileset->iTilesHeight);
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
	pLevel->szLayout = malloc(pLevel->iLevel_TileWidth*sizeof(unsigned char*));
	for (i = 0; i<pLevel->iLevel_TileWidth; i++)
		pLevel->szLayout[i] = malloc(pLevel->iLevel_TileHeight*sizeof(unsigned char));

	/* Affectation des données level au schema */
	for (j = 0; j<pLevel->iLevel_TileHeight; j++)
	{
		for (i = 0; i<pLevel->iLevel_TileWidth; i++)
		{
			fscanf(pFile, "%d", &iTmp); 
			if (iTmp >= pLevel->pLevel_Tileset->iNbTilesX * pLevel->pLevel_Tileset->iNbTilesY) // On vérifie que le codage du tile existe
			{
				Kr_Log_Print(KR_LOG_ERROR, "Can't load a tile in the layout because it's out of the tileset file ");
				Kr_Log_Print(KR_LOG_ERROR, ">> ile n°\"%d\", but iNbTilesX = %d and iNbTilesY = %d", iTmp, pLevel->pLevel_Tileset->iNbTilesX, pLevel->pLevel_Tileset->iNbTilesY);
				return FALSE;
			}
			pLevel->szLayout[i][j] = iTmp;
			//Kr_Log_Print(KR_LOG_INFO, "pMap->schema[%d][%d] = %d \n", i, j, pMap->szSchema[i][j]);
		}
	}
	Kr_Log_Print(KR_LOG_INFO, "Kr_Level_Layout: Done\n");
	return TRUE;
}




/*!
*  \fn     void Kr_Level_Draw(SDL_Renderer *pRenderer, Kr_Level *pLevel, SDL_Rect rDst)
*  \brief  Function to draw a level on a renderer
*
*  \param  pRenderer a pointer to the renderer
*  \param  pLevel    a pointer to a the level structure
*  \param  rDst      the rectangle where the level must be draw
*  \return none
*/
void Kr_Level_Draw(SDL_Renderer *pRenderer, Kr_Level *pLevel, SDL_Rect *rDst)
{
	Sint32   i, j;
	SDL_Rect Rect_dest;
	Sint32   iNumTile;
	Sint32   iMinX, iMaxX, iMinY, iMaxY;
	Kr_Level_UpdateScroll(pLevel, rDst);

	iMinX =  pLevel->rScrollWindow.x / pLevel->pLevel_Tileset->iTilesWidth;// -1;
	iMinY =  pLevel->rScrollWindow.y / pLevel->pLevel_Tileset->iTilesHeight;// -1;
	iMaxX = (pLevel->rScrollWindow.x + rDst->w) / pLevel->pLevel_Tileset->iTilesWidth;
	iMaxY = (pLevel->rScrollWindow.y + rDst->h) / pLevel->pLevel_Tileset->iTilesHeight;
	//Kr_Log_Print(KR_LOG_WARNING, " Scroll W: %d\n", pLevel->rScrollWindow.w);
	//Kr_Log_Print(KR_LOG_WARNING, " Scroll H: %d\n", pLevel->rScrollWindow.h);
	//Kr_Log_Print(KR_LOG_WARNING, " Limitation x: %d\n", pLevel->rLimitation.x);
	//Kr_Log_Print(KR_LOG_WARNING, " Limitation y: %d\n", pLevel->rLimitation.y);
	for (i = iMinX; i <= iMaxX; i++)
	{
		for (j = iMinY; j <= iMaxY; j++)
		{
			Rect_dest.x =  + i*pLevel->pLevel_Tileset->iTilesWidth - pLevel->rScrollWindow.x;
			Rect_dest.y =  + j*pLevel->pLevel_Tileset->iTilesHeight - pLevel->rScrollWindow.y;
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
*  \fn     void Kr_Level_Focus(Kr_Level *pLevel, SDL_Rect *rToFocus, SDL_Rect *rLimitation)
*  \brief  Function to initialize the scrolling data
*
*  \param  pLevel        a pointer to a the level structure
*  \param  rToFocus      the rectangle which will be focus for the scrolling
*  \param  rLimitation   the limitation rectangle 
*  \return none
*/
void Kr_Level_Focus(Kr_Level *pLevel, SDL_Rect *rToFocus, SDL_Rect *rLimitation)
{
	pLevel->rLimitation = rLimitation;
	pLevel->rToFocus = rToFocus;

}

/*!
*  \fn     void Kr_Level_UpdateScroll(Kr_Level *pLevel, SDL_Rect *rDst)
*  \brief  Function to update the scrolling window
*
*  \param  pLevel a pointer to a the level structure
*  \param  rDst      the rectangle where the level must be draw
*  \return none
*/
void Kr_Level_UpdateScroll(Kr_Level *pLevel, SDL_Rect *rDst)
{
	Sint32 iCenterXEntity, iCenterYEntity, iLimitXMin, iLimitXMax, iLimitYMin, iLimitYMax;

	
	iCenterXEntity = pLevel->rToFocus->x + pLevel->rToFocus->w / 2;
	iCenterYEntity = pLevel->rToFocus->y + pLevel->rToFocus->h / 2;

	

	iLimitXMin = pLevel->rScrollWindow.x + pLevel->rLimitation->x;
	iLimitYMin = pLevel->rScrollWindow.y + pLevel->rLimitation->y;
	iLimitXMax = iLimitXMin + pLevel->rLimitation->w;
	iLimitYMax = iLimitYMin + pLevel->rLimitation->h;

	//	Kr_Log_Print(KR_LOG_INFO, "ScrollX   : %d |ScrollY     : %d \n", pLevel->rScrollWindow.x, pLevel->rScrollWindow.y);
	//	Kr_Log_Print(KR_LOG_INFO, "LimitX    : %d |LimitY     : %d \n", pLevel->rLimitation.x, pLevel->rLimitation.y);
	//	Kr_Log_Print(KR_LOG_INFO, "X         : %d |Y          : %d \n", pLevel->rToFocus->x, pLevel->rToFocus->y);
	//	Kr_Log_Print(KR_LOG_INFO, "iLimitXMin: %d |iLimitXMax : %d \n", iLimitXMin, iLimitXMax);
	//	Kr_Log_Print(KR_LOG_INFO, "iLimitYMin: %d |iLimitYMax : %d \n", iLimitYMin, iLimitYMax);
 	//	Kr_Log_Print(KR_LOG_INFO, "iCoordX   : %d |iCoordY    : %d \n", iCenterXEntity, iCenterYEntity);
	//	Kr_Log_Print(KR_LOG_INFO, "\n");

	/* Gestion de la fenetre de scroll */
	if (iCenterXEntity < iLimitXMin)
	{
		pLevel->rScrollWindow.x -= (iLimitXMin - iCenterXEntity);
	}
	if (iCenterYEntity < iLimitYMin)
	{
		pLevel->rScrollWindow.y -= (iLimitYMin - iCenterYEntity);
	}

	if (iCenterXEntity > iLimitXMax)
	{
		pLevel->rScrollWindow.x += (iCenterXEntity - iLimitXMax);
	}

	if (iCenterYEntity > iLimitYMax)
	{
		pLevel->rScrollWindow.y += (iCenterYEntity - iLimitYMax);
	}
	Kr_Level_ClampScroll(pLevel, rDst);
}

/*!
*  \fn     void Kr_Level_Free(Kr_Level *pLevel, SDL_Rect *rDst)
*  \brief  Function to handle the collision of the scrolling window with the edges of the level
*
*  \param  pLevel a pointer to a the level structure
*  \param  rDst      the rectangle where the level must be draw
*  \return none
*/
void Kr_Level_ClampScroll(Kr_Level *pLevel, SDL_Rect *rDst)
{

	if (pLevel->rScrollWindow.x + pLevel->rToFocus->x  < 0)
	{
		Kr_Log_Print(KR_LOG_WARNING, " 1 Reach limit on X : %d\n", pLevel->rScrollWindow.x);
		pLevel->rScrollWindow.x = 0;
		pLevel->rToFocus->x = 0;
	}
	if (pLevel->rScrollWindow.y + pLevel->rToFocus->y < 0)
	{
		Kr_Log_Print(KR_LOG_WARNING, " 2 Reach limit on Y : %d\n", pLevel->rScrollWindow.y);
		pLevel->rScrollWindow.y = 0;
		pLevel->rToFocus->y = 0;
	}

	if (pLevel->rScrollWindow.x > pLevel->iLevel_TileWidth * pLevel->pLevel_Tileset->iTilesWidth - rDst->w - 1)
	{
		Kr_Log_Print(KR_LOG_WARNING, " 3 Reach limit on X : %d\n", pLevel->rScrollWindow.x);
		pLevel->rScrollWindow.x = pLevel->iLevel_TileWidth * pLevel->pLevel_Tileset->iTilesWidth - rDst->w - 1;
	}
	if (pLevel->rScrollWindow.y > pLevel->iLevel_TileHeight* pLevel->pLevel_Tileset->iTilesHeight - rDst->h- 1)
	{
		//Kr_Log_Print(KR_LOG_WARNING, " 4 Reach limit on Y : %d\n", pLevel->rScrollWindow.y);
		pLevel->rScrollWindow.y = pLevel->iLevel_TileHeight * pLevel->pLevel_Tileset->iTilesHeight - rDst->h - 1;
	
	}
	//
	if (pLevel->rScrollWindow.x + pLevel->rToFocus->x + pLevel->rToFocus->w >= pLevel->iLevel_TileWidth* pLevel->pLevel_Tileset->iTilesWidth - 1) // Faux car tout est constant
	{
		Kr_Log_Print(KR_LOG_WARNING, " 5 Reach limit on X : %d\n", pLevel->rToFocus->x);
		pLevel->rToFocus->x = pLevel->iLevel_TileWidth* pLevel->pLevel_Tileset->iTilesWidth - pLevel->rScrollWindow.x - pLevel->rToFocus->w - 1;
	}

	if (pLevel->rScrollWindow.y + pLevel->rToFocus->y + pLevel->rToFocus->h >= pLevel->iLevel_TileHeight* pLevel->pLevel_Tileset->iTilesHeight - 1)
	{
		Kr_Log_Print(KR_LOG_WARNING, " 6 Reach limit on Y : %d\n", pLevel->rToFocus->y);
		pLevel->rToFocus->y = pLevel->iLevel_TileHeight* pLevel->pLevel_Tileset->iTilesHeight - pLevel->rScrollWindow.y - pLevel->rToFocus->h - 1;
	}
	
//	Kr_Log_Print(KR_LOG_WARNING, " RLimit X : %d   Y: %d\n", pLevel->rLimitation->x,pLevel->rLimitation->y);
}

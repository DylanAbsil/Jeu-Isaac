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
/* Herrou        | 08/03/2015 | Gestion du Scrolling en cours                */
/* Herrou        | 18/03/2015 | Arret du scrolling, map en 40x22 en tiles de 32*/
/* ========================================================================= */
/*
 Commentaire :
  - L'affichage de la carte fonctionne correctement, cependant si on ne l'affiche pas dans le renderer complet (rDst != renderer) 
    alors on aura l'impression que la fenêtre se déplace dans le renderer mais c'est simplement car on affiche que des blocs de 32 et non pas pixel par pixel


*/
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
				Kr_Log_Print(KR_LOG_ERROR, ">> Tile n°\"%d\", but iNbTilesX = %d and iNbTilesY = %d", iTmp, pLevel->pLevel_Tileset->iNbTilesX, pLevel->pLevel_Tileset->iNbTilesY);
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
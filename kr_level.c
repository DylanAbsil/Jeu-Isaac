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
/* Herrou        | 04/04/2015 | Initialisation du nom faite par UTIL_CopyStr										*/
/*               |            | Mise à jour des fonctions au vue des modifications de la structure Kr_Level 		*/		
/*               |            | Création de la fonction Kr_Level_GetLevelNumber  									*/
/* Herrou        | 05/04/2015 | Initialisation des pLevel->iNumXXXX													*/
/*               |            | Suppression de la fonction Kr_Level_Event, qui est désormais dans Kr_Map			*/
/*               |            | Suppression de Kr_GetLevelNumber													*/
/*               |            | Kr_Level_Change, prend en paramètre le numéro du level et non pas son nom			*/
/* Herrou        | 20/04/2015 | Transfert des fonctions SaveLayout et WriteLayout dans Kr_Level						*/
/* Herrou        | 22/04/2015 | Gestion des collisions effectués dans kr_collision									*/
/* Herrou        | 27/04/2015 | Mise à jour de l'initialisation de la structure pour szLevelMessage					*/
/* Herrou        | 27/04/2015 | Passage en Level Version 1.4 et ajout de la musique du level						*/
/* ===============================================================================================================  */

/*
Commentaire :


*/
#include "kr_level.h"

#define CACHE_SIZE 15000

/*!
*  \fn     Kr_Tileset *Kr_Level_Init(char *szFileName)
*  \brief  Function to initialize a Level thanks to the level file
*
*  \param  szFileName the name of the level file
*  \return the initialize structure
*/
Kr_Level *Kr_Level_Init(char *szFileName)
{
	Kr_Level *pLevel = NULL;
	Uint32 iNameLen = strlen(szFileName);
	pLevel = (Kr_Level *)UTIL_Malloc(sizeof(Kr_Level));

	pLevel->szLevelFile = UTIL_CopyStr(szFileName, iNameLen);
	pLevel->pMusic = Kr_Sound_InitMusic();
	pLevel->szLevelName = NULL;	
	pLevel->szLevelMessage = NULL;
	
	pLevel->iLevelNum = -1;       
	pLevel->iLevel_TileWidth = 0;
	pLevel->iLevel_TileHeight = 0;
	pLevel->pLevel_Tileset = NULL;
	pLevel->szLayout = NULL;
	pLevel->iNumEst = 0;
	pLevel->iNumNord = 0;
	pLevel->iNumOuest = 0;
	pLevel->iNumSud = 0;
	return pLevel;
}


/*!
*  \fn     Boolean Kr_Level_Load(Kr_Level *pLevel, Kr_Map *pMap, SDL_Renderer *pRenderer)
*  \brief  Function to load a Kr_Level structure via a level file
*
*  \param  pMap      a pointer to the map
*  \param  pLevel    a pointer to a the level structure
*  \param  pRenderer a pointer to the renderer
*  \return TRUE if everything is ok, NULL otherwise
*/
Boolean   Kr_Level_Load(Kr_Level *pLevel,  SDL_Renderer *pRenderer)
{
	char   szBuf[CACHE_SIZE];  // Buffer
	char   szBuf2[CACHE_SIZE]; // Buffer2
	char   szLevelPath[50];
	FILE  *pFile;
	Uint32 iNameLen = 0;

	/* Ouverture du fichier level */
	sprintf(szLevelPath, "maps\\%s.txt", pLevel->szLevelFile);
	Kr_Log_Print(KR_LOG_INFO, "Opening level file %s\n", szLevelPath);
	pFile = UTIL_OpenFile(szLevelPath, "r"); // Ouverture du level en read
	if (!pFile) return FALSE;

	/* Vérification de la version du fichier level */
	fgets(szBuf, CACHE_SIZE, pFile);
	if (strstr(szBuf, KR_LEVEL_VERSION) == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Wrong version of the level file, expected :%s\n"KR_LEVEL_VERSION);
		Kr_Log_Print(KR_LOG_ERROR, ">> Level \"%s\".\n", szLevelPath);
		return FALSE;
	}

	/* Recherche du numéro du level*/
	strcpy(szBuf2, pLevel->szLevelFile);
	iNameLen = strlen(szBuf2);
	UTIL_SousChaine(szBuf2, 5, iNameLen, szBuf2); // 5 correspond à "level", on veut extraire le numéro qui est après
	pLevel->iLevelNum = UTIL_StrToUint32(szBuf2);


	do // Lecture ligne par ligne du fichier
	{
		fgets(szBuf, CACHE_SIZE, pFile);
		if (strstr(szBuf, "#property")) // Identification de la ligne property
		{
			fgets(szBuf2, CACHE_SIZE, pFile); // Lecture de la ligne suivante qui indique le nom du level
			szBuf2[strcspn(szBuf2, "\n")] = '\0'; //retirer \n
			iNameLen = strlen(szBuf2) - 1;      // Il faut retirer 1 car il ne faut pas envoyer à UTIL_CopyStr \0
			pLevel->szLevelName = UTIL_CopyStr(szBuf2, iNameLen);

			fgets(szBuf2, CACHE_SIZE, pFile); // Lecture de la ligne suivante qui indique le message
			szBuf2[strcspn(szBuf2, "\n")] = '\0'; 
			iNameLen = strlen(szBuf2) - 1;     
			pLevel->szLevelMessage = UTIL_CopyStr(szBuf2, iNameLen);

			fgets(szBuf2, CACHE_SIZE, pFile); // Lecture de la ligne suivante qui indique la musique du level
			szBuf2[strcspn(szBuf2, "\n")] = '\0';
			iNameLen = strlen(szBuf2) - 1;
			pLevel->pMusic->szMscName = UTIL_CopyStr(szBuf2, iNameLen);
			pLevel->pMusic->pMsc = NULL;
			if (strcmp(szBuf2, "none") != 0) 
			{
				Kr_Sound_LoadMusic(pLevel->pMusic, szBuf2);
			}
		}
		if (strstr(szBuf, "#tileset")) // Identification de la ligne tileset
		{
			fscanf(pFile, "%s", szBuf2); // Lecture de la ligne suivante qui indique le fichier .tls

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
	//Kr_Map_GetNeighbor(pMap, pLevel, &pLevel->iNumNord, &pLevel->iNumSud, &pLevel->iNumEst, &pLevel->iNumOuest);
	Kr_Log_Print(KR_LOG_INFO, "tiles %d %d !\n", pLevel->iLevel_TileHeight, pLevel->pLevel_Tileset->iTilesHeight);
	Kr_Log_Print(KR_LOG_INFO, "Level : %s has been loaded !\n", pLevel->szLevelFile);
	return TRUE;
}


/*!
*  \fn     void Kr_Level_Free(Kr_Level *pLevel)
*  \brief  Function to free a Kr_Level structure
*
*  \param  pLevel     a pointer to a the level structure
*  \return none
*/
void Kr_Level_Free(Kr_Level *pLevel)
{
	Sint32 i;
	if (pLevel == NULL) return;
	Kr_Tileset_Free(pLevel->pLevel_Tileset);
	for (i = 0; i< pLevel->iLevel_TileHeight; i++)
		free(pLevel->szLayout[i]);
	Kr_Sound_FreeMusic(pLevel->pMusic);

	UTIL_Free(pLevel->szLayout);
	UTIL_Free(pLevel->szLevelFile);
	//UTIL_Free(pLevel->szLevelName);
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
	if ((x >= (Uint32)(pLevel->iLevel_TileWidth * pLevel->pLevel_Tileset->iTilesWidth)) || (y >= (Uint32)(pLevel->iLevel_TileHeight * pLevel->pLevel_Tileset->iTilesHeight)))
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
*  \fn     Kr_Level *Kr_Level_Change(Kr_Level *pCurrentLevel, Kr_Map *pMap, char* szLevelName, SDL_Renderer *pRenderer)
*  \brief  Function to change the level
*
*  \param  pMap           a pointer to the map
*  \param  pCurrentLevel  a pointer to the current Level which must be freed
*  \param  szLevelName    the name of the new Level to load
*  \param  pRenderer      a pointer to the Renderer
*  \return the initialised level structure, NULL otherwise
*/
Kr_Level *Kr_Level_Change(Kr_Level *pCurrentLevel, Uint32 iCurrentLevelNumber, SDL_Renderer *pRenderer)
{
	char szLevelName[20] = "";
    sprintf(szLevelName, "level%d", iCurrentLevelNumber);
	Kr_Level_Free(pCurrentLevel);
	Kr_Level *pNewLevel = Kr_Level_Init(szLevelName);
	if (!Kr_Level_Load(pNewLevel, pRenderer))
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't Load a level\n");
		return NULL;
	}
	return pNewLevel;
}



/*!
*  \fn     void Kr_Level_WriteLayout(Level_Editor *pEditor, Uint32 iNumTile, Uint32 x, Uint32 y)
*  \brief  Function to rewrite in the layout of the level
*
*  \param  pLevel         a pointer to the level
*  \param  iNumTile       the number of the tile we want to draw
*  \param  x			  coordinate
*  \param  y			  coordinate
*  \return none
*/
void Kr_Level_WriteLayout(Kr_Level *pLevel, Uint32 iNumTile, Uint32 x, Uint32 y)
{
	Sint32 iNumTilesX, iNumTilesY;

	iNumTilesX = x / pLevel->pLevel_Tileset->iTilesWidth;
	iNumTilesY = y / pLevel->pLevel_Tileset->iTilesHeight;

	if (iNumTilesX >= pLevel->iLevel_TileWidth || iNumTilesY >= pLevel->iLevel_TileHeight) return; // On vérifie que l'on est bien sur la carte

	pLevel->szLayout[iNumTilesX][iNumTilesY] = iNumTile;
}


/*!
*  \fn     Boolean Kr_Level_SaveLayout(Kr_Level *pLevel)
*  \brief  Function to save the layout of the level
*
*  \param  pLevel    a pointer to the level
*  \return TRUE if everything is ok, FALSE otherwise
*/
Boolean Kr_Level_SaveLayout(Kr_Level *pLevel)
{
	char   szPath1[50];
	char   szPath2[50];
	FILE  *pFileSrc;
	FILE  *pFileDst;
	Sint32 i, j;

	Kr_Log_Print(KR_LOG_INFO, "Saving the Layout !\n");
	/* Ouverture du fichier temporaire*/
	sprintf(szPath1, "maps\\level%d.tmp", pLevel->iLevelNum);
	pFileDst = UTIL_OpenFile(szPath1, "w"); //écriture
	if (!pFileDst) return FALSE;


	/* Ouverture du fichier level */
	sprintf(szPath2, "maps\\level%d.txt", pLevel->iLevelNum);
	Kr_Log_Print(KR_LOG_INFO, "Opening level file %s\n", szPath2);
	pFileSrc = UTIL_OpenFile(szPath2, "r"); //Lecture 
	if (!pFileDst)
	{
		UTIL_CloseFile(&pFileSrc);
		return FALSE;
	}

	if (!UTIL_FileCopy(pFileSrc, pFileDst, "#layout")) return FALSE; // copie de la partie précédent le layout
	UTIL_CloseFile(&pFileSrc);
	UTIL_CloseFile(&pFileDst);
	if (remove(szPath2)) Kr_Log_Print(KR_LOG_ERROR, "Failed to delete %s !\n", szPath1);
	if (rename(szPath1, szPath2)) Kr_Log_Print(KR_LOG_ERROR, "Failed to rename %s to %s !\n", szPath1, szPath2);


	/* Ouverture du fichier temporaire*/
	sprintf(szPath1, "maps\\level%d.txt", pLevel->iLevelNum);
	Kr_Log_Print(KR_LOG_INFO, "Opening level file %s\n", szPath2);
	pFileSrc = UTIL_OpenFile(szPath1, "r+");
	if (!pFileSrc) return FALSE;
	fseek(pFileSrc, 0, SEEK_END);
	fprintf(pFileSrc, "%d %d\n", pLevel->iLevel_TileWidth, pLevel->iLevel_TileHeight);
	// Remplissage du nouveau Layout
	for (j = 0; j< pLevel->iLevel_TileHeight; j++)
	{
		for (i = 0; i< pLevel->iLevel_TileWidth; i++)
		{
			fprintf(pFileSrc, "%d ", pLevel->szLayout[i][j]);
		}
		fprintf(pFileSrc, "\n");
	}
	fprintf(pFileSrc, "#end");
	UTIL_CloseFile(&pFileSrc);
	return TRUE;
}

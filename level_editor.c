/* ========================================================================= */
/*!
* \file    level_editor.c
* \brief   Contains the functions to handle the level editor.
* \author  Olivier Herrou
* \version 1.0
* \date    06 Avril 2015
*/
/* ================================================================================================================ */
/* Developers    | Date       | Comments																			*/
/* --------------+------------+------------------------------------------------------------------------------------ */
/* Herrou        | 06/04/2015 | Création																			*/
/*               |            |         																			*/
/*               |            |         																			*/
/*               |            |         																			*/
/*               |            |         																			*/
/*               |            |         																			*/
/*               |            |         																			*/
/* ===============================================================================================================  */


#include "level_editor.h"
#define CACHE_SIZE 200

/*!
*  \fn     Level_Editor *Level_Editor_Init(char *szEditorFile)
*  \brief  Function to initialize a Level Editor structure thanks to the level editor file
*
*  \param  szEditorFile the name of the level editor file
*  \return the initialize structure
*/
Level_Editor *Level_Editor_Init(char *szEditorFile)
{
	Level_Editor *pEditor= NULL;

	Uint32 iNameLen = strlen(szEditorFile);
	pEditor = (Level_Editor *)UTIL_Malloc(sizeof(Level_Editor));

	pEditor->szEditorFile	= UTIL_CopyStr(szEditorFile, iNameLen);
	pEditor->iStandardTile	= 0;
	pEditor->pLevel			= NULL;
	
	return pEditor;
}


/*!
*  \fn     Level_Editor_Load(Level_Editor *pEditor, SDL_Renderer *pRenderer);
*  \brief  Function to load a Level Editor structure thanks to the level editor file
*
*  \Remarks : Le numéro du niveau doit être compris entre 1 et 999
*
*  \param  pEditor a pointer to the level editor structure
*  \param  pRenderer    a pointer to the renderer
*  \return TRUE if everything is OK, FALSE otherwise
*/
Boolean	Level_Editor_Load(Level_Editor *pEditor, SDL_Renderer *pRenderer)
{
	Uint32 iNameLen = 0;
	char   szPath[50];
	char   szLevelFile[50];
	FILE  *pFile;
	Uint32 iTmp = 0;

	/* Ouverture du fichier level_editor */
	sprintf(szPath, "maps\\%s.txt", pEditor->szEditorFile);
	Kr_Log_Print(KR_LOG_INFO, "Opening level editor file %s\n", szPath);
	pFile = UTIL_OpenFile(szPath, "r"); 
	if (!pFile) return FALSE;
	
// Lecture ligne par ligne du fichier

	// Numero du level
	fscanf(pFile, "%d", &iTmp);
	if (iTmp < 1 || iTmp > 999)
	{
		return FALSE;
	}
	UTIL_CloseFile(&pFile);
	// Recherche du fichier dans le répertoire /maps pour savoir si on continue de modifier un level existant ou si on en créé un nouveau
	sprintf(szLevelFile, "level%d", iTmp);
	/* Ouverture du fichier level */
	sprintf(szPath, "maps\\%s.txt", szLevelFile);
	Kr_Log_Print(KR_LOG_INFO, "Trying to open the level %s\n", szPath);
	pFile = UTIL_OpenFile(szPath, "r");
	if (pFile)
	{
		Kr_Log_Print(KR_LOG_INFO, "The level already exist, loading %s for modification !\n", szPath);
		UTIL_CloseFile(&pFile);
		/*
		
		  ICI ENVOYER VERS LA FONCTION QUI GERE DIRECTEMENT LES MODIFICATIONS DES LEVELS
		  IL FAUT CEPENDANT CHARGER LE LEVEL AVANT !

		*/
		return TRUE;
	}
	UTIL_CloseFile(&pFile);
	Kr_Log_Print(KR_LOG_INFO, "The level %s does not exist !\n", szPath);
	Kr_Log_Print(KR_LOG_INFO, "Creating the level %s !\n", szPath);
	Level_Editor_LoadLevel(pEditor, szLevelFile, pRenderer);
	




	return TRUE;
}


/*!
*  \fn     void Level_Editor_Free(Level_Editor *pEditor)
*  \brief  Function to free the Level Editor structure
*
*  \param  pEditor a pointer to the level editor structure
*  \return none
*/
void Level_Editor_Free(Level_Editor *pEditor)
{

}


/*!
*  \fn     void Level_Editor_Log(Level_Editor *pEditor)
*  \brief  Function to log the Level Editor structure
*
*  \param  pEditor a pointer to the level editor structure
*  \return none
*/
void Level_Editor_Log(Level_Editor *pEditor)
{
	Kr_Log_Print(KR_LOG_INFO, "Level Editor Log\n\n");
	Kr_Log_Print(KR_LOG_INFO, "=============================\n");
	Kr_Log_Print(KR_LOG_INFO, "Level Editor file : %s\n",pEditor->szEditorFile);
	Kr_Log_Print(KR_LOG_INFO, "Standard Tile : %d\n", pEditor->iStandardTile);
	Kr_Log_Print(KR_LOG_INFO, "End of log \n");
	Kr_Log_Print(KR_LOG_INFO, "=============================\n");
}


/*!
*  \fn     Boolean	Level_Editor_LoadLevel(Level_Editor *pEditor, char *szLevelFile, SDL_Renderer *pRenderer)
*  \brief  Function to create a LevelFile from a pEditor structure
*
*  \param  pEditor		a pointer to the level editor structure
*  \param  szLevelFile	the name of the level file
*  \param  pRenderer    a pointer to the renderer
*  \return TRUE if everything is OK, FALSE otherwise
*/
Boolean	Level_Editor_LoadLevel(Level_Editor *pEditor, char *szLevelFile, SDL_Renderer *pRenderer)
{
	Uint32 iNameLen = 0;
	char   szBuf[CACHE_SIZE];  // Buffer
	char   szPath[50];
	FILE  *pFile;
	Uint32 iNameLen = 0;
	Uint32 iTmp = 0;
	Uint32 i, j;

	/* Ouverture du fichier level_editor */
	sprintf(szPath, "maps\\%s.txt", pEditor->szEditorFile);
	Kr_Log_Print(KR_LOG_INFO, "Opening level editor file %s\n", szPath);
	pFile = UTIL_OpenFile(szPath, "r"); 
	if (!pFile) return FALSE;

	// Initialisation du level
	pEditor->pLevel = Kr_Level_Init(szLevelFile);

	// Numero du level
	fscanf(pFile, "%d", &pEditor->pLevel->iLevelNum);

	// Nom du level
	fgets(szBuf, CACHE_SIZE, pFile);
	szBuf[strcspn(szBuf, "\n")] = '\0'; //retirer \n
	iNameLen = strlen(szBuf) - 1;      // Il faut retirer 1 car il ne faut pas envoyer à UTIL_CopyStr \0
	pEditor->pLevel->szLevelName = UTIL_CopyStr(szBuf, iNameLen);

	// Chargement du tileset
	fgets(szBuf, CACHE_SIZE, pFile);
	szBuf[strcspn(szBuf, "\n")] = '\0'; //retirer \n
	iNameLen = strlen(szBuf) - 1;      // Il faut retirer 1 car il ne faut pas envoyer à UTIL_CopyStr \0
	pEditor->pLevel->pLevel_Tileset = Kr_Tileset_Init(szBuf);
	if (!Kr_Tileset_Load(pEditor->pLevel->pLevel_Tileset, pRenderer))
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't load \"%s\" in the level, must quit !\n", szBuf);
		return FALSE;
	}	
	
	//Width and Height and standard tile
	fscanf(pFile, "%d", &pEditor->pLevel->iLevel_TileWidth);
	fscanf(pFile, "%d", &pEditor->pLevel->iLevel_TileHeight);
	fscanf(pFile, "%d", &pEditor->iStandardTile);
	UTIL_CloseFile(&pFile);
	// Controle
	if (pEditor->iStandardTile < 0)
	{
		Kr_Log_Print(KR_LOG_ERROR, "The standard Tile is not correct ! %d\n", pEditor->iStandardTile);
		return FALSE; // L'id d'une Tile est positif
	}
	if ((pEditor->pLevel->iLevel_TileWidth * pEditor->pLevel->pLevel_Tileset->iTilesWidth > KR_WIDTH_WINDOW) ||
		(pEditor->pLevel->iLevel_TileHeight * pEditor->pLevel->pLevel_Tileset->iTilesHeight > KR_HEIGHT_WINDOW))
	{
		Kr_Log_Print(KR_LOG_ERROR, "The level is too big max is %d*%d not %d*%d !\n", KR_WIDTH_WINDOW, KR_HEIGHT_WINDOW, 
		pEditor->pLevel->iLevel_TileWidth * pEditor->pLevel->pLevel_Tileset->iTilesWidth, pEditor->pLevel->iLevel_TileHeight * pEditor->pLevel->pLevel_Tileset->iTilesHeight);
		return FALSE; // Dimension de la carte trop grande, limité à KR_WIDTH_WINDOW et KR_HEIGHT_WINDOW
	}

	/*Affectation des données level au schema */
	for (j = 0; j<pEditor->pLevel->iLevel_TileHeight; j++)
	{
		for (i = 0; i<pEditor->pLevel->iLevel_TileWidth; i++)
		{
			pEditor->pLevel->szLayout[i][j] = pEditor->iStandardTile;
		}
	}
	Kr_Log_Print(KR_LOG_INFO, "Kr_Level_Layout: Done\n");
	Kr_Log_Print(KR_LOG_INFO, "Creating the level file with the loaded data !\n");
	return TRUE;
	
}


/*!
*  \fn     Boolean	Level_Editor_CreateLevelFile(Kr_Level *pLevel)
*  \brief  Function to create a LevelFile from a pEditor structure
*
*  \param  pLevel	a pointer to the level structure
*  \return TRUE if everything is OK, FALSE otherwise
*/
Boolean	Level_Editor_CreateLevelFile(Kr_Level *pLevel)
{
	return TRUE;
}






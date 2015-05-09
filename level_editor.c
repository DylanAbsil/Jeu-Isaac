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
/* Herrou        | 07/04/2015 | Add Level_Editor_GetTile        													*/
/* Herrou        | 09/04/2015 | Sauvegarde des données, gestion de la sélection de groupe de tiles					*/
/* Herrou        | 10/04/2015 | Ajout de la fonction Editor		 													*/
/* Herrou        | 14/04/2015 | MAJ fonction createFile, on créer deux fois #entity									*/
/* Herrou        | 20/04/2015 | MAJ fonction pour prendre en paramètre le level plutot que le Level_Editor dans certain cas*/
/* Herrou        | 20/04/2015 | Transfert des fonctions SaveLayout et WriteLayout dans Kr_Level						*/
/* Herrou        | 22/04/2015 | Renommer le fichier level_editor.txt en editor.txt									*/
/* Herrou        | 27/04/2015 | Mise à jour pour Level Version 1.3 : Ajout des messages								*/
/*               |            | Mise à jour pour Level Version 1.4 : Ajout de la musique du niveau					*/
/*               |            |																						*/
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
	if (pEditor == NULL) return NULL;
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
		pEditor->pLevel = Kr_Level_Init(szLevelFile);
		if (pEditor->pLevel == NULL)
		{
			Kr_Log_Print(KR_LOG_ERROR, "Can't initialize the level\n", szLevelFile);
			return FALSE;
		}
		if (!Kr_Level_Load(pEditor->pLevel, pRenderer))
		{
			Kr_Log_Print(KR_LOG_ERROR, "Can't load the level\n", szLevelFile);
			return FALSE;
		}
		return TRUE;
	}
	Kr_Log_Print(KR_LOG_INFO, "The level %s does not exist !\n", szPath);
	Kr_Log_Print(KR_LOG_INFO, "Creating the level %s !\n", szPath);
	if(!Level_Editor_LoadLevel(pEditor, szLevelFile, pRenderer))
	{
		return FALSE;
	}
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
	Kr_Level_Free(pEditor->pLevel);
	UTIL_Free(pEditor);
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
	char   szBuf[CACHE_SIZE];
	char   szPath[50];
	FILE  *pFile;
	Uint32 iTmp = 0;
	Sint32 i = 0, j = 0;

	/* Ouverture du fichier level_editor */
	sprintf(szPath, "maps\\%s.txt", pEditor->szEditorFile);
	Kr_Log_Print(KR_LOG_INFO, "Opening level editor file %s\n", szPath);
	pFile = UTIL_OpenFile(szPath, "r"); 
	if (!pFile) return FALSE;
	//rewind(pFile); // remise du curseur au top

	// Initialisation du level
	pEditor->pLevel = Kr_Level_Init(szLevelFile);
	if (pEditor->pLevel == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't initialize the level\n", szLevelFile);
		return FALSE;
	}

	// Numero du level
	fscanf(pFile, "%d", &pEditor->pLevel->iLevelNum);

	// Nom du level
	fgets(szBuf, CACHE_SIZE, pFile); // On récupère le caractère \n
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

	/* Allocation du tableau 2D szLayout */
	pEditor->pLevel->szLayout = malloc(pEditor->pLevel->iLevel_TileWidth*sizeof(Uint32*));
	for (i = 0; i<pEditor->pLevel->iLevel_TileWidth; i++)
		pEditor->pLevel->szLayout[i] = malloc(pEditor->pLevel->iLevel_TileHeight*sizeof(Uint32));


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
	if (!Level_Editor_CreateLevelFile(pEditor->pLevel))
	{
		return FALSE;
	}
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
	FILE *pFile;
	char szPath[50];
	Sint32 i = 0, j = 0;

	sprintf(szPath, "maps\\%s.txt", pLevel->szLevelFile);
	pFile = UTIL_OpenFile(szPath, "w"); // Ouverture en écriture 
	if (!pFile) return FALSE;

	fprintf(pFile, "%s\n", KR_LEVEL_VERSION);
	fprintf(pFile, "#property\n");
	fprintf(pFile, "%s\n", pLevel->szLevelName);
	fprintf(pFile, "%s\n", pLevel->szLevelName);
	fprintf(pFile, "none\n");
	fprintf(pFile, "#tileset\n");
	fprintf(pFile, "%s\n", pLevel->pLevel_Tileset->szTilesetName);
	fprintf(pFile, "#entity\n");
	fprintf(pFile, "#layout\n");
	fprintf(pFile, "%d %d\n", pLevel->iLevel_TileWidth, pLevel->iLevel_TileHeight);
	
	for (j = 0; j<pLevel->iLevel_TileHeight; j++)
	{
		for (i = 0; i<pLevel->iLevel_TileWidth; i++)
		{
			fprintf(pFile, "%d ", pLevel->szLayout[i][j]);
		}
		fprintf(pFile,"\n");
	}
	fprintf(pFile, "#end\n");

	UTIL_CloseFile(&pFile);
	Kr_Log_Print(KR_LOG_INFO, "File %s has been created \n",szPath);
	return TRUE;
}


/*!
*  \fn     Grid *Grid_Init(char *szFileName, Kr_Level *pLevel, SDL_Renderer *pRenderer)
*  \brief  Function to initialize a Grid structure
*
*  \param  szFileName the name of the image which we'll use to do the grid with the extension
*  \param  pLevel    the level structure 
*  \param  pRenderer a pointer to the renderer
*  \return the initialized structure, NULL otherwise
*/
Grid *Grid_Init(char *szFileName, Kr_Level *pLevel, SDL_Renderer *pRenderer)
{
	Grid *pGrid = NULL;
	char szPath[50];
	sprintf(szPath, "sprites\\%s", szFileName);

	pGrid = (Grid *)UTIL_Malloc(sizeof(Grid));
	if (pGrid == NULL) return NULL;
	pGrid->Rect.x = 0;
	pGrid->Rect.y = 0;
	pGrid->Rect.h = pLevel->pLevel_Tileset->iTilesHeight;
	pGrid->Rect.w = pLevel->pLevel_Tileset->iTilesWidth;
	pGrid->pTexture = UTIL_LoadTexture(pRenderer, szPath, NULL, &pGrid->Rect); // Le redimensionnement de la texture sera fait dans cette fonction

	return pGrid;
}

/*!
*  \fn     void Grid_Free(Grid *pGrid)
*  \brief  Function to free a Grid structure
*
*  \param  pGrid  the Grid structure to free
*  \return none
*/
void Grid_Free(Grid *pGrid)
{
	UTIL_FreeTexture(&pGrid->pTexture);
	UTIL_Free(pGrid);
}


/*!
*  \fn     void Grid_Draw(Grid *pGrid, Kr_Level *pLevel, Boolean bMustDraw, SDL_Renderer *pRenderer)
*  \brief  Function to Draw a Grid on a level
*
*  \param  pGrid     the Grid structure 
*  \param  pLevel    the level structure 
*  \param  bMustDraw Must we draw the grid?
*  \param  pRenderer a pointer to the renderer
*  \return none
*/
void Grid_Draw(Grid *pGrid, Kr_Level *pLevel, Boolean bMustDraw, SDL_Renderer *pRenderer)
{
	Sint32   i, j;
	SDL_Rect Rect_dest;

	if (!bMustDraw) return;
	
	for (i = 0; i <= pLevel->iLevel_TileWidth; i++)
	{
		for (j = 0; j <= pLevel->iLevel_TileHeight; j++)
		{
			Rect_dest.x = i*pLevel->pLevel_Tileset->iTilesWidth;
			Rect_dest.y = j*pLevel->pLevel_Tileset->iTilesHeight;
			Rect_dest.h = pLevel->pLevel_Tileset->iTilesHeight;
			Rect_dest.w = pLevel->pLevel_Tileset->iTilesWidth;
			if (i > 0 || i <= pLevel->iLevel_TileWidth || j > 0 || j <= pLevel->iLevel_TileHeight)
			{
				SDL_RenderCopy(pRenderer, pGrid->pTexture, NULL, &Rect_dest); 
			}
		}
	}	
}


/*!
*  \fn     void Level_Editor_PrintTiles(Kr_Tileset *pTileset, Boolean bMustPrint, SDL_Renderer *pRenderer)
*  \brief  Function to print the tiles
*
*  \param  pTileset   a pointer to the tileset
*  \param  bMustPrint Must we print the tiles ?
*  \param  pRenderer  a pointer to the renderer
*  \return none
*/
void Level_Editor_PrintTiles(Kr_Tileset *pTileset, Boolean bMustPrint, SDL_Renderer *pRenderer)
{
	Sint32   i, j, iNumTile;
	SDL_Rect Rect_dest;

	if (!bMustPrint) return;
	iNumTile = 0;
		
	for (j = 0; j < pTileset->iNbTilesY; j++)
	{
		for (i = 0; i < pTileset->iNbTilesX; i++)
		{
			Rect_dest.x = i*pTileset->iTilesWidth;
			Rect_dest.y = j*pTileset->iTilesHeight;
			Rect_dest.h = pTileset->iTilesHeight;
			Rect_dest.w = pTileset->iTilesWidth;
			SDL_RenderCopy(pRenderer, pTileset->pTextureTileset, &(pTileset->pTilesProp[iNumTile].rTile), &Rect_dest);
			iNumTile++;
		}
	}
}


/*!
*  \fn     Sint32 Level_Editor_GetTile(Level_Editor *pEditor,Sint32 x, Sint32 y, Boolean tilesetIsShown)
*  \brief  Function to get the tile on the renderer from coordinate
*
*  \param  x			  coordinate
*  \param  y			  coordinate
*  \param  tilesetIsShown is the tileset shown ?
*  \param  pEditor        a pointer to the Level_Editor structure
*  \return The tile number, -1 if error
*/
Sint32 Level_Editor_GetTile(Level_Editor *pEditor, Sint32 x, Sint32 y, Boolean tilesetIsShown)
{
	Sint32 iNumTilesX, iNumTilesY, iTileNum = 0;

	//Si le tileset n'est pas affiché on récupère directement la tile du level, de même si le curseur est hors du tileset bien qu'affiché
	if ((!tilesetIsShown) || (x > (pEditor->pLevel->pLevel_Tileset->iNbTilesX *pEditor->pLevel->pLevel_Tileset->iTilesWidth))
		|| (y > (pEditor->pLevel->pLevel_Tileset->iNbTilesY *pEditor->pLevel->pLevel_Tileset->iTilesHeight)))
	{
		//Kr_Log_Print(KR_LOG_INFO, "You clicked out of the tileset or the tileset is not shown \n");
		return Kr_Level_GetTile(pEditor->pLevel, x, y);
	}
	
	// récupération du tile sur le tileset
	// Obtenir les numéros des tiles
	//Kr_Log_Print(KR_LOG_INFO, "Fetching the tile on the tileset\n");
	iNumTilesX = x / pEditor->pLevel->pLevel_Tileset->iTilesWidth;
	iNumTilesY = y / pEditor->pLevel->pLevel_Tileset->iTilesHeight;
	iTileNum = (pEditor->pLevel->pLevel_Tileset->iNbTilesX * iNumTilesY) + iNumTilesX;
	return iTileNum;
}


/*!
*  \fn     void Level_Editor_PreDrawTile(Level_Editor *pEditor, Uint32 iNumTile, Uint32 x, Uint32 y, Boolean bMustDraw, SDL_Renderer *pRenderer, SDL_Texture *pTextureSelected)
*  \brief  Function to draw the current tile on the renderer from coordinate
*
*  \param  pEditor        a pointer to the Level_Editor structure
*  \param  iNumTile       the number of the tile we want to draw
*  \param  x			  coordinate
*  \param  y			  coordinate
*  \param  bMustDraw      must we draw the tile ?
*  \param  pRenderer      a pointer to the renderer
*  \return none
*/
void Level_Editor_PreDrawTile(Level_Editor *pEditor, Uint32 iNumTile, Uint32 x, Uint32 y, Boolean bMustDraw, SDL_Renderer *pRenderer, SDL_Texture *pTextureSelected)
{
	Uint32 iNumTilesX, iNumTilesY;
	SDL_Rect Rect_dest;

	if (!bMustDraw) return;

	iNumTilesX = x / pEditor->pLevel->pLevel_Tileset->iTilesWidth;
	iNumTilesY = y / pEditor->pLevel->pLevel_Tileset->iTilesHeight;

	Rect_dest.x = iNumTilesX * pEditor->pLevel->pLevel_Tileset->iTilesWidth;
	Rect_dest.y = iNumTilesY * pEditor->pLevel->pLevel_Tileset->iTilesHeight;
	Rect_dest.h = pEditor->pLevel->pLevel_Tileset->iTilesHeight;
	Rect_dest.w = pEditor->pLevel->pLevel_Tileset->iTilesWidth;
	SDL_RenderCopy(pRenderer, pEditor->pLevel->pLevel_Tileset->pTextureTileset, &(pEditor->pLevel->pLevel_Tileset->pTilesProp[iNumTile].rTile), &Rect_dest);
	SDL_RenderCopy(pRenderer, pTextureSelected,NULL, &Rect_dest);
}


/*!
*  \fn     Boolean Level_Editor_SelectingGroup(Sint32 *iTabCursor, Kr_Input *inEvent)
*  \brief  Function to check the area of tile the user is selecting
*
*  \param  iTabCursor   array of position of the cursor
*  \param  inEvent      Structure which handle the input
*  \return TRUE if everything is ok, FALSE otherwise
*/
Boolean Level_Editor_SelectingGroup(Sint32 *iTabCursor, Kr_Input *inEvent)
{
	Boolean bLeave = FALSE;
	Boolean bReturn = FALSE;
	while (!bLeave)
	{
		UpdateEvents(inEvent);
		if (inEvent->szMouseButtons[0] && inEvent->szKey[SDL_SCANCODE_LSHIFT])
		{
			iTabCursor[2] = inEvent->iMouseX;
			iTabCursor[3] = inEvent->iMouseY;
			bLeave = TRUE;
			bReturn = TRUE;
		}
		if (inEvent->szKey[SDL_SCANCODE_ESCAPE])
		{
			bLeave = TRUE;
			bReturn = FALSE;
			inEvent->szKey[SDL_SCANCODE_ESCAPE] = 0;
		}
	}
	return bReturn;
}


/*!
*  \fn    Boolean Level_Editor_GroupFill(Sint32 *iTabTile, Sint32 *iTabCursor, Level_Editor *pEditor, Boolean tilesetIsShown);
*  \brief  Function to fill an array with the number of the tile of each block of the group selection
*
*  \param  iTabTile			array with the number of the tile of each block of the group selection
*  \param  iTabCursor		array of position of the cursor
*  \param  tilesetIsShown	is the tileset shown ?
*  \param  pEditor			a pointer to the Level_Editor structure
*  \return TRUE if everything is ok, FALSE otherwise
*/
Boolean Level_Editor_GroupFill(Sint32 *iTabTile, Sint32 *iTabCursor, Level_Editor *pEditor, Boolean tilesetIsShown)
{
	Sint32 iDiffX = 0, iDiffY = 0, iTmp = 0, iTmp2 = 0, iNumTile = -1;
	Uint32 iNbTilesX = 0, iNbTilesY = 0, i, j,z=0;
	
	// On range dans l'ordre croissant les valeurs de X et Y tel que tab[0]=minX, tab[1]=minY, tab[2]=maxX, tab[3]=maxY
	if (iTabCursor[0] > iTabCursor[2])
	{
		iTmp = iTabCursor[0];
		iTabCursor[0] = iTabCursor[2];
		iTabCursor[2] = iTmp;
	
	}
	if (iTabCursor[1] > iTabCursor[3])
	{
		iTmp = iTabCursor[1];
		iTabCursor[1] = iTabCursor[3];
		iTabCursor[3] = iTmp;
	}

	iNbTilesX = iTabCursor[2] / pEditor->pLevel->pLevel_Tileset->iTilesWidth - iTabCursor[0] / pEditor->pLevel->pLevel_Tileset->iTilesWidth + 1;
	iNbTilesY = iTabCursor[3] / pEditor->pLevel->pLevel_Tileset->iTilesHeight - iTabCursor[1] / pEditor->pLevel->pLevel_Tileset->iTilesHeight + 1;

	if (iNbTilesX * iNbTilesY > LEVEL_EDITOR_MAX_SELECTION) return FALSE;
	for (i = 0; i < iNbTilesY; i++) // ligne du level
	{
		for (j = 0; j < iNbTilesX; j++) //Colonne du level
		{
			iTabTile[z] = Level_Editor_GetTile(pEditor, iTabCursor[0] + j * pEditor->pLevel->pLevel_Tileset->iTilesWidth, iTabCursor[1] + i *pEditor->pLevel->pLevel_Tileset->iTilesHeight, tilesetIsShown);
			if (iTabTile[z] == -1) return FALSE;
			z++;
		}
	}
	iTabCursor[0] = iNbTilesX;
	iTabCursor[1] = iNbTilesY;
	return TRUE;
}


/*!
*  \fn     void    Level_Editor_PreDrawTileSelection(Level_Editor *pEditor, Sint32 *iTabTile, Uint32 x, Uint32 y, Boolean bPreDraw,SDL_Renderer *pRendererSint32, *iTabNbTiles,SDL_Texture *pTextureSelected)
*  \brief  Function to draw the current group selection on the renderer
*
*  \param  pEditor        a pointer to the Level_Editor structure
*  \param  iTabTile       array of tile number to draw
*  \param  x			  coordinate
*  \param  y			  coordinate
*  \param  bMustDraw      must we draw the tile ?
*  \param  pRenderer      a pointer to the renderer
*  \param  iTabNbTiles    an array which two first case indicate the nb of tiles on X and Y
*  \return none
*/
void Level_Editor_PreDrawTileSelection(Level_Editor *pEditor, Sint32 *iTabTile, Uint32 x, Uint32 y, Boolean bPreDraw, SDL_Renderer *pRenderer, Sint32 *iTabNbTiles, SDL_Texture *pTextureSelected)
{
	Uint32 i = 0, iCoordX = x, iCoordY = y - pEditor->pLevel->pLevel_Tileset->iTilesHeight;
	
	while (iTabTile[i] != -1)
	{
		
		if (i % (iTabNbTiles[0])) //détection numéro de la ligne
		{
			iCoordX += pEditor->pLevel->pLevel_Tileset->iTilesWidth; // colonne suivante
		}		
		else
		{	
			iCoordY += pEditor->pLevel->pLevel_Tileset->iTilesHeight; // Ligne suivante
			iCoordX = x; // remise à 0 de la colonne
		}
		Level_Editor_PreDrawTile(pEditor, iTabTile[i], iCoordX, iCoordY, bPreDraw, pRenderer, pTextureSelected);
		i++;
	}
}


/*!
*  \fn     void Level_Editor_WriteLayoutSelection(Level_Editor *pEditor, Sint32 *iTabTile, Uint32 x, Uint32 y, Sint32 *iTabCursor);
*  \brief  Function to write the current group selection in the layout
*
*  \param  pEditor        a pointer to the Level_Editor structure
*  \param  iTabTile       array of tile number to draw
*  \param  x			  coordinate
*  \param  y			  coordinate
*  \param  iTabCursor     an array which two first case indicate the nb of tiles on X and Y
*  \return none
*/
void Level_Editor_WriteLayoutSelection(Level_Editor *pEditor, Sint32 *iTabTile, Uint32 x, Uint32 y, Sint32 *iTabCursor)
{
	Uint32 i = 0, iCoordX = x, iCoordY = y - pEditor->pLevel->pLevel_Tileset->iTilesHeight;

	while (iTabTile[i] != -1)
	{
		if (i % (iTabCursor[0])) //détection numéro de la ligne
		{
			iCoordX += pEditor->pLevel->pLevel_Tileset->iTilesWidth; // colonne suivante
		}
		else
		{
			iCoordY += pEditor->pLevel->pLevel_Tileset->iTilesHeight; // Ligne suivante
			iCoordX = x; // remise à 0 de la colonne
		}
		Kr_Level_WriteLayout(pEditor->pLevel, iTabTile[i],iCoordX,iCoordY);
		i++;
	}	
}



/*!
*  \fn     int Editor(void)
*  \brief  Function to launch the level editor
*
*  \return EXIT_SUCCESS if everything is ok, EXIT_FAILURE otherwise
*/
Uint32 Editor(SDL_Renderer *pRenderer, SDL_Window *pWindow)
{
	/* ========================================================================= */
	/*                           CONFIGURATION GENERALE                          */
	/* ========================================================================= */

	Kr_Input inEvent; // Structure pour la gestion des événements

	/* Initialisation de la structure pour gérer les événements*/
	InitEvents(&inEvent);

	/* ========================================================================= */
	/*                                LEVEL EDITOR                               */
	/* ========================================================================= */

	Kr_Log_Print(KR_LOG_INFO, "Launching the Level Editor \n\n");
	Level_Editor *pEditor = NULL;

	// Chargement du level Editor
	pEditor = Level_Editor_Init("editor");
	if (pEditor == NULL)
	{
		Kr_Log_Print(KR_LOG_INFO, "Can't initialize a Level_Editor structure \n\n");
		SDL_Quit();
	}
	if (!Level_Editor_Load(pEditor, pRenderer))
	{
		Kr_Log_Print(KR_LOG_INFO, "Can't Load a Level_Editor structure \n\n");
		SDL_Quit();
	}

	/* Préparation d'une Texture contenant un message via util.c*/
	SDL_Rect     textPosition;
	SDL_Rect     textPosition2;
	SDL_Color    couleur = { 247, 7, 7 };
	SDL_Color    couleur2 = { 255, 255, 0 };
	SDL_Texture *pTextureText = NULL;
	TTF_Font    *pFont = NULL;
	char         szCompteur[100] = " ";// Tableau contenant la valeur du compteur
	textPosition.x = 0;
	textPosition.y = 0;
	textPosition2.x = 350;
	textPosition2.y = 0;
	pFont = Kr_Text_OpenFont("cour", 20);
	TTF_SetFontStyle(pFont, TTF_STYLE_BOLD);

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

	/* Gestion de la grille */
	Grid *pGrid = NULL;
	Boolean bGridShow = TRUE;
	pGrid = Grid_Init("quadrillage32.png", pEditor->pLevel, pRenderer);
	if (pGrid == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't initialize the Grid structure\n");
		exit(EXIT_FAILURE);
	}

	/* Affichage des tiles*/
	Boolean bTilesShow = FALSE; // Affichage ou non du tileset
	Boolean bPreDraw = TRUE;    // Visualiser ou non le tile actuel
	Boolean bSelection = FALSE;
	/* Divers */
	Sint32 iNumTile = 0;
	Sint32 iTabCursor[4] = { 0, 0, 0, 0 };
	Sint32 iTabTile[LEVEL_EDITOR_MAX_SELECTION];
	Uint32 i = 0, j = 0;
	Sint32 iNumCursor = 0;
	SDL_Texture *pTextureSelected = NULL;
	pTextureSelected = UTIL_LoadTexture(pRenderer, "sprites\\SelectionEditor32.png", NULL, NULL);
	if (pTextureSelected == NULL)
	{
		Kr_Log_Print(KR_LOG_INFO, "Can't load the SelectionEditor32.png texture !\n");
		SDL_Quit();
	}
	/* ========================================================================= */
	/*                                 EVENEMENT                                 */
	/* ========================================================================= */
	while (!inEvent.bQuit)
	{

		UpdateEvents(&inEvent);
		//Annuler une sélection de groupe
		if (inEvent.szKey[SDL_SCANCODE_ESCAPE])
		{
			bSelection = FALSE;
			inEvent.szKey[SDL_SCANCODE_ESCAPE] = 0;
		}
		// SELECTION D'UN GROUPE DE TILES
		if (inEvent.szMouseButtons[0] && inEvent.szKey[SDL_SCANCODE_LSHIFT])
		{
			iTabCursor[0] = inEvent.iMouseX;
			iTabCursor[1] = inEvent.iMouseY;
			inEvent.szMouseButtons[0] = 0;
			bSelection = FALSE;
			sprintf(szCompteur, "SELECTING A GROUP OF TILES - PRESS ESCAPE TO CANCEL");
			pTextureText = Kr_Text_FontCreateTexture(pRenderer, pFont, szCompteur, couleur2, TRUE, &textPosition2); // Création d'une texture contenant le texte d'une certaine couleur avec le mode Blended  
			SDL_RenderCopy(pRenderer, pTextureText, NULL, &textPosition2);
			SDL_RenderPresent(pRenderer);
			if (Level_Editor_SelectingGroup(iTabCursor, &inEvent)) // Si la sélection d'une zone est confirmé
			{
				for (i = 0; i < LEVEL_EDITOR_MAX_SELECTION; i++) iTabTile[i] = -1; // initialisation du tableau contenant les tiles à -1;
				if (Level_Editor_GroupFill(iTabTile, iTabCursor, pEditor, bTilesShow)) // si le remplissage du tableau s'est bien passé
				{
					bSelection = TRUE;
					bPreDraw = FALSE;
				}
				bTilesShow = FALSE;
			}
			inEvent.szMouseButtons[0] = 0;
			inEvent.szKey[SDL_SCANCODE_LSHIFT] = 0;
		}

		// Modifier le tile Strandard
		if (inEvent.szMouseButtons[1] && inEvent.szKey[SDL_SCANCODE_LCTRL])
		{
			pEditor->iStandardTile = Level_Editor_GetTile(pEditor, inEvent.iMouseX, inEvent.iMouseY, bTilesShow);
			bPreDraw = FALSE;
			inEvent.szMouseButtons[1] = 0;
		}

		if (inEvent.szMouseButtons[0])
		{
			if (bTilesShow) // Menu des tiles ouverts = Sélection d'une tile
			{
				iNumTile = Level_Editor_GetTile(pEditor, inEvent.iMouseX, inEvent.iMouseY, bTilesShow);
				Kr_Log_Print(KR_LOG_INFO, "Select tiles : %d\n", iNumTile);
				inEvent.szKey[SDL_SCANCODE_TAB] = 1; // Forcer la fermeture du menu tiles
				inEvent.szMouseButtons[0] = 0;
			}
			else // Affecter un tile à la map
			{
				if (bSelection) // Appliquer un groupe de tiles
				{
					Level_Editor_WriteLayoutSelection(pEditor, iTabTile, inEvent.iMouseX, inEvent.iMouseY, iTabCursor);
					bSelection = FALSE;
					inEvent.szMouseButtons[0] = 0;
				}
				else
				{
					if (bPreDraw == FALSE) // Dans le cas d'un clique droit sans affichage du tile courant
					{
						bPreDraw = TRUE;
						inEvent.szMouseButtons[0] = 0;
					}
					else
					{
						Kr_Level_WriteLayout(pEditor->pLevel, iNumTile, inEvent.iMouseX, inEvent.iMouseY);
					}					
				}
			}
		}

		// Selectionner une tile
		if (inEvent.szMouseButtons[1])
		{
			iNumTile = Level_Editor_GetTile(pEditor, inEvent.iMouseX, inEvent.iMouseY, bTilesShow);
			inEvent.szKey[SDL_SCANCODE_LCTRL] = 0;
			inEvent.szMouseButtons[1] = 0;		
			bPreDraw = TRUE;
			if (bTilesShow) bTilesShow = FALSE;
		}

		// APPLIQUER LE TILE STANDARD
		if (inEvent.szMouseButtons[2])
		{
			
			if (!bTilesShow) // Remettre le tile standard
			{
				Kr_Level_WriteLayout(pEditor->pLevel, pEditor->iStandardTile, inEvent.iMouseX, inEvent.iMouseY);
				bPreDraw = FALSE;
			}
			else
			{
				inEvent.szMouseButtons[2] = 0;
			}
		}

		// Gestion de l'affichage des FPS
		if (inEvent.szKey[SDL_SCANCODE_F])
		{
			if (pFPS->bMustShow) pFPS->bMustShow = FALSE;
			else pFPS->bMustShow = TRUE;
			inEvent.szKey[SDL_SCANCODE_F] = 0;
		}

		// Gestion de la grille
		if (inEvent.szKey[SDL_SCANCODE_G])
		{
			if (bGridShow) bGridShow = FALSE;
			else bGridShow = TRUE;
			inEvent.szKey[SDL_SCANCODE_G] = 0;
		}

		// Gestion de l'affichage des tiles
		if (inEvent.szKey[SDL_SCANCODE_TAB])
		{
			if (bTilesShow)
			{
				bTilesShow = FALSE;
				bPreDraw = TRUE;
			}
			else
			{
				bTilesShow = TRUE;
				bPreDraw = FALSE;
			}
			inEvent.szKey[SDL_SCANCODE_TAB] = 0;
		}

		// Roulette en avant
		if (inEvent.iScrollVertical < 0)
		{
			if (iNumTile >= (pEditor->pLevel->pLevel_Tileset->iNbTilesX * pEditor->pLevel->pLevel_Tileset->iNbTilesY - 1))
			{
				iNumTile = 0;
			}
			else iNumTile++;
			inEvent.iScrollVertical = 0;
			bPreDraw = TRUE;
		}

		// Roulette en arrière
		if (inEvent.iScrollVertical > 0)
		{
			if (iNumTile <= 0)
			{
				iNumTile = (pEditor->pLevel->pLevel_Tileset->iNbTilesX * pEditor->pLevel->pLevel_Tileset->iNbTilesY) - 1;
			}
			else iNumTile--;
			inEvent.iScrollVertical = 0;
			bPreDraw = TRUE;
		}

		//Sauvegarder la map
		if (inEvent.szKey[SDL_SCANCODE_S])
		{
			Kr_Level_SaveLayout(pEditor->pLevel);
			inEvent.szKey[SDL_SCANCODE_S] = 0;
		}

		/* ========================================================================= */
		/*                                    FPS                                    */
		/* ========================================================================= */

		Kr_Fps_Wait(pFPS, &iCurrentTime, &iPreviousTime, KR_FPS);


		/* ========================================================================= */
		/*                                  DIVERS                                   */
		/* ========================================================================= */
		iNumCursor = Kr_Level_GetTile(pEditor->pLevel, inEvent.iMouseX, inEvent.iMouseY);
		pTextureText = Kr_Text_FontCreateTexture(pRenderer, pFont, szCompteur, couleur, TRUE, &textPosition);  
		sprintf(szCompteur, "Level%d: %s | Collision : %d", pEditor->pLevel->iLevelNum, pEditor->pLevel->szLevelName, pEditor->pLevel->pLevel_Tileset->pTilesProp[iNumCursor].iPlein);

		/* ========================================================================= */
		/*                                  RENDER                                   */
		/* ========================================================================= */

		SDL_RenderClear(pRenderer);
		Kr_Level_Draw(pRenderer, pEditor->pLevel);
		Level_Editor_PreDrawTile(pEditor, iNumTile, inEvent.iMouseX, inEvent.iMouseY, bPreDraw, pRenderer, pTextureSelected);
		if (bSelection) Level_Editor_PreDrawTileSelection(pEditor, iTabTile, inEvent.iMouseX, inEvent.iMouseY, TRUE, pRenderer, iTabCursor, pTextureSelected);
		Level_Editor_PrintTiles(pEditor->pLevel->pLevel_Tileset, bTilesShow, pRenderer);
		Grid_Draw(pGrid, pEditor->pLevel, bGridShow, pRenderer);
		SDL_RenderCopy(pRenderer, pTextureText, NULL, &textPosition);
		Kr_FPS_Show(pFPS);
		SDL_RenderPresent(pRenderer);
		UTIL_FreeTexture(&pTextureText);
	}
	UTIL_FreeTexture(&pTextureSelected);
	UTIL_FreeTexture(&pTextureText);	// Libération mémoire de la texture du Texte ttf
	SDL_DestroyRenderer(pRenderer);		// Libération mémoire du renderer
	SDL_DestroyWindow(pWindow);			// Libération mémoire de la fenetre
	Kr_Text_CloseFont(&pFont);			// Libération mémoire de la police
	Kr_Text_CloseFont(&pFontFPS);		// Libération mémoire de la police
	Level_Editor_Free(pEditor);
	Grid_Free(pGrid);

	return EXIT_SUCCESS;
}

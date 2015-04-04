/* ========================================================================= */
/*!
* \file    kr_tileset.c
* \brief   Contains the function to handle the tileset.
* \author  Olivier Herrou
* \version 1.0
* \date    01 Mars 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 01/03/2015 | Création.                                    */
/* Herrou        | 22/03/2015 | MAJ tiles properties                         */
/* Herrou        | 23/03/2015 | MAJ tiles properties, iPorteLevel            */
/* Herrou        | 04/04/2015 | Initialisation du nom faite par UTIL_CopyStr */
/*               |            |                                              */
/* ========================================================================= */

#include "kr_tileset.h"

#define CACHE_SIZE 5000

/*!
*  \fn     void Kr_Tileset *Kr_Tileset_Init(const char *szFileName)
*  \brief  Function to initialize a Kr_Tileset structure
*
*  \param  szFileName the name of the tileset file
*  \return the initialize structure
*/
Kr_Tileset *Kr_Tileset_Init(const char *szFileName)
{
	Kr_Tileset  *pTileset = NULL;
	Uint32 iNameLen = strlen(szFileName);
	pTileset = (Kr_Tileset *)UTIL_Malloc(sizeof(Kr_Tileset));

	pTileset->szTilesetName   = UTIL_CopyStr(szFileName, iNameLen);
	pTileset->iNbTilesX		  = 0;
	pTileset->iNbTilesY		  = 0;
	pTileset->iTilesHeight	  = 0;
	pTileset->iTilesWidth	  = 0;
	pTileset->pTextureTileset = NULL;
	pTileset->pTilesProp	  = NULL;
	return pTileset;
}


/*!
*  \fn     void Kr_Tileset_Load(Kr_Tileset *pTileset, SDL_Renderer *pRenderer);
*  \brief  Function to load a tileset 
*
*  \param  pTileset a pointer to a the tileset structure
*  \param  pRenderer  a pointer to the renderer
*  \return TRUE if everything is ok, NULL otherwise
*/
Boolean Kr_Tileset_Load(Kr_Tileset *pTileset, SDL_Renderer *pRenderer)
{
	Uint32 iNumTile;
	Sint32 i, j;
	char         szBuf[CACHE_SIZE];  // Buffer
	char         szBuf2[CACHE_SIZE]; // Buffer2
	SDL_Surface *pSurfTileset = NULL;
	char         szTilePath[50];
	FILE        *pFile;

	/* Ouverture du fichier tileset */
	sprintf(szTilePath, "tilesets\\%s", pTileset->szTilesetName);
	Kr_Log_Print(KR_LOG_INFO, "Opening tileset file %s\n", szTilePath);
	pFile = UTIL_OpenFile(szTilePath, "r"); // Ouverture du tileset en read
	if (!pFile) return FALSE;
	
	/* Vérification de la version du fichier tileset */
	fgets(szBuf, CACHE_SIZE, pFile);
	if (strstr(szBuf, KR_TILESET_VERSION) == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Wrong version of the tileset file, expected :%s\n"KR_TILESET_VERSION);
		Kr_Log_Print(KR_LOG_ERROR, ">> Tileset \"%s\".\n", szTilePath);
		UTIL_CloseFile(&pFile);
		return FALSE;
	}

	/* Chargement de la texture du tileset*/
	fscanf(pFile, "%s", szBuf); // Lecture de la ligne indiquant le nom du fichier image du tileset
	sprintf(szTilePath, "tilesets\\%s",szBuf);

	Kr_Log_Print(KR_LOG_INFO, "Opening tileset image %s\n", szTilePath);
	if (UTIL_OpenFile(szTilePath, "r") == NULL) // Vérifier que le fichier est présent
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't open the tileset image !\n");
		Kr_Log_Print(KR_LOG_ERROR, ">> Tileset image \"%s\".\n", szTilePath);
		UTIL_CloseFile(&pFile);
		return FALSE;
	}

	pSurfTileset = IMG_Load(szTilePath);// Chargement de l'image contenant le tileset
	

	/* Lecture iNbTilesX et iNbTilesY */
	fscanf(pFile, "%d %d", &pTileset->iNbTilesX, &pTileset->iNbTilesY);

	/* Calcul iTilesWidth et iTilesHeight*/
	pTileset->iTilesWidth = pSurfTileset->w / pTileset->iNbTilesX;
	pTileset->iTilesHeight = pSurfTileset->h / pTileset->iNbTilesY;

	/* Création de la texture */
	pTileset->pTextureTileset = SDL_CreateTextureFromSurface(pRenderer, pSurfTileset);
	SDL_FreeSurface(pSurfTileset);
	if (pTileset->pTextureTileset == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't create a texture of the tileset !\n");
		Kr_Log_Print(KR_LOG_ERROR, ">> Tileset image \"%s\".\n", szTilePath);
		UTIL_CloseFile(&pFile);
		return FALSE;
	}

	pTileset->pTilesProp = malloc(pTileset->iNbTilesX*pTileset->iNbTilesY*sizeof(Kr_TileProp)); // Allocation dynamique des propriétés du tileset

	/* Affectation des caractéristiques à chaque tile*/
	for (j = 0, iNumTile = 0; j<pTileset->iNbTilesY; j++) // Sur Y
	{
		for (i = 0; i<pTileset->iNbTilesX; i++, iNumTile++) // Sur X
		{
			pTileset->pTilesProp[iNumTile].rTile.w = pTileset->iTilesWidth; //Donnée du SDL_RECT  width
			pTileset->pTilesProp[iNumTile].rTile.h = pTileset->iTilesHeight; //Donnée du SDL_RECT  height
			pTileset->pTilesProp[iNumTile].rTile.x = i*pTileset->iTilesWidth; //Donnée du SDL_RECT  x
			pTileset->pTilesProp[iNumTile].rTile.y = j*pTileset->iTilesHeight; //Donnée du SDL_RECT  y

			/* Attribut du tile*/
			fscanf(pFile, "%s %s", szBuf, szBuf2); // Rappel : fscanf s'arrete au premier espace rencontré ou \n
		    //Kr_Log_Print(KR_LOG_INFO, "Attribut %s %s\n", szBuf, szBuf2);
			
			pTileset->pTilesProp[iNumTile].iPlein = 1;
			pTileset->pTilesProp[iNumTile].iWater = 0;
			pTileset->pTilesProp[iNumTile].iPorteLevel = 0;
			if ((strcmp(szBuf2, "sol") == 0)   || (strcmp(szBuf2, "porteLevel") == 0) ||
				(strcmp(szBuf2, "fleur") == 0) || (strcmp(szBuf2, "NONE") == 0)  ||
				(strcmp(szBuf2, "porteMaison") == 0)) pTileset->pTilesProp[iNumTile].iPlein = 0;
			if ((strcmp(szBuf2, "eau") == 0)) pTileset->pTilesProp[iNumTile].iWater = 1;
			if ((strcmp(szBuf2, "porteLevel") == 0)) pTileset->pTilesProp[iNumTile].iPorteLevel = 1;

		}
	}
	UTIL_CloseFile(&pFile);
	//Kr_Tileset_Log(pTileset);
	Kr_Log_Print(KR_LOG_INFO, "Kr_Tileset_Init : Done\n");

	return TRUE;
}

/*!
*  \fn     void Kr_Tileset_Free(Kr_Tileset *pTileset);
*  \brief  Function to free a Kr_Tileset structure
*
*  \param  pTileset a pointer to a the tileset structure
*  \return none
*/
void Kr_Tileset_Free(Kr_Tileset *pTileset)
{
	UTIL_FreeTexture(&pTileset->pTextureTileset);
	UTIL_Free(pTileset->pTilesProp);
	UTIL_Free(pTileset);
}

/*!
*  \fn     void Kr_Tileset_Log(Kr_Tileset *pTileset);
*  \brief  Function to log the data of a Tileset
*
*  \param  pTileset a pointer to a the tileset structure
*  \return none
*/
void Kr_Tileset_Log(Kr_Tileset *pTileset)
{
	Kr_Log_Print(KR_LOG_INFO, "Tileset \"%s\" loaded.\n", pTileset->szTilesetName);
	Kr_Log_Print(KR_LOG_INFO, "> Tiles size    Width : %d / Height : %d.\n", pTileset->iTilesWidth, pTileset->iTilesHeight);
	Kr_Log_Print(KR_LOG_INFO, "> Tiles number      X : %d / Y : %d\n", pTileset->iNbTilesX, pTileset->iNbTilesY);
}

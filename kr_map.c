/* ========================================================================= */
/*!
* \file    kr_map.c
* \brief   Contains the functions to handle the map.
* \author  Olivier Herrou
* \version 1.0
* \date    22 Mars 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 22/03/2015 | Creation.                                    */
/*               |            | Ajout de Kr_GetNeigbor                       */
/*               |            |  TODO : strcspn compatible linux ?           */
/* Herrou        | 04/04/2015 | Initialisation du nom faite par UTIL_CopyStr */
/* Herrou        | 05/04/2015 | Ajout de Kr_Map_ShouldChangeLevel    		 */
/*               |            | Ajout de Kr_Map_GetNeighborOfLevel           */
/*               |            | Gestion du changement de Map ok	             */
/* ========================================================================= */

#include "kr_map.h"

#define CACHE_SIZE 200

/*!
*  \fn     Kr_Map *Kr_Map_Init(char *szMapFile)
*  \brief  Function to initialize a Kr_Map structure
*
*  \param szMapFile  the name of the map file
*  \return none
*/
Kr_Map *Kr_Map_Init(char *szMapFile)
{
	char     szMapPath[50];
	char     szBuffer[50];
	Kr_Map  *pMap = NULL;
	FILE    *pFile;
	Uint32   i, iNameLen;

	iNameLen = strlen(szMapFile);

	pMap = (Kr_Map *)UTIL_Malloc(sizeof(Kr_Map));
	pMap->szMapFile = UTIL_CopyStr(szMapFile, iNameLen);

	/* Ouverture du fichier map */
	sprintf(szMapPath, "maps\\%s.txt", pMap->szMapFile);
	Kr_Log_Print(KR_LOG_INFO, "Opening maps data file %s\n", szMapPath);

	pFile = UTIL_OpenFile(szMapPath, "r"); 
	if (!pFile) return NULL;

	fscanf(pFile, "%d", &pMap->iNbLevel);
	Kr_Log_Print(KR_LOG_INFO, "Maps : %d Levels\n", pMap->iNbLevel);

	//Tableau 2D
	pMap->szMapLayout = UTIL_Malloc(pMap->iNbLevel*sizeof(*pMap->szMapLayout));
	for (i = 0; i < pMap->iNbLevel; i++) pMap->szMapLayout[i] = UTIL_Malloc(50 * sizeof(**pMap->szMapLayout));

	do // Lecture ligne par ligne du fichier
	{
		fgets(szBuffer, CACHE_SIZE, pFile);
		if (strstr(szBuffer, "#array"))
		{
			for (i = 0; i < pMap->iNbLevel; i++)
			{
				fgets(szBuffer, CACHE_SIZE, pFile);
				szBuffer[strcspn(szBuffer, "\n")] = '\0'; //retirer \n
				strcpy(pMap->szMapLayout[i],szBuffer); 
			}
		}
	}while (strstr(szBuffer, "#end") == NULL); // Identification de la fin du fichier
	Kr_Map_Log(pMap);
	Kr_Log_Print(KR_LOG_INFO, "Maps %s initialisation completed\n", szMapPath);
	UTIL_CloseFile(&pFile);
	return pMap;
}

/*!
*  \fn     void Kr_Map_Free(Kr_Map *pMap);
*  \brief  Function to free a Kr_Map structure
*
*  \param  pMap a pointer to the map structure
*  \return none
*/
void Kr_Map_Free(Kr_Map *pMap)
{
	Uint32 i;
	for (i = 0; i < pMap->iNbLevel; i++)
		free(pMap->szMapLayout[i]);	
	UTIL_Free(pMap->szMapLayout);
	UTIL_Free(pMap->szMapFile);
	UTIL_Free(pMap);
}

/*!
*  \fn     void Kr_Map_Log(Kr_Map *pMap);
*  \brief  Function to log a Kr_Map structure
*
*  \param  pMap a pointer to the map structure
*  \return none
*/
void Kr_Map_Log(Kr_Map *pMap)
{
	Uint32 i;

	for (i = 0; i < pMap->iNbLevel; i++)
	{
		Kr_Log_Print(KR_LOG_INFO, "[level%d] : %s\n",i+1, pMap->szMapLayout[i]);
	}
}

/*!
*  \fn     void Kr_Map_GetNeighborOfLevel(Kr_Map *pMap, Kr_Level *pLevel, Uint32 *iNumNord, Uint32 *iNumSud, Uint32 *iNumEst, Uint32 *iNumOuest)
*  \brief  Function to fill the parameter of the Kr_Level structure about the neighbor level of pLevel
*
*  \param  pMap      a pointer to the map
*  \param  pLevel    a pointer to the level
*  \param  iNumNord  a pointer to the number of the North level
*  \param  iNumSud   a pointer to the number of the South level
*  \param  iNumEst   a pointer to the number of the East level
*  \param  iNumOuest a pointer to the number of the West level
*  \return none
*/
void Kr_Map_GetNeighborOfLevel(Kr_Map *pMap, Kr_Level *pLevel, Uint32 *iNumNord, Uint32 *iNumSud, Uint32 *iNumEst, Uint32 *iNumOuest)
{
	char szArray[20];
	char szBuf[5]= "";
	Uint32 iPosDeb = 0, iPosFin = 0, iArrayLen = 0, i;
	Sint32 iNum = 0;
	/* Déterminer le numéro du level actuel */
	strcpy(szArray, pMap->szMapLayout[pLevel->iLevelNum - 1]);
	
	Kr_Log_Print(KR_LOG_INFO, "The current level %d is connected to %s, lenght : %d\n", pLevel->iLevelNum, szArray, iArrayLen);

	for (i = 0; i < 4; i++)
	{
		iArrayLen = strlen(szArray);
		iPosFin = strcspn(szArray, " ");// Donne le nombre de digits du niveau
		UTIL_SousChaine(szArray, iPosDeb, iPosFin-1, szBuf);//On récupère la string indiquant le numéro du level
		iNum = UTIL_StrToUint32(szBuf); // Conversion de la string en entier non signé
		if (iNum == -1)
		{
			Kr_Log_Print(KR_LOG_WARNING, "Kr_Map_GetNeighborOfLevel : Error during conversion of '%s' to a Uint32\n",szBuf);
			return;
		}
		//Affectation des niveaux voisins
		if (i == 0)		 pLevel->iNumNord = iNum;
		else if (i == 1) pLevel->iNumSud = iNum;
		else if (i == 2) pLevel->iNumEst = iNum;
		else if (i == 3) pLevel->iNumOuest = iNum;
		UTIL_SousChaine(szArray, strlen(szBuf)+1, iArrayLen, szArray); // On supprime de la chaine la partie traité
	}
	
}



/*!
*  \fn     Uint32 Kr_Map_ShouldChangeLevel(Kr_Map *pMap, Kr_Level *pLevel, Entity *pEntity)
*  \brief  Function to check if the level should be changed
*
*  \param  pLevel  a pointer to a the level structure
*  \param  pEntity a pointer to the entity
*  \return the number of the level if we must change, 0 otherwise
*/
Uint32 Kr_Map_ShouldChangeLevel(Kr_Map *pMap, Kr_Level *pLevel, Entity *pEntity)
{
	Sint32 x, y, iTmp;
	Sint32 iTilesID;

	iTmp = 0;
	// Calcule des coordonnées du milieu du rectangle
	x = pEntity->pSprEntity->pRectPosition->x + pEntity->pSprEntity->pRectPosition->w / 2;
	y = pEntity->pSprEntity->pRectPosition->y + pEntity->pSprEntity->pRectPosition->h / 2;
	iTilesID = Kr_Level_GetTile(pLevel, x, y);
	if (pLevel->pLevel_Tileset->pTilesProp[iTilesID].iPorteLevel && iTilesID != -1)// Le tile est-il un Tile pour changer de level ?
	{
		Kr_Map_GetNeighborOfLevel(pMap, pLevel, &pLevel->iNumNord, &pLevel->iNumSud, &pLevel->iNumEst, &pLevel->iNumOuest);
		if (pEntity->direction == nord && (y < pLevel->pLevel_Tileset->iTilesHeight))
		{
			pEntity->pSprEntity->pRectPosition->y = KR_HEIGHT_WINDOW - pEntity->pSprEntity->pRectPosition->h - 1; // Pour éviter les collisions également
			pEntity->iCoordYEntity = KR_HEIGHT_WINDOW - pEntity->pSprEntity->pRectPosition->h - 1;
			return pLevel->iNumNord;
		}
		else if (pEntity->direction == sud && (y >(KR_HEIGHT_WINDOW - pLevel->pLevel_Tileset->iTilesHeight)))
		{
			pEntity->pSprEntity->pRectPosition->y = 1; //1 et non 0 pour éviter des collisions dans le mur dès le respawn
			pEntity->iCoordYEntity = 1;
			return pLevel->iNumSud;
		}
		else if (pEntity->direction == est && (x > (KR_WIDTH_WINDOW - pLevel->pLevel_Tileset->iTilesWidth)))
		{
			pEntity->pSprEntity->pRectPosition->x = 1;
			pEntity->iCoordXEntity = 1;
			return pLevel->iNumEst;
		}
		else if (pEntity->direction == ouest && (x < pLevel->pLevel_Tileset->iTilesWidth))
		{
			pEntity->pSprEntity->pRectPosition->x = KR_WIDTH_WINDOW - pEntity->pSprEntity->pRectPosition->w - 1;
			pEntity->iCoordXEntity = KR_WIDTH_WINDOW - pEntity->pSprEntity->pRectPosition->w - 1;
			return pLevel->iNumOuest;
		}
		return 0;
	}


	/* Autre événement */

	return 0;
}

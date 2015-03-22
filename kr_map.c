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
/*               |            |                                              */
/*               |            |                                              */
/*               |            |                                              */
/*               |            |  TODO : strcspn compatible linux ?           */
/* ========================================================================= */

#include "kr_map.h"

#define CACHE_SIZE 200

Kr_Map *Kr_Map_Init(char *szMapFile)
{
	char     szMapPath[50];
	char     szBuffer[50];
	Kr_Map  *pMap = NULL;
	FILE    *pFile;
	Uint32   i;

	pMap = (Kr_Map *)UTIL_Malloc(sizeof(Kr_Map));

	pMap->szMapFile = szMapFile;

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


/* ========================================================================= */
/*!
* \file    level_state.c
* \brief   Contains the structure to handle the level data.
* \author  Alexandre Robin
* \version 1.0
* \date    15 avril 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/*				 |			  |												 */
/* ========================================================================= */

#include "level_state.h"

#define CACHE_SIZE 15000

/*!
 * \fn		Level_State * Level_State_Init()
 * \brief	Function to init a struct handling the level data
 *
 * \return Level_State a pointer to the Level_State created
 */
Level_State * Level_State_Init(){
	Level_State *pLevelSt = UTIL_Malloc(sizeof(Level_State));

	pLevelSt->szLevelStName = NULL;
	pLevelSt->iLevelNum = 0;
	pLevelSt->pLevel = NULL;
	pLevelSt->aEntityLevel = NULL;
	pLevelSt->aSpriteLevel = NULL;
	pLevelSt->aRectPositionEntity = NULL;
	pLevelSt->iNbEntities = 0;

	return pLevelSt;
}


/*!
 * \fn	   Boolean Level_State_Load(Level_State *pLevelSt, Kr_Level *pLevel, SDL_Renderer *pRenderer)
 * \brief  Function to load the Level_State from the Kr_Level and the file related
 *
 * \param  *pLevelSt	a pointer to the data non initialized
 * \param  *pLevel		a pointer to the level
 * \param  *pRenderer	a pointer to the renderer
 * \return TRUE if everything is OK, FALSE otherwise
 */
Boolean	Level_State_Load(Level_State *pLevelSt, Kr_Level *pLevel, SDL_Renderer *pRenderer){
	Uint32	iNameLen = strlen(pLevel->szLevelName);
	Uint32	iNbEntities = 0;
	Uint32	iFrameHeight = 0;
	Uint32	iFrameWidth = 0;
	Uint32	iNbFrames = 0;
	Uint32	iLife = 0;
	Uint32	iArmor = 0;
	Uint32	iCoordX = 0;
	Uint32	iCoordY = 0;
	char   szLevelPath[50];

	FILE  *pFile;
	char    szBuf[CACHE_SIZE];  // Buffer
	char    szEntityName[CACHE_SIZE];

	pLevelSt->szLevelStName = UTIL_CopyStr(pLevel->szLevelName, iNameLen);
	pLevelSt->iLevelNum = pLevel->iLevelNum;
	pLevelSt->pLevel = pLevel;

	/* Ouverture du fichier level */
	sprintf(szLevelPath, "maps\\%s.txt", pLevel->szLevelFile);
	Kr_Log_Print(KR_LOG_INFO, "Opening level file %s\n", szLevelPath);
	pFile = UTIL_OpenFile(szLevelPath, "r"); // Ouverture du level en read
	if (!pFile) return FALSE;

	do // Lecture ligne par ligne du fichier
	{
		fgets(szBuf, CACHE_SIZE, pFile);
		if (strstr(szBuf, "#entity")) // Identification de la ligne property
		{
			fscanf(pFile, "%d\n", &iNbEntities);
			pLevelSt->iNbEntities = iNbEntities;
			pLevelSt->aEntityLevel = (Entity **)malloc((iNbEntities + 1)*sizeof(Entity*));
			pLevelSt->aSpriteLevel = (Kr_Sprite **)malloc((iNbEntities + 1)*sizeof(Kr_Sprite*));
			pLevelSt->aRectPositionEntity = (SDL_Rect**)malloc((iNbEntities + 1)*sizeof(SDL_Rect*));

			int i = 0;
			Kr_Sprite **aSprite = pLevelSt->aSpriteLevel;
			Entity    **aEntity = pLevelSt->aEntityLevel;
			SDL_Rect  **aRect = pLevelSt->aRectPositionEntity;
			for (i = 1; i < iNbEntities + 1; i++){
				fscanf(pFile, "%s %d %d %d %d %d %d %d \n", szEntityName, &iFrameWidth, &iFrameHeight, &iNbFrames, &iLife, &iArmor, &iCoordX, &iCoordY);
				*(aSprite + i) = Kr_Sprite_Init(szEntityName);
				*(aEntity + i) = Entity_Init(szEntityName);
				*(aRect + i) = (SDL_Rect*)malloc(sizeof(SDL_Rect));
				(*(aRect + i))->x = iCoordX;
				(*(aRect + i))->y = iCoordY;
				(*(aRect + i))->h = 64;
				(*(aRect + i))->w = 64;
				Kr_Sprite_Load(*(aSprite + i), iFrameHeight, iFrameWidth, iNbFrames, *(aRect + i), pRenderer);
				Entity_Load(*(aEntity + i), iLife, iArmor, *(aSprite + i));
			}

		}
	} while (strstr(szBuf, "#layout") == NULL); // Identification de la fin des entites

	Kr_Log_Print(KR_LOG_INFO, "Level_State : %s has been loaded !\n", pLevelSt->szLevelStName);
	UTIL_CloseFile(&pFile);
	return TRUE;
}

/*!
* \fn		void Level_State_Free(Level_State *pLevelSt)
* \brief	Function to freed the level data
*
* \param pLevelSt a pointer to the level data
* \return none
*/
void Level_State_Free(Level_State *pLevelSt){
	UTIL_Free(pLevelSt->szLevelStName);
	int i = 0;
	Kr_Sprite **aSprite = pLevelSt->aSpriteLevel;
	Entity    **aEntity = pLevelSt->aEntityLevel;
	SDL_Rect  **aRect = pLevelSt->aRectPositionEntity;
	for (i = 1; i < pLevelSt->iNbEntities; i++){
		Entity_Free(*(aEntity + i));
		Kr_Sprite_Free(*(aSprite+i));
		UTIL_Free(*(aRect+i));
	}
	UTIL_Free(pLevelSt);
}

/*!
*  \fn     void updateAllEntities(Level_State *pLevelSt, SDL_Renderer *pRenderer)
*  \brief  Function to update the direction and the position on the map of the entite
*
*  \param  pLevel  a pointer to the Level
*  \param  pRenderer a pointer to the renderer
*  \return Boolean true if the entites have all been updated false either
*/
/*Boolean updateAllEntities(Level_State *pLevelSt, SDL_Renderer *pRenderer){
	int *i = pLevelSt->pLevel->aListEntity;
	Boolean goodupdate = TRUE;
	for (i + 1; i < pLevelSt->pLevel->iNbEntity; i += 1){
		if (updateEntityVector(pLevelSt->pLevel, *i, pRenderer) == FALSE){
			goodupdate = FALSE;
			break;
		}
	}
	return goodupdate;
}*/



/*!
*  \fn     Boolean drawAllEntities(Level_State *pLevelSt, SDL_Renderer *pRenderer);
*  \brief  Function to draw all the entities of the current level
*
*  \param  pLevelSt
*  \param  pRenderer
*  \return boolean if the entities have been draw on the screen or not
*/
Boolean	drawAllEntities(Level_State *pLevelSt, SDL_Renderer *pRenderer){
	int i = 0;
	Entity **aEntity = pLevelSt->aEntityLevel;
	for (i = 1; i < pLevelSt->iNbEntities + 1; i++){
		Entity_Draw(pRenderer, *(aEntity+i));
	}
	return TRUE;
}


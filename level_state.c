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
	char    szLevelPath[50];

	FILE   *pFile;
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

			Uint32 i = 0;
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
Kr_Sprite_Load(*(aSprite + i), unknown, iFrameHeight, iFrameWidth, iNbFrames, *(aRect + i), pRenderer);
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
	Uint32 i = 0;
	Kr_Sprite **aSprite = pLevelSt->aSpriteLevel;
	Entity    **aEntity = pLevelSt->aEntityLevel;
	SDL_Rect  **aRect = pLevelSt->aRectPositionEntity;
	for (i = 1; i < pLevelSt->iNbEntities; i++){
		Entity_Free(*(aEntity + i));
	}
	UTIL_Free(pLevelSt->aEntityLevel);
	UTIL_Free(pLevelSt->aSpriteLevel);
	UTIL_Free(pLevelSt->aRectPositionEntity);
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
Boolean updateAllEntities(Level_State *pLevelSt, Entity *pPlayer, SDL_Renderer *pRenderer){
	Uint32      i = 0;
	Entity **aEntity = pLevelSt->aEntityLevel;

	for (i = 1; i < pLevelSt->iNbEntities + 1; i++){
		if (updateEntityVector(pLevelSt, *(aEntity + i), pPlayer, pRenderer) == FALSE){
			Kr_Log_Print(KR_LOG_ERROR, "The entity %d couldn't have been updated", i - 1);
			return FALSE;
		}
		//UpdateAllProjectiles((*(aEntity + i))->pWeapon, pRenderer);
	}
	return TRUE;
}

/*!
*  \fn     void updateEntityVector(Level_State *pLevelSt, Entity *pEntity, Entity *pPlayer, SDL_Renderer *pRenderer)
*  \brief  Function to update the direction and the position on the map of the entite
*
*  \param	pLevelSt	a pointer to the Level State
*  \param	pEntity		a pointer to the entity
*  \param	pPlayer		a pointer to the player
*  \param	pRenderer	a pointer to the renderer
*  \return Boolean true if the vector has been updated false either
*/
Boolean updateEntityVector(Level_State *pLevelSt, Entity *pEntity, Entity *pPlayer, SDL_Renderer *pRenderer){
	Sint32 movex, movey;
	Uint32 i = 0;
	//Obtention de la nouvelle trajectoire du monstre
	getVectorToPlayer(pEntity, pPlayer, &movex, &movey);
	movex = movey = 0;
	if ((movex == 0) && (movey == 0)){						//Si pas de mouvement :
		pEntity->mouvement = 0;									//
		pEntity->pSprEntity->iCurrentFrame = 0;					// reset de l'animation
		pEntity->iTempoAnim = 0;
		return TRUE;
	}
	else{
		pEntity->mouvement = 1;

		//Gestion de l'animation
		pEntity->iTempoAnim += 1;
		if (pEntity->iTempoAnim == RESET_FRAME){						//Si la tempo est arrivée à son terme :
			pEntity->pSprEntity->iCurrentFrame += 1;				//	- Frame suivante
			if (pEntity->pSprEntity->iCurrentFrame == pEntity->pSprEntity->iNbFrames)   //Si l'animation est arrivée au bout 
				pEntity->pSprEntity->iCurrentFrame = 0;
			pEntity->iTempoAnim = 0;
		}

		//Gestion des collisions
	/*	if (Kr_CollisionLevel_Move(pLevelSt->pLevel, pEntity->pSprEntity->pRectPosition, movex, movey) == 3){
			//findWayToPlayer(pEntity, pPlayer, &movex, &movey);
			movex = movey = 0;
		}

		
		Entity **aEntity = pLevelSt->aEntityLevel;
		for (i = 1; i < pLevelSt->iNbEntities + 1; i++){
			if (Kr_CollisionRect_Move(pEntity->pSprEntity->pRectPosition, (*(aEntity + i))->pSprEntity->pRectPosition, movex, movey) == 3){
				movex = movey = 0;
			}
		}*/

		//Deplacement final prévu
		//pEntity->iCoordXEntity = pEntity->pSprEntity->pRectPosition->x;
		//pEntity->iCoordYEntity = pEntity->pSprEntity->pRectPosition->y;
		/*pEntity->pSprEntity->pRectPosition->x += movex;
		pEntity->pSprEntity->pRectPosition->y += movey;
		pEntity->iCoordXEntity += movex;
		pEntity->iCoordYEntity += movey;*/
	}

	//	Kr_Log_Print(KR_LOG_INFO, "The entity %s has moved of %d in x and of %d in y\nNew Position : %d ; %d\n", pEntity->strEntityName, movex, movey, pEntity->iCoordXEntity, pEntity->iCoordYEntity);
	return TRUE;
}

/*!
*  \fn     void updatePlayerVector(Kr_Input myEvent, Level_State *pLevelSt, Entity *pPlyer, SDL_Renderer *pRenderer)
*  \brief  Function to update the direction and the position on the map of the entite
*
*	\todo rajouter la fonction de gestion des collisions
*  \param  inEvent  Structure which handle the input
*  \param  pLevelSt a pointer to the Level State
*  \param  pPlayer  a pointer to the player
*  \param pRenderer a pointer to the renderer
*  \return Boolean true if the vector has been updated false either
*/
Boolean updatePlayerVector(Kr_Input myEvent, Level_State *pLevelSt, Entity *pPlayer, SDL_Renderer *pRenderer){
	Sint32 movex, movey;
	Uint32 i = 0;
	//Obtention des déplacements générés par le clavier
	getVector(myEvent, &movex, &movey);
//	Kr_Log_Print(KR_LOG_INFO, "Move vector = { %d , %d }\n", movex, movey);

	Direction newDir = foundDirection(movex, movey, pPlayer);	//On cherche la nouvelle direction

	// Changement de l'animation
	if ((movex == 0) && (movey == 0)){						//Si pas de mouvement :
		pPlayer->mouvement = 0;									//
		pPlayer->pSprEntity->iCurrentFrame = 0;					// reset de l'animation
		pPlayer->iTempoAnim = 0;											// reset de la tempo
//		Kr_Log_Print(KR_LOG_INFO, "The entity %s hasn't moved\n", pPlayer->strEntityName);
		return TRUE;
	}
	else{												//Sinon
		pPlayer->mouvement = 1;

		//Gestion de l'animation
		pPlayer->iTempoAnim += 1;
		if (pPlayer->iTempoAnim == RESET_FRAME){						//Si la tempo est arrivée à son terme :
			pPlayer->pSprEntity->iCurrentFrame += 1;				//	- Frame suivante
			if (pPlayer->pSprEntity->iCurrentFrame == pPlayer->pSprEntity->iNbFrames)   //Si l'animation est arrivée au bout 
				pPlayer->pSprEntity->iCurrentFrame = 0;								  //	-> on revient au début
//			Kr_Log_Print(KR_LOG_INFO, "Frame counter = %d\n", entite->pSprEntity->iCurrentFrame);


			pPlayer->iTempoAnim = 0;

//			Kr_Log_Print(KR_LOG_INFO, "The animation has changed to the next frame\n");
		}

		switchTextureFromDirection(pPlayer, newDir, pRenderer);

		//Gestion des collisions
	
		Sint32 NewVx = 0;
		Sint32 NewVy = 0;
		Uint32 iTmp = 0;
		//Kr_Log_Print(KR_LOG_INFO, "\nAvant : %d %d\n", pPlayer->pSprEntity->pRectPosition->x, pPlayer->pSprEntity->pRectPosition->y);
		iTmp = Kr_Collision(pLevelSt->pLevel, pPlayer->pSprEntity->pRectPosition, NULL, movex, movey, &NewVx, &NewVy);
		Kr_Log_Print(KR_LOG_INFO, "A %d  Souhaité : %d %d   | Réel : %d %d \n",iTmp, movex, movey, NewVx, NewVy);
		//Kr_Log_Print(KR_LOG_INFO, "Après : %d %d\n", pPlayer->pSprEntity->pRectPosition->x, pPlayer->pSprEntity->pRectPosition->y);
	
		Entity **aEntity = pLevelSt->aEntityLevel;
		for (i = 1; i < pLevelSt->iNbEntities + 1; i++)
		{
			movex = NewVx;
			movey = NewVy;
			NewVx = NewVy = 0;
			iTmp = Kr_Collision(NULL, pPlayer->pSprEntity->pRectPosition, (*(aEntity + i))->pSprEntity->pRectPosition, movex, movey, &NewVx, &NewVy);
			Kr_Log_Print(KR_LOG_INFO, "B %d  Souhaité : %d %d   | Réel : %d %d \n", iTmp, movex, movey, NewVx, NewVy);
			//infightingDamage(*(aEntity + i), pPlayer);
		}
		

		//Deplacement final prévu
		pPlayer->pSprEntity->pRectPosition->x += NewVx;
		pPlayer->pSprEntity->pRectPosition->y += NewVy;
		pPlayer->iCoordXEntity = pPlayer->pSprEntity->pRectPosition->x;
		pPlayer->iCoordYEntity = pPlayer->pSprEntity->pRectPosition->y;
		Kr_Log_Print(KR_LOG_INFO, "B %d  Souhaité : %d %d   | Réel : %d %d \n", iTmp, movex, movey, NewVx, NewVy);
		//Kr_Log_Print(KR_LOG_INFO, "The entity %s has moved of %d in x and of %d in y\nNew Position : %d ; %d\n", pPlayer->strEntityName, movex, movey, pPlayer->iCoordXEntity, pPlayer->iCoordYEntity);
		return TRUE;
	}
}


/*!
*  \fn     Boolean drawAllEntities(Level_State *pLevelSt, SDL_Renderer *pRenderer);
*  \brief  Function to draw all the entities of the current level
*
*  \param  pLevelSt
*  \param  pRenderer
*  \return boolean if the entities have been draw on the screen or not
*/
Boolean	drawAllEntities(Level_State *pLevelSt, SDL_Renderer *pRenderer){
	Uint32 i = 0;
	Entity **aEntity = pLevelSt->aEntityLevel;
	for (i = 1; i < pLevelSt->iNbEntities + 1; i++){
		Entity_Draw(pRenderer, *(aEntity + i));
	}
	return TRUE;
}


/*!
*  \fn    Uint32 Kr_Level_Interraction(Kr_Level *pLevel, Entity *pPlayer);
*  \brief  Function to handle the interraction of an entity on the map
*
*  \param  pLevel   a pointer to the level
*  \param  pPlayer  a pointer to the entity
*  \return the value of the "interraction" cf code
*/
Uint32 Kr_Level_Interraction(Kr_Level *pLevel, Entity *pPlayer)
{
	Sint32 iTilesID = -1;
	Uint32 x = pPlayer->pSprEntity->pRectPosition->x + pPlayer->pSprEntity->pRectPosition->w / 2, y = pPlayer->pSprEntity->pRectPosition->y + pPlayer->pSprEntity->pRectPosition->h / 2;
	// recherche du bloc que l'entité à devant lui
	if (pPlayer->direction == nord)
	{
		y = y - pPlayer->pSprEntity->pRectPosition->h;
	}
	else if (pPlayer->direction == sud)
	{
		y = y + pPlayer->pSprEntity->pRectPosition->h;
	}
	else if (pPlayer->direction == est)
	{
		x = x + pPlayer->pSprEntity->pRectPosition->w;
	}
	else if (pPlayer->direction == ouest)
	{
		x = x - pPlayer->pSprEntity->pRectPosition->w;
	}

	iTilesID = Kr_Level_GetTile(pLevel, x, y);
	Kr_Log_Print(KR_LOG_INFO, "Interraction with iTiles: %d\n", iTilesID);
	if (iTilesID == -1) return 0; // Rien à analyser

	// Ouverture des coffres
	if (pLevel->pLevel_Tileset->pTilesProp[iTilesID].iCoffreFerme == 1)
	{
		Kr_Log_Print(KR_LOG_INFO, "Ouverture d'un coffre ! \n");
	}

	// Lecture des panneaux
	if (pLevel->pLevel_Tileset->pTilesProp[iTilesID].iPanneau == 1)
	{
		Kr_Log_Print(KR_LOG_INFO, "Lecture d'un panneau ! \n");
	}
	return iTilesID;
}

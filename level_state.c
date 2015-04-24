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
* \fn	  Level_State * Level_State_Init(Entity *pPlayer)
* \brief  Function to init a struct handling the level data
*
* \param  pPlayer a pointer to the player
* \return Level_State a pointer to the Level_State created
*/
Level_State * Level_State_Init(Entity *pPlayer)
{
	Level_State *pLevelSt = UTIL_Malloc(sizeof(Level_State));

	pLevelSt->pPlayer = pPlayer;
	pLevelSt->pLevel = NULL;
	pLevelSt->aEntityLevel = NULL;
	pLevelSt->iNbEntities = 0;

	return pLevelSt;
}


/*!
* \fn	   Boolean Level_State_Load(Level_State *pLevelSt, Kr_Level *pLevel, SDL_Renderer *pRenderer)
* \brief  Function to load the Level_State from the Kr_Level and the file related
*
* \param  pLevelSt	a pointer to the data non initialized
* \param  pLevel	a pointer to the level
* \param  pRenderer	a pointer to the renderer
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
	char    szLevelPath[50];

	FILE   *pFile;
	char    szBuf[CACHE_SIZE];  // Buffer
	char    szEntityName[CACHE_SIZE];
	Uint32 i = 0;

	Kr_Sprite *pSprite = NULL;
	SDL_Rect  Rect;

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
			Entity    **aEntity = pLevelSt->aEntityLevel;

			for (i = 0; i < iNbEntities; i++)
			{
				fscanf(pFile, "%s %d %d %d %d %d %d %d \n", szEntityName, &iFrameWidth, &iFrameHeight, &iNbFrames, &iLife, &iArmor, &Rect.x, &Rect.y);
				Rect.h = iFrameHeight;
				Rect.w = iFrameWidth / iNbFrames;

				/* Chargement des sprites */
				pSprite = Kr_Sprite_Init(szEntityName);	
				if (Kr_Sprite_Load(pSprite, unknown, iFrameHeight, iFrameWidth, iNbFrames, &Rect, pRenderer) == FALSE)
				{
					Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite !\n");
					return FALSE;
				}

				/* Création de l'entité */
				*(aEntity + i) = Entity_Init(szEntityName);

				if (Entity_Load(*(aEntity + i), iLife, iArmor, pSprite) == FALSE)
				{
					Kr_Log_Print(KR_LOG_ERROR, "Cant load the entity !\n");
					return FALSE;
				}
				Entity_Log(*(aEntity + i));
			}

		}
	} while (strstr(szBuf, "#layout") == NULL); // Identification de la fin des entites

	Kr_Log_Print(KR_LOG_INFO, "Level_State : %s has been loaded !\n", pLevelSt->pLevel->szLevelName);
	UTIL_CloseFile(&pFile);
	return TRUE;
}

/*!
* \fn		void Level_State_Free(Level_State *pLevelSt,Boolean bFreePlayer)
* \brief	Function to freed the level state
*
* \param pLevelSt    a pointer to the level state
* \param bFreePlayer must we free the player entity ?
* \return none
*/
void Level_State_Free(Level_State *pLevelSt,Boolean bFreePlayer)
{
	Uint32 i = 0;
	Entity    **aEntity = pLevelSt->aEntityLevel;
	for (i = 0; i < pLevelSt->iNbEntities; i++){
		Entity_Free(*(aEntity + i));
	}
	if (bFreePlayer) Entity_Free(pLevelSt->pPlayer);
	UTIL_Free(pLevelSt);
}

/*!
*  \fn     Boolean updateAllEntities(SDL_Renderer *pRenderer, Level_State *pLevelSt, Kr_Input myEvent)
*  \brief  Function to update all the entities of the current level
*
*  \param  pLevelSt  a pointer to the Level_State structure
*  \param  pRenderer a pointer to the renderer
*  \param  myEvent   the Kr_Input Structure
*  \return Boolean TRUE if the entities have all been updated, FALSE otherwise
*/
Boolean updateAllEntities(SDL_Renderer *pRenderer, Level_State *pLevelSt, Kr_Input myEvent)
{
	Uint32     i = 0;
	Entity   **aEntity = pLevelSt->aEntityLevel;

	if (updateEntity(pRenderer, pLevelSt, myEvent, pLevelSt->pPlayer, TRUE) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "The entity PLAYER haven't been updated", i);
		return FALSE;
	}

	for (i = 0; i < pLevelSt->iNbEntities; i++)
	{
		if (updateEntity(pRenderer, pLevelSt, myEvent, *(aEntity + i), FALSE) == FALSE)
		{
			Kr_Log_Print(KR_LOG_ERROR, "The entity %d haven't been updated",i);
			return FALSE;
		}
	}
	return TRUE;
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
	Entity_Draw(pRenderer, pLevelSt->pPlayer);
	for (i = 0; i < pLevelSt->iNbEntities; i++){
		Entity_Draw(pRenderer, *(aEntity + i));
	}
	return TRUE;
}






/*!
*  \fn     Boolean  updateEntity(Level_State *pLevelSt, Kr_Input myEvent, Entity *pEntity, Boolean bIsPlayer)
*  \brief  This function will update the data about the entity
*
*  \param  pRenderer	a pointer to the renderer
*  \param  pLevelSt		a pointer to the Level_State structure
*  \param  myEvent		the Kr_Input structure
*  \param  pEntity		the Entity which must be updated
*  \param  bIsPlayer	is the entity the player ?

*  \return boolean if data have been upadated
*/
Boolean  updateEntity(SDL_Renderer *pRenderer, Level_State *pLevelSt, Kr_Input myEvent, Entity *pEntity, Boolean bIsPlayer)
{
	Sint32		movex = 0, movey = 0, NewVx = 0, NewVy = 0;
	Uint32		i = 0, iTmp = 0;
	Direction	newDir	= sud; //Défaut
	Entity	  **aEntity = pLevelSt->aEntityLevel;

	// Calcul des vecteurs de déplacement
	if (bIsPlayer) //Player
	{
		getVector(myEvent, &movex, &movey);
	}
	else // Monster
	{
		//getVectorToPlayer(pEntity, &movex, &movey);
		/* Nécessaire que cette fonction connaisse les coordonnées du joueur*/
	}


	newDir = foundDirection(movex, movey, pEntity);
	if ((movex == 0) && (movey == 0)) // Aucun déplacement
	{					
		pEntity->mouvement = 0;	
		pEntity->pSprEntity->iCurrentFrame = 0;	
		pEntity->iTempoAnim = 0;
		return TRUE;
	}
	else
	{
		pEntity->mouvement = 1;
		pEntity->iTempoAnim += 1;
		if (pEntity->iTempoAnim == RESET_FRAME)	//Si la tempo est arrivée à son terme :
		{					
			pEntity->pSprEntity->iCurrentFrame += 1; //	- Frame suivante
			if (pEntity->pSprEntity->iCurrentFrame == pEntity->pSprEntity->iNbFrames) //Si l'animation est arrivée au bout 
			{
				pEntity->pSprEntity->iCurrentFrame = 0;
			}				
			pEntity->iTempoAnim = 0;
		}

		//Gestion de la direction, seulement pour le personnage pour le moment
		if(bIsPlayer) switchTextureFromDirection(pEntity, newDir, pRenderer); 

		// Collision avec le level
		iTmp = Kr_Collision(pLevelSt->pLevel, pEntity->pSprEntity->pRectPosition, NULL, movex, movey, &NewVx, &NewVy);

		// Collision avec les autres entités du level
		for (i = 0; i < pLevelSt->iNbEntities; i++)
		{
			if (pEntity != *(aEntity + i)) // On vérifie que l'on détecte pas une collision avec sois même
			{ 
				movex = NewVx;
				movey = NewVy;
				NewVx = NewVy = 0;
				iTmp = Kr_Collision(NULL, pEntity->pSprEntity->pRectPosition, (*(aEntity + i))->pSprEntity->pRectPosition, movex, movey, &NewVx, &NewVy);
			}
		}
		
		// Déplacement de l'entité 
		pEntity->pSprEntity->pRectPosition->x += NewVx;
		pEntity->pSprEntity->pRectPosition->y += NewVy;
		pEntity->iCoordXEntity = pEntity->pSprEntity->pRectPosition->x;
		pEntity->iCoordYEntity = pEntity->pSprEntity->pRectPosition->y;
		return TRUE;
	}
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

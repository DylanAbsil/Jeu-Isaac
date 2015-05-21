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

#define CACHE_SIZE 1000

/*!
* \fn	  Level_State * Level_State_Init(Entity *pPlayer)
* \brief  Function to init a struct handling the level data
*
* \param  pPlayer a pointer to the player
* \return Level_State a pointer to the Level_State created
*/
Level_State * Level_State_Init(Entity *pPlayer)
{
	Level_State		*pLevelSt = (Level_State *)UTIL_Malloc(sizeof(Level_State));
	ListEnt			*plEnt = (ListEnt *)UTIL_Malloc(sizeof(ListEnt));

	pLevelSt->pPlayer = pPlayer;
	pLevelSt->pLevel = NULL;
	pLevelSt->plEnt = plEnt;
	initListEnt(pLevelSt->plEnt);

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
	Uint32  iEntityState = 0;
	Uint32 i = 0, iSpeed = 0,iDirection = 0;
	Boolean bFriendly = TRUE;
	Kr_Sprite *pSprite = NULL;
	Sint32 iCoordX = 0, iCoordY = 0;

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

			SDL_Rect **aRect = (SDL_Rect**)malloc((iNbEntities + 1)*sizeof(SDL_Rect*));

			setOnFirstEnt(pLevelSt->plEnt);
			for (i = 0; i < iNbEntities; i++)
			{
				fscanf(pFile, "%s %d %d %d %d %d %d %d %d %d %d %d\n", szEntityName, &iDirection, &iFrameWidth, &iFrameHeight, &iNbFrames, &iLife, &iArmor, &iCoordX, &iCoordY, &iSpeed, &iEntityState, &bFriendly);
				
				/* Chargement des rect */
				*(aRect + i) = (SDL_Rect*)malloc(sizeof(SDL_Rect));
				(*(aRect + i))->x = iCoordX;
				(*(aRect + i))->y = iCoordY;
				(*(aRect + i))->h = iFrameHeight;
				(*(aRect + i))->w = iFrameWidth / iNbFrames;

				/* Chargement des sprites */
				pSprite = NULL;
				pSprite = Kr_Sprite_Init(szEntityName);	
				if (Kr_Sprite_Load(pSprite, iDirection, iFrameHeight, iFrameWidth, iNbFrames, (*(aRect + i)), pRenderer) == FALSE)
				{
					Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite !\n");
					UTIL_CloseFile(&pFile);
					return FALSE;
				}

				/* Création de l'entité */
				Entity * new = Entity_Init(szEntityName);
				if (Entity_Load(new, iLife, iArmor, iSpeed, iEntityState, bFriendly, pSprite) == FALSE)
				{
					Kr_Log_Print(KR_LOG_ERROR, "Cant load the entity !\n");
					UTIL_CloseFile(&pFile);
					return FALSE;
				}
				insertLastEnt(pLevelSt->plEnt, new);
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
	deleteListEnt(pLevelSt->plEnt);
	if (bFreePlayer) Entity_Free(pLevelSt->pPlayer);
	UTIL_Free(pLevelSt);
}



/*!
*  \fn     Uint32 updateAllEntities(SDL_Renderer *pRenderer, Level_State *pLevelSt, Kr_Input myEvent)
*  \brief  Function to update all the entities of the current level
*
*  \param  pLevelSt  a pointer to the Level_State structure
*  \param  pRenderer a pointer to the renderer
*  \param  myEvent   the Kr_Input Structure
*  \return  0 : an error occured
			1 : everything went fine, no events
			2 : A bird was feared*
			3 : an entity is dead
*/
Uint32 updateAllEntities(SDL_Renderer *pRenderer, Level_State *pLevelSt, Kr_Input myEvent)
{
	Uint32     i = 0, iTmp = 0, iRetour = 1;
	Boolean		next = FALSE;

	if (updateEntity(pRenderer, pLevelSt, myEvent, pLevelSt->pPlayer, TRUE) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "The entity PLAYER haven't been updated\n", i);
		return FALSE;
	}

	setOnFirstEnt(pLevelSt->plEnt);
	while (pLevelSt->plEnt->current != NULL)
	{
		iTmp = updateEntity(pRenderer, pLevelSt, myEvent, pLevelSt->plEnt->current->e, FALSE);
		if (iTmp == FALSE)
		{
			Kr_Log_Print(KR_LOG_ERROR, "The entity %d haven't been updated\n",i);
			return FALSE;
		}
		else if (iTmp == 2) // Oiseau effrayé
		{
			iRetour = 2;
		}
		else if (iTmp == 3) // Entity dead
		{
			deleteCurrentEnt(pLevelSt->plEnt, &next);
			pLevelSt->iNbEntities -= 1;
		}
		
		if (next == FALSE)
			nextEnt(pLevelSt->plEnt);
		else
			next = FALSE;
	}
	return iRetour;
}

/*!
*  \fn     Boolean drawAllEntities(Level_State *pLevelSt, SDL_Renderer *pRenderer);
*  \brief  Function to draw all the entities of the current level
*
*  \param  pLevelSt
*  \param  pRenderer
*  \return boolean if the entities have been draw on the screen or not
*/
Boolean	drawAllEntities(Level_State *pLevelSt, SDL_Renderer *pRenderer)
{
	if (Entity_Draw(pRenderer, pLevelSt->pPlayer) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "The entity PLAYER hasn't been drawn\n");
		return FALSE;
	}

	setOnFirstEnt(pLevelSt->plEnt);
	while (pLevelSt->plEnt->current != NULL)
	{

		if (Entity_Draw(pRenderer, pLevelSt->plEnt->current->e) == FALSE)
		{
			Kr_Log_Print(KR_LOG_ERROR, "The entity hasn't been drawn\n");
			return FALSE;
		}
		nextEnt(pLevelSt->plEnt);
	}
	return TRUE;
}



/*!
*  \fn     Uint32  updateEntity(Level_State *pLevelSt, Kr_Input myEvent, Entity *pEntity, Boolean bIsPlayer)
*  \brief  This function will update the data about the entity
*
*  \param  pRenderer	a pointer to the renderer
*  \param  pLevelSt		a pointer to the Level_State structure
*  \param  myEvent		the Kr_Input structure
*  \param  pEntity		the Entity which must be updated
*  \param  bIsPlayer	is the entity the player ?

*  \return  0 : an error occured
			1 : everything went fine, no events
			2 : A bird was feared
*/
Uint32  updateEntity(SDL_Renderer *pRenderer, Level_State *pLevelSt, Kr_Input myEvent, Entity *pEntity, Boolean bIsPlayer)
{
	if (pEntity->iEntityLife <= 0 && !bIsPlayer){
		Kr_Log_Print(KR_LOG_INFO, "The entity %s is dead to death !\n", pEntity->strEntityName);
		return 3;
	}
	else{
		Sint32		movex = 0, movey = 0, NewVx = 0, NewVy = 0;
		Uint32		iTmp = 0, iRandomVectorRetour = 0, iRetour = 1;
		Direction	newDir = sud; //Défaut
		NodeListEnt *currentNode = pLevelSt->plEnt->current;

		// Calcul des vecteurs de déplacement
		if (bIsPlayer) //Player
		{
			getVector(myEvent, &movex, &movey);
		}
		else // Monster
		{
			if (pEntity->bFriendly == TRUE)
			{
				iRandomVectorRetour = GenerateRandomVector(&movex, &movey, 0, pEntity->iSpeedEntity, pEntity, pLevelSt->pLevel, pLevelSt->pPlayer, 25, 50);
				if (iRandomVectorRetour == 2) // On souhaite détecter la collision d'un oiseau avec le joueur
				{
					if (strcmp(pEntity->strEntityName, "pigeon1") == 0)
					{
						iRetour = 2;
					}
				}
			}
			else getVectorToPlayer(pEntity, pLevelSt->pPlayer, &movex, &movey);
		}

		newDir = foundDirection(movex, movey, pEntity);
		
		if ((movex == 0) && (movey == 0)) // Aucun déplacement
		{
			pEntity->mouvement = 0;
			pEntity->pSprEntity->iCurrentFrame = 0;
			pEntity->iTempoAnim = 0;
			pEntity->iCurrentMoveX = 0;
			pEntity->iCurrentMoveY = 0;
			return iRetour;
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
			switchTextureFromDirection(pEntity, newDir, pRenderer);

			// Collision avec le level
			if (pEntity->state != noclip)
			{
				iTmp = Kr_Collision(pLevelSt->pLevel, pEntity->pSprEntity->pRectPosition, NULL, movex, movey, &NewVx, &NewVy);
			}

			if (!bIsPlayer && pEntity->state != noclip && pEntity->state != invisible)
			{
				movex = NewVx;
				movey = NewVy;
				NewVx = NewVy = 0;
				iTmp = Kr_Collision(NULL, pEntity->pSprEntity->pRectPosition, pLevelSt->pPlayer->pSprEntity->pRectPosition, movex, movey, &NewVx, &NewVy);
			}

			// Collision avec les autres entités du level
			setOnFirstEnt(pLevelSt->plEnt);
			while (pLevelSt->plEnt->current != NULL)
			{
				if (pEntity != pLevelSt->plEnt->current->e && pEntity->state != noclip && pLevelSt->plEnt->current->e->state != noclip && pLevelSt->plEnt->current->e->state != invisible) // On vérifie que l'on détecte pas une collision avec sois même
				{
					movex = NewVx;
					movey = NewVy;
					NewVx = NewVy = 0;
					iTmp = Kr_Collision(NULL, pEntity->pSprEntity->pRectPosition, pLevelSt->plEnt->current->e->pSprEntity->pRectPosition, movex, movey, &NewVx, &NewVy);
				}
				nextEnt(pLevelSt->plEnt);
			}
			pLevelSt->plEnt->current = currentNode;

			// Déplacement de l'entité sans la gestion des collisions
			if (pEntity->state == noclip)
			{
				NewVx = movex;
				NewVy = movey;
			}
			pEntity->pSprEntity->pRectPosition->x += NewVx;
			pEntity->pSprEntity->pRectPosition->y += NewVy;
			pEntity->iCurrentMoveX = NewVx;
			pEntity->iCurrentMoveY = NewVy;
			return iRetour;
		}

	}
}

/*!
*  \fn		Boolean	updateProjectilesWeapon(SDL_Renderer *pRenderer, Level_State *pLevelSt, Weapon *pWeapon)
*  \brief	Function to update all the position of the projectiles contained in a list in the struct weapon
*
*  \param	pRenderer	a pointer to the renderer
*  \param	pLevelSt	a pointer to the Level_State structure
*  \param	pWeapon		a pointer to the weapon which you want to draw the projectiles
*  \return	boolean TRUE if all the projectiles have moved on the screen, FALSE otherwise
*/
Boolean	updateProjectilesWeapon(SDL_Renderer *pRenderer, Level_State *pLevelSt, Weapon *pWeapon){
	Sint32 movex = 0, movey = 0, NewVx = 0, NewVy = 0;
	Uint32 iTmp = 0;
	Boolean res = TRUE, nextL = FALSE;

	if (emptyList(pWeapon->plProjectile) == FALSE){
		Kr_Log_Print(KR_LOG_INFO, "A projectile to be load\n");
		setOnFirst(pWeapon->plProjectile);
		while (pWeapon->plProjectile->current != NULL)
		{
			/* Calcul de la nouvelle position */
			switch (pWeapon->plProjectile->current->p->direction)
			{
			case nord:
				movex = 0;
				movey -= PROJECTILE_SPEED;
				if (pWeapon->plProjectile->current->p->pSprProjectile->pRectPosition->y + movey <= pWeapon->plProjectile->current->p->iCoordPrj_YEnd)
					iTmp = 1;
				break;
			case est:
				movex += PROJECTILE_SPEED;
				movey = 0;
				if (pWeapon->plProjectile->current->p->pSprProjectile->pRectPosition->x + movey >= pWeapon->plProjectile->current->p->iCoordPrj_XEnd)
					iTmp = 1;
				break;
			case sud:
				movex = 0;
				movey += PROJECTILE_SPEED;
				if (pWeapon->plProjectile->current->p->pSprProjectile->pRectPosition->y + movey >= pWeapon->plProjectile->current->p->iCoordPrj_YEnd)
					iTmp = 1;
				break;
			case ouest:
				movex -= PROJECTILE_SPEED;
				movey = 0;
				if (pWeapon->plProjectile->current->p->pSprProjectile->pRectPosition->x + movey <= pWeapon->plProjectile->current->p->iCoordPrj_XEnd)
					iTmp = 1;
				break;
			}

			if (iTmp == 1)
			{	//Fin de course
				deleteCurrent(pWeapon->plProjectile, &nextL);
				Kr_Log_Print(KR_LOG_INFO, "The projectile is out of range\n");
				res = FALSE;
			}
			else
			{
				iTmp = Kr_Collision(pLevelSt->pLevel, pWeapon->plProjectile->current->p->pSprProjectile->pRectPosition, NULL, movex, movey, &NewVx, &NewVy);
				if (iTmp == 6){	// Collision avec le level
					Kr_Log_Print(KR_LOG_INFO, "The projectile hit the level\n");
					deleteCurrent(pLevelSt->pPlayer->pWeapon->plProjectile, &nextL);
					res = FALSE;
				}
				else{	// Collision avec les autres entités du level
					setOnFirstEnt(pLevelSt->plEnt);
					while (pLevelSt->plEnt->current != NULL)
					{

						NewVx = NewVy = 0;
						iTmp = Kr_Collision(NULL, pWeapon->plProjectile->current->p->pSprProjectile->pRectPosition, pLevelSt->plEnt->current->e->pSprEntity->pRectPosition, movex, movey, &NewVx, &NewVy);
						if (iTmp == 2)
						{
							Kr_Log_Print(KR_LOG_INFO, "The projectile hit an entity in (%d;%d)\n", pWeapon->plProjectile->current->p->pSprProjectile->pRectPosition->x + movex, pWeapon->plProjectile->current->p->pSprProjectile->pRectPosition->y + movey);
							weaponDamage(pWeapon->plProjectile->current->p, pLevelSt->plEnt->current->e);
							deleteCurrent(pWeapon->plProjectile, &nextL);
							res = FALSE;
							break;
						}
						nextEnt(pLevelSt->plEnt);
					}

					if (res == TRUE)
					{
						pWeapon->plProjectile->current->p->pSprProjectile->pRectPosition->x += NewVx;
						pWeapon->plProjectile->current->p->pSprProjectile->pRectPosition->y += NewVy;
						Kr_Log_Print(KR_LOG_INFO, "The projectile has moved in (%d;%d)\n", pWeapon->plProjectile->current->p->pSprProjectile->pRectPosition->x, pWeapon->plProjectile->current->p->pSprProjectile->pRectPosition->y);
					}

				}

			}
			
			if (nextL == FALSE)
				next(pWeapon->plProjectile);
			else
				nextL = FALSE;

		}

		return res;

	}	

	else return FALSE;

}



/*!
*  \fn    Uint32 Kr_Level_Interraction(Kr_Level *pLevel, Entity *pPlayer);
*  \brief  Function to handle the interraction of an entity on the map
*
*  \param  pLevel   a pointer to the level
*  \param  pPlayer  a pointer to the entity
*  \return the value of the "interraction" cf code
			0 : Rien
			1 : Ouverture d'un coffre
			2 : Lecture d'un panneau
*/
Uint32 Kr_Level_Interraction(Kr_Level *pLevel, Entity *pPlayer)
{
	Sint32 iTilesID = -1;
	Uint32 x = pPlayer->pSprEntity->pRectPosition->x + pPlayer->pSprEntity->pRectPosition->w / 2, y = pPlayer->pSprEntity->pRectPosition->y + pPlayer->pSprEntity->pRectPosition->h / 2;
	Sint32 i = 0, j = 0;
	Uint32 xTiles = 0, yTiles = 0;
	Boolean bSave = FALSE;
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
		//Remplacement de tous les coffres fermé
		for (j = 0; j < pLevel->iLevel_TileHeight; j++)
		{
			for (i = 0; i < pLevel->iLevel_TileWidth; i++)
			{
				xTiles = i * pLevel->pLevel_Tileset->iTilesWidth;
				yTiles = j * pLevel->pLevel_Tileset->iTilesHeight;
				iTilesID = Kr_Level_GetTile(pLevel, xTiles, yTiles);
				if (iTilesID == 293)
				{
					Kr_Level_WriteLayout(pLevel, 355, xTiles, yTiles); 
					bSave = TRUE;
				}
				if (iTilesID == 294)
				{
					Kr_Level_WriteLayout(pLevel, 356, xTiles, yTiles);
					bSave = TRUE;
				}
				if (iTilesID == 324)
				{
					Kr_Level_WriteLayout(pLevel, 386, xTiles, yTiles);
					bSave = TRUE;
				}
				if (iTilesID == 325)
				{
					Kr_Level_WriteLayout(pLevel, 387, xTiles, yTiles);
					bSave = TRUE;
				}
				if (iTilesID == 417)
				{
					Kr_Level_WriteLayout(pLevel, 418, xTiles, yTiles);
					bSave = TRUE;
				}
			}
		}
		if (bSave == TRUE) // Sauvegarde du niveau en cas de modification
		{
			if (Kr_Level_SaveLayout(pLevel, FALSE) == FALSE)
			{
				Kr_Log_Print(KR_LOG_WARNING, "Can't save the level after interraction ! \n");
			}
			return 1;
		}
	}

	// Lecture des panneaux
	if (pLevel->pLevel_Tileset->pTilesProp[iTilesID].iPanneau == 1)
	{
		Kr_Log_Print(KR_LOG_INFO, "Lecture d'un panneau ! \n");
		Kr_Log_Print(KR_LOG_INFO, "%s\n",pLevel->szLevelMessage);
		return 2;
	}
	return 0;
}



/*!
*  \fn     Uint32 GenerateRandomVector(Sint32 *pMovex, Sint32 *pMovey, Uint32 iMin, Uint32 iMax, Entity *pEntity, Kr_Level *pLevel, Entity *pPlayer, Uint32 iWait)
*  \brief  Function to generate random vector
*
*  \param  pMoveX  a pointer to the X vector
*  \param  pMoveY  a pointer to the Y vector
*  \param  iMax    Max bound
*  \param  iMin    Min bound
*  \param  pEntity a pointer to the entity
*  \param  pLevel  a pointer to the level
*  \param  pPlayer a pointer to the player entity
*  \param  iWait   number of cycle before a new movement is compute
*  \param  iRatio  The rand is between 0 and 100, if the value is above iRatio the entity will move
*  \return  0 : No event
			1 : The entity collide with the level and not with the player
			2 : The entity collide with the player (and maybe with the level)
*/
Uint32 GenerateRandomVector(Sint32 *pMovex, Sint32 *pMovey, Uint32 iMin, Uint32 iMax, Entity *pEntity, Kr_Level *pLevel, Entity *pPlayer, Uint32 iWait, Uint32 iRatio)
{
	Uint32 i = 0, iSgn = 0, iValue = 0, iRetour = 0;
	Sint32 Dummy = 0;
	Boolean bChange = TRUE;

	// Calcul d'un nouveau déplacement aléatoire
	if (pEntity->iTempoMovement == 0)
	{
		iValue = rand() % 100;
		if (iValue > iRatio)
		{
			iValue = rand() % 2;
			if (iValue == 0)
			{
				*pMovex = (rand() % (iMax - iMin + 1)) + iMin;
				// Inversion du signe
				iValue = rand() % 2;
				if (iValue == 0)	*pMovex = *pMovex * -1;

				iValue = rand() % 5;
				if (iValue == 4)
				{
					*pMovey = (rand() % (iMax - iMin + 1)) + iMin;
					iValue = rand() % 2;
					if (iValue == 0)	*pMovey = *pMovey * -1;
				}
				else *pMovey = 0;
			}
			else
			{

				*pMovey = (rand() % (iMax - iMin + 1)) + iMin;
				// Inversion du signe
				iValue = rand() % 2;
				if (iValue == 0)	*pMovey = *pMovey * -1;

				iValue = rand() % 5;
				if (iValue == 4)
				{
					*pMovex = (rand() % (iMax - iMin + 1)) + iMin;
					iValue = rand() % 2;
					if (iValue == 0)	*pMovex = *pMovex * -1;
				}
				else *pMovex = 0;
			}
		}
		else
		{
			*pMovex = 0;
			*pMovey = 0;
		}
		pEntity->iTempoMovement = iWait;
		return iRetour;
	}

	// Màj des données
	*pMovex = pEntity->iCurrentMoveX;
	*pMovey = pEntity->iCurrentMoveY;
	pEntity->iTempoMovement = pEntity->iTempoMovement - 1;

	// Collision avec le décor
	if (pLevel)
	{
		if (Kr_Collision(pLevel, pEntity->pSprEntity->pRectPosition, NULL, *pMovex, *pMovey, &Dummy, &Dummy) == 6)
		{
			if (*pMovex != 0) *pMovex = *pMovex * -1; // On rentre dans un mur à cause d'un mouvement sur X donc on l'inverse
			if (*pMovey != 0) *pMovey = *pMovey * -1;
			iRetour = 1;
		}
	}
	// Collision avec le joueur
	if (pPlayer && iRetour == 0) // vérifier que la valeur n'est pas déjà été modifié par une collision avec le décor sinon l'entité disparait
	{
		if (Kr_Collision(NULL, pEntity->pSprEntity->pRectPosition, pPlayer->pSprEntity->pRectPosition, *pMovex, *pMovey, &Dummy, &Dummy) == 2)
		{
			if (*pMovex != 0) *pMovex = *pMovex * -1; // On rentre dans un mur à cause d'un mouvement sur X donc on l'inverse
			if (*pMovey != 0) *pMovey = *pMovey * -1;
			iRetour = 2;
		}
	}
	return iRetour;
}


/*!
*  \fn     Boolean Level_State_SaveLevel(Level_State *pCurrentLevelState)
*  \brief  Function to save the data of the level
*
*  \param  pCurrentLevelState  a pointer to the current Level_state
*  \return TRUE if everything is OK, FALSE otherwose
*/
Boolean Level_State_SaveLevel(Level_State *pCurrentLevelState)
{
	char   szPath1[50] = "";
	char   szPath2[50] = "";
	FILE  *pFileSrc = NULL;
	FILE  *pFileDst = NULL;
	Sint32 i = 0, j = 0;
	Uint32 state = 0;

	if (pCurrentLevelState->pLevel == NULL) return FALSE;
	Kr_Log_Print(KR_LOG_INFO, "Saving data of the level %d !\n",pCurrentLevelState->pLevel->iLevelNum);
	/* Ouverture du fichier temporaire*/
	sprintf(szPath1, "maps\\level%d.tmp", pCurrentLevelState->pLevel->iLevelNum);
	pFileDst = UTIL_OpenFile(szPath1, "w"); //écriture
	if (!pFileDst) return FALSE;


	/* Ouverture du fichier level */
	sprintf(szPath2, "maps\\level%d.txt", pCurrentLevelState->pLevel->iLevelNum);
	Kr_Log_Print(KR_LOG_INFO, "Opening level file %s\n", szPath2);
	pFileSrc = UTIL_OpenFile(szPath2, "r"); //Lecture 
	if (!pFileDst)
	{
		UTIL_CloseFile(&pFileSrc);
		return FALSE;
	}

	if (!UTIL_FileCopy(pFileSrc, pFileDst, "#entity")) return FALSE; // copie de la partie précédent les informations sur les entités
	UTIL_CloseFile(&pFileSrc);
	UTIL_CloseFile(&pFileDst);
	if (remove(szPath2)) Kr_Log_Print(KR_LOG_ERROR, "Failed to delete %s !\n", szPath1);
	if (rename(szPath1, szPath2)) Kr_Log_Print(KR_LOG_ERROR, "Failed to rename %s to %s !\n", szPath1, szPath2);



	/* Ouverture du fichier temporaire*/
	sprintf(szPath1, "maps\\level%d.txt", pCurrentLevelState->pLevel->iLevelNum);
	Kr_Log_Print(KR_LOG_INFO, "Opening level file %s\n", szPath2);
	pFileSrc = UTIL_OpenFile(szPath1, "r+");
	if (!pFileSrc) return FALSE;
	fseek(pFileSrc, 0, SEEK_END);

	fprintf(pFileSrc, "%d\n", pCurrentLevelState->iNbEntities);

	setOnFirstEnt(pCurrentLevelState->plEnt);
	while (pCurrentLevelState->plEnt->current != NULL)
	{
		state = pCurrentLevelState->plEnt->current->e->state;
		if (strcmp(pCurrentLevelState->plEnt->current->e->strEntityName, "pigeon1") == 0) state = 0; // On force l'état du pigeon à 0 dans le cas où celui-ci était en phase de vol
		fprintf(pFileSrc, "%s %d %d %d %d %d %d %d %d %d %d %d\n",
			pCurrentLevelState->plEnt->current->e->strEntityName,
			pCurrentLevelState->plEnt->current->e->direction,
			pCurrentLevelState->plEnt->current->e->pSprEntity->iFrameWidth,
			pCurrentLevelState->plEnt->current->e->pSprEntity->iFrameHeight,
			pCurrentLevelState->plEnt->current->e->pSprEntity->iNbFrames,
			pCurrentLevelState->plEnt->current->e->iEntityLife,
			pCurrentLevelState->plEnt->current->e->iArmor,
			pCurrentLevelState->plEnt->current->e->pSprEntity->pRectPosition->x,
			pCurrentLevelState->plEnt->current->e->pSprEntity->pRectPosition->y,
			pCurrentLevelState->plEnt->current->e->iSpeedEntity,
			state,
			pCurrentLevelState->plEnt->current->e->bFriendly);
		nextEnt(pCurrentLevelState->plEnt);
	}
	fprintf(pFileSrc, "#layout\n");
	fprintf(pFileSrc, "%d %d\n", pCurrentLevelState->pLevel->iLevel_TileWidth, pCurrentLevelState->pLevel->iLevel_TileHeight);
	// Remplissage du nouveau Layout
	for (j = 0; j< pCurrentLevelState->pLevel->iLevel_TileHeight; j++)
	{
		for (i = 0; i< pCurrentLevelState->pLevel->iLevel_TileWidth; i++)
		{
			fprintf(pFileSrc, "%d ", pCurrentLevelState->pLevel->szLayout[i][j]);
		}
		fprintf(pFileSrc, "\n");
	}
	fprintf(pFileSrc, "#end");
	UTIL_CloseFile(&pFileSrc);
	return FALSE;
}




/*!
*  \fn     void Level_State_Bomb_Detect(Level_State *pLevelSt, Bombe *pBombe)
*  \brief  Check if the entity of the level are near an exploding bomb
*
*  \param  pLevelSt  a pointer to the current Level_state
*  \param  pBombe    a pointer to the bomb
*  \return none
*/
void Level_State_Bomb_Detect(Level_State *pLevelSt, Bombe *pBombe)
{
	Sint32 i = 0, j = 0, x = 0, y = 0;
	Uint32  iContact = 0;
	setOnFirstEnt(pLevelSt->plEnt);
	while (pLevelSt->plEnt->current != NULL)
	{
		iContact = 0;
		for (i = -1; i <= 1; i++)
		{
			for (j = -1; j <= 1; j++)
			{
				y = pBombe->y + i * pBombe->pEntExplosion->pSprEntity->pRectPosition->h;
				x = pBombe->x + j * pBombe->pEntExplosion->pSprEntity->pRectPosition->w;
				if (pLevelSt->plEnt->current->e->pSprEntity->pRectPosition->x > x && pLevelSt->plEnt->current->e->pSprEntity->pRectPosition->x < x + pBombe->pEntExplosion->pSprEntity->pRectPosition->w &&
					pLevelSt->plEnt->current->e->pSprEntity->pRectPosition->y > y && pLevelSt->plEnt->current->e->pSprEntity->pRectPosition->y < y + pBombe->pEntExplosion->pSprEntity->pRectPosition->h)
				{
					iContact = 1;
					break;
				}
			}
				
		}
		if (iContact)
		{
			Kr_Log_Print(KR_LOG_INFO, "The entity %s was damaged by the bomb \n", pLevelSt->plEnt->current->e->strEntityName);
		}
		nextEnt(pLevelSt->plEnt);
	}
}
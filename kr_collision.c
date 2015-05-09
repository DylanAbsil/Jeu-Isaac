/* ========================================================================= */
/*!
* \file    kr_collision.c
* \brief   Contains the functions to handle the collision
* \author  Olivier Herrou
* \version 1.0
* \date    22 Avril 2015
*/
/* ================================================================================================================ */
/* Developers    | Date       | Comments																			*/
/* --------------+------------+------------------------------------------------------------------------------------ */
/* Herrou        | 22/04/2015 | Création																			*/
/* ===============================================================================================================  */


#include "kr_collision.h"




/*!
*  \fn     Uint32 Kr_Collision(Kr_Level *pLevel, SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy, Sint32 *NewVx, Sint32 *NewVy)
*  \brief  Function to detect the collision of a rectangle with the level and an other rectangle, 
*
*  \param  pLevel a pointer to the level structure ( NULL if you don't want to check )
*  \param  pRect1 a pointer to the rectangle in movements
*  \param  pRect2 a pointer to the second rectangle who is not moving ( NULL if you don't want to check )
*  \param  vx      the movement vector on X you want to move the rectangle 1
*  \param  vy      the movement vector on Y you want to move the rectangle 1
*  \param  NewVx   the new movement vector on X, which might get modified by the function, you MUST sent NULL as a parameter
*  \param  NewVy   the new movement vector on Y, which might get modified by the function, you MUST sent NULL as a parameter
*  \return 0 : The function did not check any collision
*          1 : No collision with the rectangle
*          2 : collision with the rectangle
*          3 : No collision with the level
*          6 : collision with the level
*/
Uint32 Kr_Collision(Kr_Level *pLevel, SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy, Sint32 *NewVx, Sint32 *NewVy)
{
	Uint32 iRetour = 0;

	if (pRect2)
	{
		// diminution du vecteur vitesse pour vérifier une collision à "grande vitesse"
		if (UTIL_ABS(vx) >= pRect2->w || UTIL_ABS(vy) >= pRect2->h) // Prendre valeur absolu sinon cela fonctionnera pas pour des vecteurs négatifs
		{
			Kr_Collision(pLevel, pRect1, pRect2, vx / 2, vy / 2, NewVx, NewVy);
			Kr_Collision(pLevel, pRect1, pRect2, vx - vx / 2, vy - vy / 2, NewVx, NewVy);
			//return 0;
		}
		if (Kr_Collision_RectTry(pRect1, pRect2, vx, vy, NewVx, NewVy) == FALSE)
		{
			iRetour = iRetour + 1; // Pas de collision avec le level
		}
		else
		{
			iRetour = iRetour + 2; // Collision avec le level
			Kr_Collision_RectAffine(pRect1, pRect2, vx, vy, NewVx, NewVy);  //Affinage du déplacement
		}
		return iRetour;
	}

	if (pLevel)
	{
		// diminution du vecteur vitesse pour vérifier une collision à "grande vitesse"
		if (UTIL_ABS(vx) >= pLevel->pLevel_Tileset->iTilesWidth || UTIL_ABS(vy) >= pLevel->pLevel_Tileset->iTilesHeight) // Prendre valeur absolu sinon cela fonctionnera pas pour des vecteurs négatifs
		{
			Kr_Collision(pLevel, pRect1, NULL, vx / 2, vy / 2, NewVx, NewVy); // On précise que l'on ne souhaite pas revérifier les collisions avec l'entité
			Kr_Collision(pLevel, pRect1, NULL, vx - vx / 2, vy - vy / 2, NewVx, NewVy);
			//return 0;
		}
		if (Kr_Collision_LevelTry(pLevel, pRect1, vx, vy, NewVx, NewVy) == FALSE)
		{
			iRetour = iRetour + 3; // Pas de collision avec le level
		}
		else
		{
			iRetour = iRetour + 6; // Collision avec le level
			Kr_Collision_LevelAffine(pLevel, pRect1, vx, vy, NewVx, NewVy);  //Affinage du déplacement
		}
	}

	return iRetour;
}


/*!
*  \fn     Boolean Kr_Collision_LevelDetect(Kr_Level *pLevel, SDL_Rect *pRect1)
*  \brief  Function to detect if a rectangle is colliding with the level background
*
*  \param  pLevel a pointer to a the level structure
*  \param  pRect1  a pointer to the rectangle you want to test
*  \return TRUE if the two rectangle are colliding, FALSE otherwise
*/
Boolean Kr_Collision_LevelDetect(Kr_Level *pLevel, SDL_Rect *pRect1)
{
	Uint32 iMinX, iMinY, iMaxX, iMaxY, i, j, iNumTile;

	// Verifie si on est pas déjà hors map
	if (pRect1->x < 0 || ((pRect1->x + pRect1->w - 1) >= pLevel->iLevel_TileWidth  * pLevel->pLevel_Tileset->iTilesWidth) ||
		pRect1->y < 0 || ((pRect1->y + pRect1->h - 1) >= pLevel->iLevel_TileHeight * pLevel->pLevel_Tileset->iTilesHeight))
	{
		return TRUE;
	}

	// Détermine les tiles à controler
	iMinX = pRect1->x / pLevel->pLevel_Tileset->iTilesWidth;
	iMinY = pRect1->y / pLevel->pLevel_Tileset->iTilesHeight;
	iMaxX = (pRect1->x + pRect1->w - 1) / pLevel->pLevel_Tileset->iTilesWidth;
	iMaxY = (pRect1->y + pRect1->h - 1) / pLevel->pLevel_Tileset->iTilesHeight;

	for (i = iMinX; i <= iMaxX; i++)
	{
		for (j = iMinY; j <= iMaxY; j++)
		{
			iNumTile = pLevel->szLayout[i][j];
			if (pLevel->pLevel_Tileset->pTilesProp[iNumTile].iPlein)
			{
				//Kr_Log_Print(KR_LOG_WARNING, "CollisionDecor:  Collision avec la Tile : %d %d \n",i,j); 
				return TRUE;
			}
		}
	}
	return FALSE;
}

/*!
*  \fn     Boolean Kr_Collision_LevelTry(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy, Sint32 *NewVx, Sint32 *NewVy)
*  \brief  Function to try to move a rectangle with a certain vector speed and check if it's colliding with the level tiles
*
*  \param  pLevel  a pointer to the level
*  \param  pRect1  a pointer to the rectangle which is moving
*  \param  vx      the movement vector on X you want to move the rectangle 1
*  \param  vy      the movement vector on Y you want to move the rectangle 1
*  \param  NewVx   the new movement vector on X, which might get modified by the function, you MUST sent NULL as a parameter
*  \param  NewVy   the new movement vector on Y, which might get modified by the function, you MUST sent NULL as a parameter
*  \return TRUE if the the rectangle and the level background are colliding, FALSE otherwise
*/
Boolean Kr_Collision_LevelTry(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy, Sint32 *NewVx, Sint32 *NewVy)
{
	SDL_Rect test;
	test = *pRect1;
	test.x += vx;
	test.y += vy;
	if (Kr_Collision_LevelDetect(pLevel, &test) == FALSE) // Pas de collision avec le level
	{
		*NewVx += vx;
		*NewVy += vy;
		return FALSE;
	}
	return TRUE;
}

/*!
*  \fn     void Kr_Collision_LevelAffine(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy, Sint32 *NewVx, Sint32 *NewVy)
*  \brief  Function to reduce the vector speed to check if there is a collision between the rectangle and the level background
*
*  \param  pLevel  a pointer to the level
*  \param  pRect1  a pointer to the rectangle which is moving
*  \param  vx      the movement vector on X you want to move the rectangle 1
*  \param  vy      the movement vector on Y you want to move the rectangle 1
*  \param  NewVx   the new movement vector on X, which might get modified by the function, you MUST sent NULL as a parameter
*  \param  NewVy   the new movement vector on Y, which might get modified by the function, you MUST sent NULL as a parameter
*  \return none
*/
void Kr_Collision_LevelAffine(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy, Sint32 *NewVx, Sint32 *NewVy)
{
	Sint32 i;
	SDL_Rect test;
	test = *pRect1;
	for (i = 0; i<UTIL_ABS(vx); i++)
	{		
		if (Kr_Collision_LevelTry(pLevel, &test, UTIL_SGN(vx), 0, NewVx, NewVy) == TRUE) break;
		test.x += UTIL_SGN(vx);
	}
	for (i = 0; i<UTIL_ABS(vy); i++)
	{
		if (Kr_Collision_LevelTry(pLevel, &test, 0, UTIL_SGN(vy), NewVx, NewVy) == TRUE) break;
		test.y += UTIL_SGN(vy);
	}
}



/*!
*  \fn     Boolean Kr_Collision_RectDetect(SDL_Rect *pRect2, SDL_Rect *pRect1)
*  \brief  Function to detect if the Rect2 is colliding with Rect1
*
*  \param  pRect2  a pointer to the rectangle which is not moving
*  \param  pRect1  a pointer to the rectangle which is moving
*  \return TRUE if the two rectangle are colliding, FALSE otherwise
*/
Boolean Kr_Collision_RectDetect(SDL_Rect *pRect2, SDL_Rect *pRect1)
{
	if ((pRect2->x >= pRect1->x + pRect1->w)      // trop à droite
		|| (pRect2->x + pRect2->w <= pRect1->x)   // trop à gauche
		|| (pRect2->y >= pRect1->y + pRect1->h)   // trop en bas
		|| (pRect2->y + pRect2->h <= pRect1->y))  // trop en haut
		return FALSE;
	else
		return TRUE;
}


/*!
*  \fn     Boolean Kr_Collision_RectTry(SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy, Sint32 *NewVx, Sint32 *NewVy)
*  \brief  Function to try to move a rectangle with a certain vector speed and check if it's colliding with the level tiles
*
*  \param  pRect1  a pointer to the rectangle which is moving
*  \param  pRect2  a pointer to the rectangle which is not moving
*  \param  vx      the movement vector on X you want to move the rectangle 1
*  \param  vy      the movement vector on Y you want to move the rectangle 1
*  \param  NewVx   the new movement vector on X, which might get modified by the function, you MUST sent NULL as a parameter
*  \param  NewVy   the new movement vector on Y, which might get modified by the function, you MUST sent NULL as a parameter
*  \return TRUE if the the rectangle and the level background are colliding, FALSE otherwise
*/
Boolean Kr_Collision_RectTry(SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy, Sint32 *NewVx, Sint32 *NewVy)
{
	SDL_Rect test;
	test = *pRect1;
	test.x += vx;
	test.y += vy;
	if (Kr_Collision_RectDetect(pRect2, &test) == FALSE) // Pas de collision avec le level
	{
		*NewVx += vx;
		*NewVy += vy;
		return FALSE;
	}
	return TRUE;
}

/*!
*  \fn     void Kr_Collision_RectAffine(SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy, Sint32 *NewVx, Sint32 *NewVy)
*  \brief  Function to reduce the vector speed to check if there is a collision between the rectangle and the level background
*
*  \param  pRect1  a pointer to the rectangle which is moving
*  \param  pRect2  a pointer to the rectangle which is not moving
*  \param  vx      the movement vector on X you want to move the rectangle 1
*  \param  vy      the movement vector on Y you want to move the rectangle 1
*  \param  NewVx   the new movement vector on X, which might get modified by the function, you MUST sent NULL as a parameter
*  \param  NewVy   the new movement vector on Y, which might get modified by the function, you MUST sent NULL as a parameter
*  \return none
*/
void Kr_Collision_RectAffine(SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy, Sint32 *NewVx, Sint32 *NewVy)
{
	Sint32 i;
	SDL_Rect test;
	test = *pRect1;
	for (i = 0; i<UTIL_ABS(vx); i++)
	{
		if (Kr_Collision_RectTry(&test, pRect2, UTIL_SGN(vx), 0, NewVx, NewVy) == TRUE) break;
		test.x += UTIL_SGN(vx);
	}
	for (i = 0; i<UTIL_ABS(vy); i++)
	{
		if (Kr_Collision_RectTry(&test, pRect2, 0, UTIL_SGN(vy), NewVx, NewVy) == TRUE) break;
		test.y += UTIL_SGN(vy);
	}
}

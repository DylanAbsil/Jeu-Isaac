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
*  \fn     Uint32 Kr_CollisionLevel_Move(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy)
*  \brief  Function to move a rectangle on a level (Recursif !)
*
*  \param  pLevel a pointer to a the level structure
*  \param  pRect1 a pointer to the rectangle you want to move
*  \param  vx     the vector on X
*  \param  vy     the vector on Y
*  \return TRUE if the rectangle and the map are colliding, FALSE otherwise
*/
Boolean Kr_CollisionLevel_Move(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy)
{
	/* Gestion des dépassements très rapides, pour ne pas passer au dela du mur*/
	if (UTIL_ABS(vx) >= pLevel->pLevel_Tileset->iTilesWidth || UTIL_ABS(vy) >= pLevel->pLevel_Tileset->iTilesHeight) // Prendre valeur absolu sinon cela fonctionnera pas pour des vecteurs négatifs
	{
		Kr_CollisionLevel_Move(pLevel, pRect1, vx / 2, vy / 2);
		Kr_CollisionLevel_Move(pLevel, pRect1, vx - vx / 2, vy - vy / 2);
		return FALSE;
	}
	if (Kr_CollisionLevel_TryMove(pLevel, pRect1, vx, vy) == TRUE) return FALSE;

	Kr_CollisionLevel_Affine(pLevel, pRect1, vx, vy);
	return TRUE;
}

/*!
*  \fn     Kr_CollisionLevel_IsCollisionDecor(Kr_Level *pLevel, SDL_Rect *pRect1)
*  \brief  Function to detect if the rectangle is colliding with the level tiles
*
*  \param  pLevel a pointer to a the level structure
*  \param  pRect1  a pointer to the rectangle you want to test
*  \return TRUE if the two rectangle are colliding, FALSE otherwise
*/
Boolean Kr_CollisionLevel_IsCollisionDecor(Kr_Level *pLevel, SDL_Rect *pRect1)
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
			//Kr_Log_Print(KR_LOG_INFO, "iNumTile: %d \n", iNumTile);

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
*  \fn     Boolean Kr_CollisionLevel_TryMove(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy)
*  \brief  Function to try to move a rectangle with a certain vector speed and check if it's colliding with the level tiles
*
*  \param  pLevel a pointer to a the level structure
*  \param  pRect1  a pointer to the first rectangle you want to test
*  \param  vx      the vector on X
*  \param  vy      the vector on Y
*  \return TRUE if the two rectangle are NOT colliding, FALSE otherwise
*/
Boolean Kr_CollisionLevel_TryMove(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy)
{
	SDL_Rect test;
	test = *pRect1;
	test.x += vx;
	test.y += vy;
	if (Kr_CollisionLevel_IsCollisionDecor(pLevel, &test) == FALSE)
	{
		//*pRect1 = test;
		return TRUE;
	}
	return FALSE;
}


/*!
*  \fn     void Kr_CollisionLevel_Affine(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy)
*  \brief  Function to reduce the vector speed to check if there is a collision between the rectangle and the level tiles
*
*  \param  pLevel a pointer to a the level structure
*  \param  pRect1  a pointer to the first rectangle you want to test
*  \param  vx      the vector on X
*  \param  vy      the vector on Y
*  \return none
*/
void Kr_CollisionLevel_Affine(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy)
{
	Sint32 i;
	for (i = 0; i<UTIL_ABS(vx); i++)
	{
		if (Kr_CollisionLevel_TryMove(pLevel, pRect1, UTIL_SGN(vx), 0) == FALSE)
			break;
	}
	for (i = 0; i<UTIL_ABS(vy); i++)
	{
		if (Kr_CollisionLevel_TryMove(pLevel, pRect1, 0, UTIL_SGN(vy)) == FALSE)
			break;
	}
}









/*!
*  \fn     Boolean Kr_CollisionRect_Move(SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy)
*  \brief  Function to check if two rectangle are colliding
*
*  \param  pRect1 a pointer to the first rectangle which is moving
*  \param  pRect2 a pointer to the second rectangle which is not moving
*  \param  vx     the vector on X
*  \param  vy     the vector on Y
*  \return TRUE if the rectangles are colliding, FALSE otherwise
*/
Boolean Kr_CollisionRect_Move(SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy)
{
	/* Gestion des dépassements très rapides, pour ne pas passer au dela du mur*/
	if (UTIL_ABS(vx) >= pRect2->w || UTIL_ABS(vy) >= pRect2->h) // Prendre valeur absolu sinon cela fonctionnera pas pour des vecteurs négatifs
	{
		Kr_CollisionRect_Move(pRect1, pRect2, vx / 2, vy / 2);
		Kr_CollisionRect_Move(pRect1, pRect2, vx - vx / 2, vy - vy / 2);
		return FALSE;
	}
	if (Kr_CollisionRect_TryMove(pRect1, pRect2, vx, vy) == TRUE) return FALSE;

	Kr_CollisionRect_Affine(pRect1, pRect2, vx, vy);
	return TRUE;
}


/*!
*  \fn     Boolean Kr_CollisionRect_TryMove(SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy)
*  \brief  Function to try to move a rectangle with a certain vector speed and check if it's colliding with the level tiles
*
*  \param  pRect1 a pointer to the first rectangle
*  \param  pRect2 a pointer to the second rectangle
*  \param  vx      the vector on X
*  \param  vy      the vector on Y
*  \return TRUE if the two rectangle are NOT colliding, FALSE otherwise
*/
Boolean Kr_CollisionRect_TryMove(SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy)
{
	SDL_Rect test;
	test = *pRect1;
	test.x += vx;
	test.y += vy;
	if (((test.x >= pRect2->x) && (test.x <= (pRect2->x + pRect2->w))) ||
		((test.y >= pRect2->y) && (test.y <= (pRect2->y + pRect2->h))))
	{
		return TRUE;
	}
	return FALSE;
}


/*!
*  \fn     void Kr_CollisionRect_Affine(SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy)
*  \brief  Function to reduce the vector speed to check if there is a collision between the rectangle and the level tiles
*
*  \param  pRect1 a pointer to the first rectangle
*  \param  pRect2 a pointer to the second rectangle
*  \param  vx      the vector on X
*  \param  vy      the vector on Y
*  \return none
*/
void Kr_CollisionRect_Affine(SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy)
{
	Sint32 i;
	for (i = 0; i<UTIL_ABS(vx); i++)
	{
		if (Kr_CollisionRect_TryMove(pRect1, pRect2, UTIL_SGN(vx), 0) == FALSE)
			break;
	}
	for (i = 0; i<UTIL_ABS(vy); i++)
	{
		if (Kr_CollisionRect_TryMove(pRect1, pRect2, 0, UTIL_SGN(vy)) == FALSE)
			break;
	}
}
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

#ifndef __LEVEL_STATE_H__
#define __LEVEL_STATE_H__


#include "entite.h"


#define LEVEL_MAX_ENTITY 10


typedef struct {
	char		   *szLevelStName;		/* Name of the level  */
	Sint32          iLevelNum;          /* Number of the level */
	Kr_Level	   *pLevel;				/* Pointer to the level associated */
	Entity		  **aEntityLevel;		/* Array containing pointers on all entities currently in the level */
	Kr_Sprite	  **aSpriteLevel;		/* Array containing pointers on all sprites related to the entites */
	SDL_Rect	  **aRectPositionEntity;/* Array containing pointers on all rectPosition related to the sprites of entities */
	Uint32			iNbEntities;		/* Number of entities currently in the map (the player not include) */
}Level_State;

Level_State *   Level_State_Init();
Boolean			Level_State_Load(Level_State *pLevelSt, Kr_Level *pLevel, SDL_Renderer *pRenderer);
void			Level_State_Free(Level_State *pLevelSt);

Boolean			drawAllEntities(Level_State *pLevelSt, SDL_Renderer *pRenderer);
Boolean			updateAllEntities(Level_State *pLevelSt, Entity *pPlayer, SDL_Renderer *pRenderer);

Uint32			Kr_Level_Interraction(Kr_Level *pLevel, Entity *pPlayer);

#endif /* __LEVEL_STATE__ */
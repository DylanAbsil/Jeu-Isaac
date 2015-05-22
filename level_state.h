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
#include "kr_level.h"
#include "bombe.h"


#define LEVEL_MAX_ENTITY 10


typedef struct {
	Kr_Level		*pLevel;			/* Pointer to the level associated */
	Entity			*pPlayer;			/* Pointer to the player */
	ListEnt			*plEnt;				/* List containing pointers on all entities currently in the level*/
	Uint32			iNbEntities;			/* Int containing the number of entites currently in the level */
}Level_State;

Level_State *   Level_State_Init(Entity *pPlayer);
Boolean			Level_State_Load(Level_State *pLevelSt, Kr_Level *pLevel, SDL_Renderer *pRenderer);
void			Level_State_Free(Level_State *pLevelSt, Boolean bFreePlayer);

Boolean			drawAllEntities(Level_State *pLevelSt, SDL_Renderer *pRenderer);
Uint32			updateAllEntities(SDL_Renderer *pRenderer, Level_State *pLevelSt, Kr_Input myEvent);

Uint32			updateEntity(SDL_Renderer *pRenderer, Level_State *pLevelSt, Kr_Input myEvent, Entity *pEntity, Boolean bIsPlayer);
Boolean			updateProjectilesWeapon(SDL_Renderer *pRenderer, Level_State *pLevelSt, Weapon *pWeapon);

Uint32			Kr_Level_Interraction(Kr_Level *pLevel, Entity *pPlayer);
Uint32		    GenerateRandomVector(Sint32 *pMovex, Sint32 *pMovey, Uint32 iMin, Uint32 iMax, Entity *pEntity, Kr_Level *pLevel, Entity *pPlayer, Uint32 iWait, Uint32 iRatio);
Boolean			Level_State_SaveLevel(Level_State *pCurrentLevelState);
Uint32			Level_State_Bomb_Detect(Level_State *pLevelSt, Bombe *pBombe);
void			Level_State_Recompense(Level_State *pLevelSt, Boolean bRand, Uint32 iRand);
#endif /* __LEVEL_STATE__ */
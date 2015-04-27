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
	Kr_Level	   *pLevel;				/* Pointer to the level associated */
	Entity		   *pPlayer;			/* Pointer to the player */
	Entity		  **aEntityLevel;		/* Array containing pointers on all entities currently in the level*/
	Uint32			iNbEntities;		/* Number of entities currently in the map (the player not include) */

}Level_State;

Level_State *   Level_State_Init(Entity *pPlayer);
Boolean			Level_State_Load(Level_State *pLevelSt, Kr_Level *pLevel, SDL_Renderer *pRenderer);
void			Level_State_Free(Level_State *pLevelSt, Boolean bFreePlayer);

Boolean			drawAllEntities(Level_State *pLevelSt, SDL_Renderer *pRenderer);
Boolean			updateAllEntities(SDL_Renderer *pRenderer, Level_State *pLevelSt, Kr_Input myEvent);

Boolean			updateEntity(SDL_Renderer *pRenderer, Level_State *pLevelSt, Kr_Input myEvent, Entity *pEntity, Boolean bIsPlayer);
Boolean			updateProjectilesWeapon(SDL_Renderer *pRenderer, Level_State *pLevelSt, Weapon *pWeapon);

Uint32			Kr_Level_Interraction(Kr_Level *pLevel, Entity *pPlayer);

#endif /* __LEVEL_STATE__ */
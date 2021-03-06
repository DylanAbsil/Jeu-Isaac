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
/* Herrou        | 22/04/2015 | Cr?ation																			*/
/* ===============================================================================================================  */

#ifndef __KR_COLLISION_H__
#define __KR_COLLISION_H__

#include "kr_common.h"
#include "kr_util.h"
#include "kr_log.h"
#include "kr_level.h"

/* Gestion des collisions */

Uint32  Kr_CollisionRect_Move(SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy);
Boolean Kr_CollisionRect_Check(SDL_Rect *pRect2, SDL_Rect *pRect1);
Boolean Kr_CollisionRect_TryMove(SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy);
void    Kr_CollisionRect_Affine(SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy);

Uint32  Kr_Collision(Kr_Level *pLevel, SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy, Sint32 *NewVx, Sint32 *NewVy);
Boolean Kr_Collision_RectDetect(SDL_Rect *pRect2, SDL_Rect *pRect1);
Boolean Kr_Collision_RectTry(SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy, Sint32 *NewVx, Sint32 *NewVy);
void    Kr_Collision_RectAffine(SDL_Rect *pRect1, SDL_Rect *pRect2, Sint32 vx, Sint32 vy, Sint32 *NewVx, Sint32 *NewVy);
Boolean Kr_Collision_LevelDetect(Kr_Level *pLevel, SDL_Rect *pRect1);
Boolean Kr_Collision_LevelTry(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy, Sint32 *NewVx, Sint32 *NewVy);
void    Kr_Collision_LevelAffine(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy, Sint32 *NewVx, Sint32 *NewVy);
#endif /* __KR_COLLISION_H__ */
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

#ifndef __KR_COLLISION_H__
#define __KR_COLLISION_H__

#include "kr_common.h"
#include "kr_util.h"
#include "kr_log.h"
#include "kr_level.h"

/* Gestion des collisions */
Boolean  Kr_CollisionLevel_Move(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy);
Boolean Kr_CollisionLevel_IsCollisionDecor(Kr_Level *pLevel, SDL_Rect *pRect1);
Boolean Kr_CollisionLevel_TryMove(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy);
void    Kr_CollisionLevel_Affine(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy);


#endif /* __KR_COLLISION_H__ */
/* ========================================================================= */
/*!
* \file    weapon.c
* \brief   Contains the structure to handle the weapons and the projectiles.
* \author  Alexandre Robin
* \version 1.0
* \date    20 Avril 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* ========================================================================= */

#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "kr_common.h"
#include "kr_sprite.h"

#define PROJECTILE_SPEED 10

typedef struct {
	char		*strNamePrj;
	Sint32	    iDamagePrj;
	Uint32	    iSpeedPrj;
	Direction	direction;
	Kr_Sprite	*pSprProjectile;
	Sint32		iCoordPrj_XStart;
	Sint32		iCoordPrj_YStart;
	Sint32		iCoordPrj_XCurrent;
	Sint32		iCoordPrj_YCurrent;
	Sint32		iCoordPrj_XEnd;
	Sint32		iCoordPrj_YEnd;
}Projectile;


typedef struct {
	char		*strNameWeapon;
	char		*strNameProjectile;
	Uint32		iRangeWeapon;
	Sint32		iMunitionWeapon;
	Sint32		iDamageWeapon;
	Uint32	    iSpeedPrj;
	Uint32		inbProjectile;
	Projectile	**aCurrentProjectiles;
}Weapon;

Projectile* Projectile_Init(char *strProjName);
Boolean		Projectile_Load(Projectile *pProj, Weapon *pWeapon, Direction dir, Uint32 speed, SDL_Rect *pRect, SDL_Renderer *pRenderer);
void		Projectile_Free(Projectile *pProj);
Boolean		Projectile_Draw(SDL_Renderer *pRenderer, Projectile *pProj);

Boolean		UpdateProjectile(Projectile *pProj);

Weapon *	Weapon_Init(char *strWeaponName);
Boolean		Weapon_Load(Weapon *weapon, char *strProjName, Uint32 range, Uint32 munition, Uint32 damage);
void		Weapon_Free(Weapon *weapon);

Boolean		Shoot(Weapon *pWeapon);
Boolean		UpdateAllProjectiles(Weapon *pWeapon);


#endif
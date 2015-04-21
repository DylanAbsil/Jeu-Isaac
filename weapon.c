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

#include "weapon.h"

Projectile* Projectile_Init(char *strProjName){
	Uint32 iNameLen = strlen(strProjName);
	Projectile * newProj = (Projectile *)UTIL_Malloc(sizeof(Projectile));

	newProj->strNamePrj = UTIL_CopyStr(strProjName, iNameLen);
	newProj->iDamagePrj = 0;
	newProj->iSpeedPrj = 0;
	newProj->iCoordPrj_XStart = 0;
	newProj->iCoordPrj_YStart = 0;
	newProj->iCoordPrj_XCurrent = 0;
	newProj->iCoordPrj_YCurrent = 0;
	newProj->iCoordPrj_XEnd = 0;
	newProj->iCoordPrj_YEnd = 0;

	return newProj;
}

Boolean	Projectile_Load(Projectile *pProj, Weapon *pWeapon, Direction dir, Uint32 speed, SDL_Rect *pRect, SDL_Renderer *pRenderer){
	Kr_Sprite	*pSprProj = Kr_Sprite_Init(pProj->strNamePrj);
	SDL_Rect	*pRectProj;

	pProj->iDamagePrj = pWeapon->iDamageWeapon;
	pProj->iSpeedPrj = pWeapon->iSpeedPrj;
	pProj->iCoordPrj_XStart = pRect->x;
	pProj->iCoordPrj_YStart = pRect->y;
	pProj->iCoordPrj_XCurrent = pRect->x;
	pProj->iCoordPrj_YCurrent = pRect->y;
	pProj->direction = dir;
	switch (dir)
	{
	case nord:
		pProj->iCoordPrj_XEnd = pRect->x;
		pProj->iCoordPrj_YEnd -= pWeapon->iRangeWeapon;
		pRectProj->h = 10;
		pRectProj->w = 3;
		Kr_Sprite_Load(pSprProj, 138, 35, 1, pRectProj, pRenderer);
		break;
	case est:
		pProj->iCoordPrj_XEnd += pWeapon->iRangeWeapon;
		pProj->iCoordPrj_YEnd = pRect->y;
		pRectProj->h = 3;
		pRectProj->w = 10;
		Kr_Sprite_Load(pSprProj, 35, 138, 1, pRectProj, pRenderer);
		break;
	case sud:
		pProj->iCoordPrj_XEnd = pRect->x;
		pProj->iCoordPrj_YEnd += pWeapon->iRangeWeapon;
		pRectProj->h = 10;
		pRectProj->w = 3;
		Kr_Sprite_Load(pSprProj, 138, 35, 1, pRectProj, pRenderer);
		break;
	case ouest:
		pProj->iCoordPrj_XEnd -= pWeapon->iRangeWeapon;
		pProj->iCoordPrj_YEnd = pRect->y;
		pRectProj->h = 3;
		pRectProj->w = 10;
		Kr_Sprite_Load(pSprProj, 35, 138, 1, pRectProj, pRenderer);
		break;
	}
	pProj->pSprProjectile = pSprProj;
	
	return TRUE;
}

void Projectile_Free(Projectile *pProj){
	UTIL_Free(pProj->strNamePrj);
	Kr_Sprite_Free(pProj->pSprProjectile);
	UTIL_Free(pProj);
}

Boolean	Projectile_Draw(SDL_Renderer *pRenderer, Projectile *pProj){
	if (SDL_RenderCopy(pRenderer, pProj->pSprProjectile->pTextureSprite, NULL, pProj->pSprProjectile->pRectPosition) == -1){
		return FALSE;
	}
	return TRUE;
}




Weapon *  Weapon_Init(char *strWeaponName){
	Uint32 iNameLen = strlen(strWeaponName);
	Weapon * newWeap = (Weapon *)UTIL_Malloc(sizeof(Weapon));

	newWeap->strNameWeapon = UTIL_CopyStr(strWeaponName, iNameLen);
	newWeap->iDamageWeapon = 0;
	newWeap->iMunitionWeapon = 0;
	newWeap->iRangeWeapon = 0;
	newWeap->inbProjectile = 0;
	newWeap->aCurrentProjectiles = NULL;

	return newWeap;
}

Boolean	 Weapon_Load(Weapon *weapon, char *strProjName, Uint32 range, Uint32 munition, Uint32 damage){
	Uint32 iNameLen2 = strlen(strProjName);

	weapon->strNameProjectile = UTIL_CopyStr(strProjName, iNameLen2);
	weapon->iRangeWeapon = range;
	weapon->iMunitionWeapon = munition;
	weapon->iDamageWeapon = damage;
	weapon->aCurrentProjectiles = (Projectile**)UTIL_Malloc(sizeof(Projectile*));

	return TRUE;
}

void Weapon_Free(Weapon *weapon){
	UTIL_Free(weapon->strNameWeapon);
	UTIL_Free(weapon->strNameProjectile);
	UTIL_Free(weapon);
}

Boolean	UpdateAllProjectiles(Weapon *pWeapon){
	int i = 0;
	Projectile **aProj = pWeapon->aCurrentProjectiles;
	for (i = 0; i < pWeapon->inbProjectile; i++)
	{
		switch ((*(aProj + i))->direction)
		{
		case nord:
			(*(aProj + i))->pSprProjectile->pRectPosition->y -= PROJECTILE_SPEED;
			(*(aProj + i))->iCoordPrj_YCurrent -= PROJECTILE_SPEED;
			if ((*(aProj + i))->iCoordPrj_YCurrent < (*(aProj + i))->iCoordPrj_YEnd)
				Projectile_Free(*(aProj + i));
			break;
		case est:
			(*(aProj + i))->pSprProjectile->pRectPosition->x += PROJECTILE_SPEED;
			(*(aProj + i))->iCoordPrj_XCurrent += PROJECTILE_SPEED;
			if ((*(aProj + i))->iCoordPrj_XCurrent >(*(aProj + i))->iCoordPrj_XEnd)
				Projectile_Free(*(aProj + i));
			break;
		case sud:
			(*(aProj + i))->pSprProjectile->pRectPosition->y += PROJECTILE_SPEED;
			(*(aProj + i))->iCoordPrj_YCurrent += PROJECTILE_SPEED;
			if ((*(aProj + i))->iCoordPrj_YCurrent > (*(aProj + i))->iCoordPrj_YEnd)
				Projectile_Free(*(aProj + i));
			break;
		case ouest:
			(*(aProj + i))->pSprProjectile->pRectPosition->x -= PROJECTILE_SPEED;
			(*(aProj + i))->iCoordPrj_XCurrent -= PROJECTILE_SPEED;
			if ((*(aProj + i))->iCoordPrj_XCurrent < (*(aProj + i))->iCoordPrj_XEnd)
				Projectile_Free(*(aProj + i));
			break;
		}
	}
	return TRUE;
}
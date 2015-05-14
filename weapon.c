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



/* ========================================================================= */
/*							GESTION DES PROJECTILES							 */	
/* ========================================================================= */

/*!
*  \fn     Projectile* Projectile_Init(char *strProjName)
*  \brief  Function to init a projectile
*
*  \todo   use this function at first to create a projectile then load
*
*  \param szFileName the name of the projectile file must be allocated in the initialization
*  \return Projectile* a pointer to the new created projectile
*/
Projectile* Projectile_Init(char *strProjName){
	Uint32 iNameLen = strlen(strProjName);
	Projectile * newProj = (Projectile *)UTIL_Malloc(sizeof(Projectile));

	newProj->strNamePrj = UTIL_CopyStr(strProjName, iNameLen);
	newProj->iDamagePrj = 0;
	newProj->iSpeedPrj = 0;
	newProj->iCoordPrj_XEnd = 0;
	newProj->iCoordPrj_YEnd = 0;

	return newProj;
}

/*!
*  \fn     Boolean	Projectile_Load(Projectile *pProj, Weapon *pWeapon, Direction dir, Uint32 speed, SDL_Rect *pRect, SDL_Renderer *pRenderer)
*  \brief  Function to load a projectile
*
*  \todo   use this function after the projectile had been initialized
*
*  \param  pProj		a pointer to the projectile you want to load
*  \param  pWeapon		a pointer to the weapon which will use the projectile
*  \param  dir			the direction to where the projectile is sent
*  \param  speed		the movespeed of the projectile
*  \param  pRec			a pointer to a rect indicating the position from where the projectile is fired
*  \param  pRenderer    a pointer to the renderer
*  \return TRUE if everything is ok, FALSE otherwise
*/
Boolean	Projectile_Load(Projectile *pProj, Weapon *pWeapon, Direction dir, Uint32 speed, SDL_Rect *pRect, SDL_Renderer *pRenderer){
	Kr_Sprite	*pSprProj = Kr_Sprite_Init(pProj->strNamePrj);
	SDL_Rect	*pRectProj = (SDL_Rect*)UTIL_Malloc(sizeof(SDL_Rect));
	Sint32		entityMiddleX = UTIL_FindMiddle(pRect->x, pRect->x + pRect->w);
	Sint32		entityMiddleY = UTIL_FindMiddle(pRect->y, pRect->y + pRect->h);

	pProj->iDamagePrj = pWeapon->iDamageWeapon;
	pProj->iSpeedPrj = pWeapon->iSpeedPrj;
	pProj->direction = dir;
	switch (dir)
	{
	case nord:
		pRectProj->h = 40;
		pRectProj->w = 10;
		pRectProj->x = entityMiddleX - pRectProj->w / 2;
		pRectProj->y = pRect->y - pRectProj->h;
		pProj->iCoordPrj_XEnd = pRectProj->x;
		pProj->iCoordPrj_YEnd = pRectProj->y - pWeapon->iRangeWeapon;
		
		Kr_Sprite_Load(pSprProj, dir, 138, 35, 1, pRectProj, pRenderer);
		break;
	case est:
		pRectProj->h = 10;
		pRectProj->w = 40;
		pRectProj->x = pRect->x + pRect->w;
		pRectProj->y = entityMiddleY - pRectProj->h / 2 ;
		pProj->iCoordPrj_XEnd = pRectProj->x + pWeapon->iRangeWeapon;
		pProj->iCoordPrj_YEnd = pRectProj->y;

		Kr_Sprite_Load(pSprProj, dir, 35, 138, 1, pRectProj, pRenderer);
		break;
	case sud:
		pRectProj->h = 40;
		pRectProj->w = 10;
		pRectProj->x = entityMiddleX - pRectProj->w / 2;
		pRectProj->y = pRect->y + pRect->h;
		pProj->iCoordPrj_XEnd = pRectProj->x;
		pProj->iCoordPrj_YEnd = pRectProj->y + pWeapon->iRangeWeapon;
		Kr_Sprite_Load(pSprProj, dir, 138, 35, 1, pRectProj, pRenderer);
		break;
	case ouest:
		pRectProj->h = 10;
		pRectProj->w = 40;
		pRectProj->x = pRect->x - pRectProj->w;
		pRectProj->y = entityMiddleY - pRectProj->h / 2;
		pProj->iCoordPrj_XEnd = pRectProj->x - pWeapon->iRangeWeapon;
		pProj->iCoordPrj_YEnd = pRectProj->y;
		Kr_Sprite_Load(pSprProj, dir, 35, 138, 1, pRectProj, pRenderer);
		break;
	}
	pProj->pSprProjectile = pSprProj;
	Kr_Log_Print(KR_LOG_INFO, "The projectile %s of %d x %d in (%d ; %d ) has been loaded\n", pProj->strNamePrj, pProj->pSprProjectile->pRectPosition->h, pProj->pSprProjectile->pRectPosition->w, pProj->pSprProjectile->pRectPosition->x, pProj->pSprProjectile->pRectPosition->y);
	return TRUE;
}

/*!
*  \fn     void Projectile_Free(Projectile *pProj)
*  \brief  Function to freed a Projectile and his sprite
*
*  \param  pProj a pointer to the Projectile texture to free
*  \return none
*/
void Projectile_Free(Projectile *pProj){
	UTIL_Free(pProj->strNamePrj);
	Kr_Sprite_Free(pProj->pSprProjectile);
	UTIL_Free(pProj);
}

/*!
*  \fn     Boolean	Projectile_Draw(SDL_Renderer *pRenderer, Projectile *pProj)
*  \brief  Function to draw a projectile
*
*  \todo   Update this function when the projectile have moved
*
*  \param	pRenderer	a pointer to the renderer
*  \param	pProj		a pointer to the projectile to draw
*  \return boolean if the projectile has been draw on the screen or not
*/
Boolean	Projectile_Draw(SDL_Renderer *pRenderer, Projectile *pProj){
	if (SDL_RenderCopy(pRenderer, pProj->pSprProjectile->pTextureSprite, NULL, pProj->pSprProjectile->pRectPosition) == -1){
		return FALSE;
	}
	return TRUE;
}

/* ========================================================================= */
/*					GESTION DES LISTES DE PROJECTILES						 */
/* ========================================================================= */

/* Pour la documentation et/ou la compréhension de cette partie revoyer votre cours */

NodeListProj *	newNodeListProj(Projectile *p, NodeListProj *n){
	NodeListProj * new = (NodeListProj *)UTIL_Malloc(sizeof(NodeListProj));
	new->p = p;
	new->next = n;
	return new;
}

void deleteNodeListProj(NodeListProj * n){
	Projectile_Free(n->p);
	UTIL_Free(n);
}

void initList(ListProj *lProj){
	lProj->first = lProj->current = lProj->last = NULL;
}

void deleteList(ListProj *lProj){
	setOnFirst(lProj);
	while (lProj->current != NULL){
		next(lProj);
		deleteNodeListProj(lProj->first);
		lProj->first = lProj->current;
	}
}

Boolean emptyList(ListProj *lProj){
	if (lProj->first == NULL) return TRUE;
	else return FALSE;
}

int first(ListProj *lProj){
	if (lProj->current->next == lProj->first->next) return TRUE;
	else return FALSE;
}

int last(ListProj *lProj){
	if (lProj->current->next == lProj->last->next) return TRUE;
	else return FALSE;
}

int outOfList(ListProj *lProj){
	if ((lProj->current == NULL) && !emptyList(lProj)) return TRUE;
	else return FALSE;
}

void setOnFirst(ListProj *lProj){
	lProj->current = lProj->first;
}

void setOnLast(ListProj *lProj){
	lProj->current = lProj->last;
}

void next(ListProj *lProj){
	if (emptyList(lProj) == FALSE)
		lProj->current = lProj->current->next;
}

Projectile * getCurrentProj(ListProj *lProj){
	return lProj->current->p;
}

void deleteCurrent(ListProj *lProj, Boolean *nextL){
	NodeListProj *nodeTmp = lProj->current;

	if (lProj->first == lProj->last){
		lProj->current = lProj->first = lProj->last = NULL;
		*nextL = TRUE;
	}
	else if (first(lProj) == TRUE){
		next(lProj);
		lProj->first = lProj->current;
		*nextL = TRUE;
	}
	else if (last(lProj) == TRUE){
		setOnFirst(lProj);
		while (lProj->current->next != nodeTmp)
			next(lProj);
		lProj->current->next = nodeTmp->next;
		lProj->last = lProj->current;
		*nextL = FALSE;
	}
	else{
		setOnFirst(lProj);
		while (lProj->current->next != nodeTmp)
			next(lProj);
		lProj->current->next = nodeTmp->next;
		*nextL = FALSE;
	}
	deleteNodeListProj(nodeTmp);
}

Boolean	insertLast(ListProj *lProj, Projectile *p){
	NodeListProj * new = newNodeListProj(p, NULL);
	if (emptyList(lProj) == TRUE){
		lProj->first = new;
		lProj->current = new;
	}
	else
		lProj->last->next = new;
	lProj->last = new;
	return TRUE;
}

/*!
*  \fn     Boolean drawAllProjectiles(ListProj *lProj, SDL_Renderer *pRenderer)
*  \brief  Function to draw all the projectiles contained in a list in the struct weapon
*
*  \param	lProj		a pointer to the list of projectiles to draw
*  \param	pRenderer	a pointer to the renderer
*  \return boolean if all the projectiles have been draw on the screen or not
*/
Boolean drawProjectilesWeapon(ListProj *lProj, SDL_Renderer *pRenderer){
	if (emptyList(lProj) == FALSE){
		setOnFirst(lProj);
		while (lProj->current != NULL){
			Projectile_Draw(pRenderer, lProj->current->p);
			next(lProj);
		}
		return TRUE;
	}
	else return FALSE;
}


/* ========================================================================= */
/*							GESTION DES WEAPONS  							 */
/* ========================================================================= */

/*!
*  \fn     Weapon *  Weapon_Init(char *strWeaponName)
*  \brief  Function to init a weapon
*
*  \param szFileName the name of theweapon file must be allocated in the initialization
*  \return Weapon* a pointer to the new created weapon
*/
Weapon *  Weapon_Init(char *strWeaponName){
	Uint32 iNameLen = strlen(strWeaponName);
	Weapon * newWeap = (Weapon *)UTIL_Malloc(sizeof(Weapon));
	static ListProj lProj;
	initList(&lProj);

	newWeap->strNameWeapon = UTIL_CopyStr(strWeaponName, iNameLen);
	newWeap->iDamageWeapon = 0;
	newWeap->iMunitionWeapon = 0;
	newWeap->iRangeWeapon = 0;
	newWeap->iSpeedPrj = 0;
	newWeap->plProjectile = &lProj;

	return newWeap;
}

/*!
*  \fn     Boolean Weapon_Load(Weapon *pWeapon, char *strProjName, Uint32 range, Uint32 munition, Uint32 damage)
*  \brief  Function to load a weapon
*
*  \param  pWeapon		a pointer to the weapon you want to load
*  \param  strProjName  a string indicate what projectile do you want to use with this weapon
*  \param  range		the range of the weapon
*  \param  munition		the munitions currently in the weapon
*  \param  damage		the damage of the weapon
*  \return TRUE if everything is ok, FALSE otherwise
*/
Boolean Weapon_Load(Weapon *pWeapon, char *strProjName, Uint32 range, Uint32 munition, Uint32 damage){
	Uint32 iNameLen = strlen(strProjName);

	pWeapon->strNameProjectile = UTIL_CopyStr(strProjName, iNameLen);
	pWeapon->iRangeWeapon = range;
	pWeapon->iMunitionWeapon = munition;
	pWeapon->iDamageWeapon = damage;
	
	return TRUE;
}

/*!
*  \fn     void Weapon_Free(Weapon *pWeapon)
*  \brief  Function to freed a weapon
*
*  \param  pWeapon a pointer to the weapon to free
*  \return none
*/
void Weapon_Free(Weapon *pWeapon){
	if (pWeapon != NULL){
		UTIL_Free(pWeapon->strNameWeapon);
		UTIL_Free(pWeapon->strNameProjectile);
		UTIL_Free(pWeapon);
	}
}



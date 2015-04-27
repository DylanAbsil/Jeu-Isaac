/* ========================================================================= */
/*!
* \file    entite.c
* \brief   Contains the structure to handle the entities.
* \author  Alexandre Robin
* \version 1.0
* \date    16 Mars 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Robin         | 16/03/2015 | Creation.                                    */
/* Herrou        | 28/03/2015 | Harmoniser les prototypes avec le projet     */
/*               |            | Mise en forme et corrections                 */
/*               |            | Ajout du SDL_Renderer pour updateEntityVector*/
/*               |            | Prévention des inclusions multiples          */
/* Herrou        | 04/04/2015 | Initialisation du nom faite par UTIL_CopyStr */
/*               |            | Le nom du sprite est donnée à Entite_Init    */
/*               |            |    et non pas à Entite_Load                  */
/* Herrou        | 05/04/2015 | Ajout du param Entity à foundDirection       */
/* Herrou        | 24/04/2015 | Suppresion du param iCoordX, on y a accès via*/
/*               |            |  le rectangle du sprite		                 */
/* ========================================================================= */


#include "entite.h"


/*!
*  \fn     Entity * Entity_Init(char* szFileName)
*  \brief  Function to init an entity
*
*  \todo   use this function at first to create an entity then load
*
*  \param szFileName the name of the sprite file must be allocated in the initialization
*  \return Entity* a pointer to the empty created entity
*/
Entity * Entity_Init(char* szFileName){
	Uint32 iNameLen = strlen(szFileName);
	Entity * entite = (Entity *)malloc(sizeof(Entity));
	
	entite->strEntityName = UTIL_CopyStr(szFileName, iNameLen);
	entite->pSprEntity = NULL;
	entite->state = normal;
	entite->iEntityLife = 0;
	entite->iSpeedEntity = 0;
	entite->iTempoAnim = 0;
	entite->iTempoAtk = 0;
	return entite;
}

/*!
*  \fn     void Entity_Load(Entity *entite, Uint32 life, Uint32 armor, Kr_Sprite *sprite)
*  \brief  Function to load the entity
*
*  \todo   use this function after the entity had been inited
*
*  \param entite a pointer to an entity
*  \param life his life with an integer
*  \param armor his armor with an integer
*  \param sprite a pointer to the sprite of the entity
*  \return boolean it verify if the load is correct or not
*/
Boolean Entity_Load(Entity *entite,  Uint32 life, Uint32 armor, Kr_Sprite *sprite){
	entite->iEntityLife = life;
	entite->iArmor = armor;
	entite->pSprEntity = sprite;
	if (entite->pSprEntity == NULL){
//		Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite %s in the entity %s !\n", sprite->strSpriteName, entite->strEntityName);
		return FALSE;
	}
	entite->state = normal;
	entite->direction = sud;
	entite->mouvement = 0;
	entite->iSpeedEntity = MOB_MOVESPEED;
	entite->iTempoAtk = ATTACK_SPEED;

	Entity_Log(entite);
//	Kr_Log_Print(KR_LOG_INFO, "Entity %s with sprite %s has been loaded !\n", entite->strEntityName, entite->pSprEntity->strName);
	return TRUE;
}

/*!
*  \fn     void Entity_Free(Entity *entite)
*  \brief  Function to freed an entity
*
*  \param  pEntity a pointer to the Entity to free
*  \return none
*/
void Entity_Free(Entity *entite){
	if (entite != NULL){
		UTIL_Free(entite->strEntityName);
		Kr_Sprite_Free(entite->pSprEntity);
		UTIL_Free(entite);
	}
}

/*!
*  \fn     void  Entity_Log(Entity *pEntity)
*  \brief  Function to log an entity
*
*  \param  pEntity a pointer to the Entity to free
*  \return none
*/
void  Entity_Log(Entity *pEntity)
{
	Kr_Log_Print(KR_LOG_INFO, "Entity loaded : %s\n", pEntity->strEntityName);
	Kr_Log_Print(KR_LOG_INFO, "			Life		: %d\n", pEntity->iEntityLife);
	Kr_Log_Print(KR_LOG_INFO, "			Armor		: %d\n", pEntity->iArmor);
	Kr_Log_Print(KR_LOG_INFO, "			CoordX		: %d\n", pEntity->pSprEntity->pRectPosition->x);
	Kr_Log_Print(KR_LOG_INFO, "			CoordY		: %d\n", pEntity->pSprEntity->pRectPosition->y);
	Kr_Log_Print(KR_LOG_INFO, "			iSpeedEntity: %d\n", pEntity->iSpeedEntity);
}





/*!
*  \fn     Entity_Draw(SDL_Renderer * renderer, Entity *entite)
*  \brief  Function to draw an entity
*
*  \todo   Update this function when the entity have moved
*
*  \param  pRenderer
*  \param  entite
*  \return boolean if the entite has been draw on the screen or not
*/
Boolean Entity_Draw(SDL_Renderer * pRenderer, Entity *entite){
	SDL_Rect frameToDraw;

	//Création d'un int permettant de sélectionner la bonne frame
	Uint32 largeur = entite->pSprEntity->iFrameWidth / entite->pSprEntity->iNbFrames;

	if ((largeur < 0) || (entite == NULL)){
		Kr_Log_Print(KR_LOG_ERROR, "Impossible to access to the entity\n");
		return FALSE;
	}
	frameToDraw.x = (entite->pSprEntity->iCurrentFrame) * largeur;
	frameToDraw.y = 0;
	frameToDraw.h = entite->pSprEntity->iFrameHeight;
	frameToDraw.w = largeur;
//	Kr_Log_Print(KR_LOG_INFO, "Frame : { x = %d ; y = %d ; h = %d ; w = %d }\n", frameToDraw.x, frameToDraw.y, frameToDraw.h, frameToDraw.w);

	//Affichage de l'entite sur l'écran
	if (SDL_RenderCopy(pRenderer, entite->pSprEntity->pTextureSprite, &frameToDraw, entite->pSprEntity->pRectPosition) == -1){
		Kr_Log_Print(KR_LOG_ERROR, "The entity %s hasn't been draw on the window\n", entite->strEntityName);
		return FALSE;
	}
//	Kr_Log_Print(KR_LOG_INFO, "The entity %s has been draw on the window on coordonates x = %d and y = %d\n", entite->strEntityName, entite->pSprEntity->pRectPosition->x, entite->pSprEntity->pRectPosition->y);
	return TRUE;

}



/*!
*  \fn     void getVector(Kr_Input inEvent, Sint32 *vx, Sint32 *vy)
*  \brief  Function to get the vector of the player
*
*  \param  inEvent Structure which handle the input
*  \param  vx      a pointer to the vector on X
*  \param  vy      a pointer to the vector on Y
*  \return none
*/
void getVector(Kr_Input myEvent, Sint32 *vx, Sint32 *vy){
	*vx = *vy = 0;
	if (myEvent.szKey[SDL_SCANCODE_UP])
		*vy = -MOVESPEED;
	if (myEvent.szKey[SDL_SCANCODE_DOWN])
		*vy = MOVESPEED;
	if (myEvent.szKey[SDL_SCANCODE_LEFT])
		*vx = -MOVESPEED;
	if (myEvent.szKey[SDL_SCANCODE_RIGHT])
		*vx = MOVESPEED;
}

/*!
*  \fn     void getVectorToPlayer(Entity *pEntity, Entity *pPlayer, Sint32 *vx, Sint32 *vy)
*  \brief  Function to get the vector from an entity to the player
*  \param  pEntity pointer to the entity
*  \param  pPlayer pointer to the player
*  \param  vx      a pointer to the vector on X
*  \param  vy      a pointer to the vector on Y
*  \return none
*/
void getVectorToPlayer(Entity *pEntity, Entity *pPlayer, Sint32 *vx, Sint32 *vy){
	Sint32 movex = pPlayer->pSprEntity->pRectPosition->x - pEntity->pSprEntity->pRectPosition->x;
	Sint32 movey = pPlayer->pSprEntity->pRectPosition->x - pEntity->pSprEntity->pRectPosition->x;
	double movez = sqrt(movex*movex + movey*movey);
	double rapport = MOB_MOVESPEED / movez;
	*vx = (Sint32)(rapport * movex);
	*vy = (Sint32)(rapport * movey);
	
}


/*!
*	\fn	    Direction foundDirection(Sint32 vx, Sint32 vy, Entity *pEntity)
*  \brief  Function to get a direction that the entity is looking
*
*  \param vx       a move in x
*  \param vy       a move in y
*  \param pEntity  a pointer to the entity structure
*  \return Direction the direction associated to the vector
*/
Direction foundDirection(Sint32 vx, Sint32 vy, Entity *pEntity){
	Direction newDir = pEntity->direction; // défaut
	if (vy > 0)
		newDir = sud;
	if (vy < 0)
		newDir = nord;
	if (vx > 0)
		newDir = est;
	if (vx < 0)
		newDir = ouest;
	return newDir;
}

/*!
*  \fn     void switchTextureFromDirection(Entity *entite, Direction dir, SDL_Renderer *pRenderer){
*  \brief  Function to switch the texture of the entity according to the new direction
*
*  \param  enite  a pointer to the entity
*  \param  dir	  the new direction
*  \param  pRenderer the renderer
*  \return none
*/
void switchTextureFromDirection(Entity *entite, Direction newDir, SDL_Renderer *pRenderer){
	// Nouveau sprite potentiel suivant la direction
	char newSprFileName[SIZE_MAX_NAME];
	//	Kr_Log_Print(KR_LOG_INFO, "Previous direction : %d\n", entite->direction);

	strcpy(newSprFileName, entite->pSprEntity->strSpriteName); //Nécessaire de l'initialiser même si après la direction change
	switch (newDir){									// Suivant la nouvelle direction :
	case nord:
		if (entite->direction != nord){						// if direction différente
			entite->direction = nord;							//  -> on change
			sprintf(newSprFileName, "sprites/%s_%s.png", entite->strEntityName, "nord"); //on va chercher le bon fichier image
			UTIL_FreeTexture(&entite->pSprEntity->pTextureSprite);							// on libère l'ancienne texture
			entite->pSprEntity->pTextureSprite = UTIL_LoadTexture(pRenderer, newSprFileName, NULL, NULL);	//on load la nouvelle texture asssociéee à la nouvelle direction
			entite->pSprEntity->iFrameWidth = 144;
		}
		break;
	case sud:
		if (entite->direction != sud){
			entite->direction = sud;
			sprintf(newSprFileName, "sprites/%s_%s.png", entite->strEntityName, "sud");
			UTIL_FreeTexture(&entite->pSprEntity->pTextureSprite);
			entite->pSprEntity->pTextureSprite = UTIL_LoadTexture(pRenderer, newSprFileName, NULL, NULL);
			entite->pSprEntity->iFrameWidth = 136;
		}
		break;
	case ouest:
		if (entite->direction != ouest){
			entite->direction = ouest;
			sprintf(newSprFileName, "sprites/%s_%s.png", entite->strEntityName, "ouest");
			UTIL_FreeTexture(&entite->pSprEntity->pTextureSprite);
			entite->pSprEntity->pTextureSprite = UTIL_LoadTexture(pRenderer, newSprFileName, NULL, NULL);
			entite->pSprEntity->iFrameWidth = 160 ;
		}
		break;
	case est:
		if (entite->direction != est){
			entite->direction = est;
			sprintf(newSprFileName, "sprites/%s_%s.png", entite->strEntityName, "est");
			UTIL_FreeTexture(&entite->pSprEntity->pTextureSprite);
			entite->pSprEntity->pTextureSprite = UTIL_LoadTexture(pRenderer, newSprFileName, NULL, NULL);
			entite->pSprEntity->iFrameWidth = 160 ;
		}
		break;
	default:
		break;
	}
	if (entite->pSprEntity->strSpriteName != NULL) 
		UTIL_Free(entite->pSprEntity->strSpriteName); // On va devoir réalloué la taille du string
	Uint32 iNameLen = strlen(newSprFileName);
	entite->pSprEntity->strSpriteName = UTIL_CopyStr(newSprFileName, iNameLen);//on change le nom du sprite (par le lien sprites/image.png pour que ca soit plus clair

//	Kr_Log_Print(KR_LOG_INFO, "Sprite %s has been loaded\n", entite->pSprEntity->strName);
//	Kr_Log_Print(KR_LOG_INFO, "New direction : %d\n", entite->direction);
}

/*  \fn void meleeDamage(Entity *pGiver, Entity *pReceiver)
*  \brief function to inflict damage to an entity by being touched by another entity
*
*  \param	pGiver		a pointer to the giver of the damage
*  \param	pReceiver	a pointer to the receiver of the damage
*  \return none
*/
void meleeDamage(Entity *pGiver, Entity *pReceiver){
	if (pReceiver->iArmor > MOB_INFIGHTING_DAMAGE)
		pReceiver->iArmor -= MOB_INFIGHTING_DAMAGE;
	else if (pReceiver->iArmor > 0){
		Uint32 truedamage = MOB_INFIGHTING_DAMAGE - pReceiver->iArmor;
		pReceiver->iArmor = 0;
		pReceiver->iEntityLife -= truedamage;
	}
	else
		pReceiver->iEntityLife -= MOB_INFIGHTING_DAMAGE;
}

void weaponDamage(Projectile *pProj, Entity *pEntity){
	if (pEntity->iArmor > pProj->iDamagePrj)
		pEntity->iArmor -= pProj->iDamagePrj;
	else if (pEntity->iArmor > 0){
		Uint32 truedamage = pProj->iDamagePrj - pEntity->iArmor;
		pEntity->iArmor = 0;
		pEntity->iEntityLife -= truedamage;
	}
	else
		pEntity->iEntityLife -= pProj->iDamagePrj;
}

/*!
*  \fn     Boolean	Shoot(Kr_Input myEvent, Entity *pEntity, SDL_Renderer *pRenderer)
*  \brief  if the player use zqsd to shoot, load a new projectile
*
*  \param	myEvent	the new event
*  \param	pEntity a pointer to the entity who wanted to fire
*  \param	pRender a pointer to the renderer
*  \return	TRUE if the entity has fired, FALSE otherwise
*/
Boolean	shoot(Kr_Input myEvent, Entity *pEntity, SDL_Renderer *pRenderer){
	if (myEvent.szKey[SDL_SCANCODE_W]){
			if (pEntity->pWeapon->iMunitionWeapon > 0 && pEntity->iTempoAtk == ATTACK_SPEED){
				Projectile * newProj = Projectile_Init(pEntity->pWeapon->strNameProjectile);
				Projectile_Load(newProj, pEntity->pWeapon, nord, pEntity->pWeapon->iSpeedPrj, pEntity->pSprEntity->pRectPosition, pRenderer);
				insertLast(pEntity->pWeapon->plProjectile, newProj);
				pEntity->pWeapon->iMunitionWeapon -= 1;
				pEntity->iTempoAtk = 0;

				Kr_Log_Print(KR_LOG_INFO, "Le projectile %s a été tiré en direction : %d\n", pEntity->pWeapon->plProjectile->last->p->strNamePrj, pEntity->pWeapon->plProjectile->last->p->direction);
				return TRUE;
			}
			else{
				if (pEntity->iTempoAtk < ATTACK_SPEED)
					pEntity->iTempoAtk += 1;
				return FALSE;
			}
	}
	else if (myEvent.szKey[SDL_SCANCODE_A]){
			if (pEntity->pWeapon->iMunitionWeapon > 0 && pEntity->iTempoAtk == ATTACK_SPEED){
				Projectile * newProj = Projectile_Init(pEntity->pWeapon->strNameProjectile);
				Projectile_Load(newProj, pEntity->pWeapon, ouest, pEntity->pWeapon->iSpeedPrj, pEntity->pSprEntity->pRectPosition, pRenderer);
				insertLast(pEntity->pWeapon->plProjectile, newProj);
				pEntity->pWeapon->iMunitionWeapon -= 1;
				pEntity->iTempoAtk = 0;

				Kr_Log_Print(KR_LOG_INFO, "Le projectile %s a été tiré en direction : %d\n", pEntity->pWeapon->plProjectile->last->p->strNamePrj, pEntity->pWeapon->plProjectile->last->p->direction);
				return TRUE;
			}
			else{
				if (pEntity->iTempoAtk < ATTACK_SPEED)
					pEntity->iTempoAtk += 1;
				return FALSE;
			}
	}
	else if (myEvent.szKey[SDL_SCANCODE_S]){
			if (pEntity->pWeapon->iMunitionWeapon > 0 && pEntity->iTempoAtk == ATTACK_SPEED){
				Projectile * newProj = Projectile_Init(pEntity->pWeapon->strNameProjectile);
				Projectile_Load(newProj, pEntity->pWeapon, sud, pEntity->pWeapon->iSpeedPrj, pEntity->pSprEntity->pRectPosition, pRenderer);
				insertLast(pEntity->pWeapon->plProjectile, newProj);
				pEntity->pWeapon->iMunitionWeapon -= 1;
				pEntity->iTempoAtk = 0;

				Kr_Log_Print(KR_LOG_INFO, "Le projectile %s a été tiré en direction : %d\n", pEntity->pWeapon->plProjectile->last->p->strNamePrj, pEntity->pWeapon->plProjectile->last->p->direction);
				return TRUE;
			}
			else{
				if (pEntity->iTempoAtk < ATTACK_SPEED)
					pEntity->iTempoAtk += 1;
				return FALSE;
			}
	}
	else if (myEvent.szKey[SDL_SCANCODE_D]){
			if (pEntity->pWeapon->iMunitionWeapon > 0 && pEntity->iTempoAtk == ATTACK_SPEED){
				Projectile * newProj = Projectile_Init(pEntity->pWeapon->strNameProjectile);
				Projectile_Load(newProj, pEntity->pWeapon, est, pEntity->pWeapon->iSpeedPrj, pEntity->pSprEntity->pRectPosition, pRenderer);
				insertLast(pEntity->pWeapon->plProjectile, newProj);
				pEntity->pWeapon->iMunitionWeapon -= 1;
				pEntity->iTempoAtk = 0;

				Kr_Log_Print(KR_LOG_INFO, "Le projectile %s a été tiré en direction : %d\n", pEntity->pWeapon->plProjectile->last->p->strNamePrj, pEntity->pWeapon->plProjectile->last->p->direction);
				return TRUE;
			}
			else{
				if (pEntity->iTempoAtk < ATTACK_SPEED)
					pEntity->iTempoAtk += 1;
				return FALSE;
			}
	}
	else{
		if (pEntity->iTempoAtk < ATTACK_SPEED)
			pEntity->iTempoAtk += 1;
		return FALSE;
	}
}

/*  \fn Boolean	ChangeWeapon(Entity *pEntity, Weapon *pWeapon)
 *  \brief function to change the weapon of an entity
 *  
 *  \param	pEntity	a pointer the entity which you want to change the weapon
 *  \param	pWeapon	a pointer to the new weapon
 *  \return	TRUE 
 */
Boolean	ChangeWeapon(Entity *pEntity, Weapon *pWeapon){
	pEntity->pWeapon = pWeapon;
	Kr_Log_Print(KR_LOG_INFO, "The entity %s with weapon %s has been changed in %s\n", pEntity->strEntityName, pEntity->pWeapon->strNameWeapon, pEntity->pWeapon->strNameProjectile);
	return TRUE;
}
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
/* Herrou        | 27/04/2015 | switchTextureFromDirection calcul la largueur*/
/*               |            |  de la frame du sprite toute seule           */
/* Herrou        | 01/05/2015 | Fix fans switchTextureFromDirection, fclose  */
/* ========================================================================= */


#include "entite.h"


/*!
*  \fn     Entity * Entity_Init(char* szFileName)
*  \brief  Function to init an entity
*
*  \todo   use this function at first to create an entity then load
*
*  \param  szFileName the name of the sprite file must be allocated in the initialization
*  \return Entity* a pointer to the empty created entity
*/
Entity * Entity_Init(char* szFileName){
	Uint32 iNameLen = strlen(szFileName);
	Entity * entite = (Entity *)malloc(sizeof(Entity));
	if (!entite) return NULL;
	entite->strEntityName = UTIL_CopyStr(szFileName, iNameLen);
	entite->pSprEntity = NULL;
	entite->state = normal;
	entite->iEntityLife = 0;
	entite->iSpeedEntity = 0;
	entite->iTempoAnim = 0;
	entite->iTempoFiringAnim = 0;
	entite->iTempoAtk = 0;
	entite->iTempoMovement = 15;
	entite->bFriendly = TRUE;
	entite->direction = sud;
	entite->mouvement = 0;
	entite->firing = 0;
	entite->iCurrentMoveX = 0;
	entite->iCurrentMoveY = 0;
	entite->pWeapon = NULL;
	return entite;
}

/*!
*  \fn     void Entity_Load(Entity *entite, Uint32 life, Uint32 armor, Uint32 iSpeed, EntityState state, Boolean bFriendly,Kr_Sprite *sprite)
*  \brief  Function to load the entity
*
*  \todo   use this function after the entity had been inited
*
*  \param entite a pointer to an entity
*  \param life his life with an integer
*  \param armor his armor with an integer
*  \param iSpeed the speed of the entity
*  \param state his state
*  \param bFriendly is the entity friendly ?
*  \param *sprite a pointer to the sprite of the entity
*  \return boolean it verify if the load is correct or not
*/
Boolean Entity_Load(Entity *entite, Uint32 life, Uint32 armor, Uint32 iSpeed, EntityState state, Boolean bFriendly, Kr_Sprite *sprite){
	entite->iEntityLife = life;
	entite->iArmor = armor;
	entite->pSprEntity = sprite;
	if (entite->pSprEntity == NULL){
		if (sprite) Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite %s of the entity %s !\n", sprite->strSpriteName, entite->strEntityName);
		else Kr_Log_Print(KR_LOG_ERROR, "Can't load an entity and a sprite, both NULL\n");
		return FALSE;
	}
	entite->state = state;
	entite->iSpeedEntity = iSpeed;
	entite->iTempoAtk = ATTACK_SPEED;
	entite->bFriendly = bFriendly;

	Entity_Log(entite);
	Kr_Log_Print(KR_LOG_INFO, "Entity %s with sprite %s has been loaded !\n", entite->strEntityName, entite->pSprEntity->strSpriteName);
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
		Weapon_Free(entite->pWeapon);
		UTIL_Free(entite);
	}
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
	if (entite->state == invisible) return TRUE;
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
	Kr_Log_Print(KR_LOG_INFO, "			state       : %d\n", pEntity->state);
	Kr_Log_Print(KR_LOG_INFO, "			bFriendly   : %d\n", pEntity->bFriendly);
}


/* ========================================================================= */
/*					GESTION DES LISTES D'ENTITES							 */
/* ========================================================================= */

NodeListEnt	* newNodeListEnt(Entity *e, NodeListEnt *n){
	NodeListEnt * new = (NodeListEnt *)malloc(sizeof(NodeListEnt));
	if (!new) return new = NULL;
	new->e = e;
	new->next = n;
	return new;
}

void deleteNodeListEnt(NodeListEnt * n){
	Entity_Free(n->e);
	UTIL_Free(n);
}


void initListEnt(ListEnt *lEnt){
	lEnt->first = lEnt->current = lEnt->last = NULL;
}

void deleteListEnt(ListEnt *lEnt){
	setOnFirstEnt(lEnt);
	while (lEnt->current != NULL){
		nextEnt(lEnt);
		deleteNodeListEnt(lEnt->first);
		Kr_Log_Print(KR_LOG_INFO, "The entity has been freed\n");
		lEnt->first = lEnt->current;
	}
}


Boolean emptyListEnt(ListEnt *lEnt){
	if (lEnt->first == NULL) return TRUE;
	else return FALSE;
}

int firstEnt(ListEnt *lEnt){
	if (lEnt->current->next == lEnt->first->next) return TRUE;
	else return FALSE;
}

int lastEnt(ListEnt *lEnt){
	if (lEnt->current->next == lEnt->last->next) return TRUE;
	else return FALSE;
}

int outOfListEnt(ListEnt *lEnt){
	if ((lEnt->current == NULL) && !emptyListEnt(lEnt)) return TRUE;
	else return FALSE;
}

void setOnFirstEnt(ListEnt *lEnt){
	lEnt->current = lEnt->first;
}

void setOnLastEnt(ListEnt *lEnt){
	lEnt->current = lEnt->last;
}

void nextEnt(ListEnt *lEnt){
	if (emptyListEnt(lEnt) == FALSE)
		lEnt->current = lEnt->current->next;
}


Entity * getCurrentEnt(ListEnt *lEnt){
	return lEnt->current->e;
}

void deleteCurrentEnt(ListEnt *lEnt, Boolean *nextL){
	NodeListEnt *nodeTmp = lEnt->current;

	if (lEnt->first == lEnt->last){
		lEnt->current = lEnt->first = lEnt->last = NULL;
		*nextL = TRUE;
	}
	else if (firstEnt(lEnt) == TRUE){
		nextEnt(lEnt);
		lEnt->first = lEnt->current;
		*nextL = TRUE;
	}
	else if (lastEnt(lEnt) == TRUE){
		setOnFirstEnt(lEnt);
		while (lEnt->current->next != nodeTmp)
			nextEnt(lEnt);
		lEnt->current->next = nodeTmp->next;
		lEnt->last = lEnt->current;
		*nextL = FALSE;
	}
	else{
		setOnFirstEnt(lEnt);
		while (lEnt->current->next != nodeTmp)
			nextEnt(lEnt);
		lEnt->current->next = nodeTmp->next;
		*nextL = FALSE;
	}
	deleteNodeListEnt(nodeTmp);
}


Boolean	insertLastEnt(ListEnt *lEnt, Entity *e){
	NodeListEnt * new = newNodeListEnt(e, NULL);
	if (emptyListEnt(lEnt) == TRUE){
		lEnt->first = new;
		lEnt->current = new;
	}
	else
		lEnt->last->next = new;
	lEnt->last = new;
	return TRUE;
}





/*!
*  \fn     void getVector(Kr_Input myEvent, Sint32 *vx, Sint32 *vy, Uint32 speed)
*  \brief  Function to get the movement vector of the player
*
*  \param  inEvent Structure which handle the input
*  \param  vx      a pointer to the vector on X
*  \param  vy      a pointer to the vector on Y
*  \param  speed   the speed of the entity
*  \return none
*/
void getVector(Kr_Input myEvent, Sint32 *vx, Sint32 *vy, Uint32 speed){
	*vx = *vy = 0;
	if (myEvent.szKey[SDL_SCANCODE_UP])
		*vy = 0 - speed;
	if (myEvent.szKey[SDL_SCANCODE_DOWN])
		*vy = speed;
	if (myEvent.szKey[SDL_SCANCODE_LEFT])
		*vx = 0 - speed;
	if (myEvent.szKey[SDL_SCANCODE_RIGHT])
		*vx = speed;
}

/*!
*  \fn     double getVectorToPlayer(Entity *pEntity, Entity *pPlayer, Sint32 *vx, Sint32 *vy)
*  \brief  Function to get the vector from an entity to the player
*  \param  pEntity pointer to the entity
*  \param  pPlayer pointer to the player
*  \param  vx      a pointer to the vector on X
*  \param  vy      a pointer to the vector on Y
*  \return movez   the diagonal to the player
*/
double getVectorToPlayer(Entity *pEntity, Entity *pPlayer, Sint32 *vx, Sint32 *vy){
	Sint32 movex = pPlayer->pSprEntity->pRectPosition->x - pEntity->pSprEntity->pRectPosition->x;
	Sint32 movey = pPlayer->pSprEntity->pRectPosition->y - pEntity->pSprEntity->pRectPosition->y;
	double movez = sqrt(movex*movex + movey*movey);
	double rapport = pEntity->iSpeedEntity / movez;
	*vx = (Sint32)(rapport * movex);
	*vy = (Sint32)(rapport * movey);
	//	Kr_Log_Print(KR_LOG_INFO, "Movement to player : %+d %+d\n", *vx, *vy);
	return movez;
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
*  \fn     void switchTextureFromDirection(Entity *entite, Direction newdir, SDL_Renderer *pRenderer){
*  \brief  Function to switch the texture of the entity according to the new direction
*
*  \param  enite  a pointer to the entity
*  \param  newdir the new direction
*  \param  pRenderer the renderer
*  \return none
*/
void switchTextureFromDirection(Entity *entite, Direction newDir, SDL_Renderer *pRenderer){
	// Nouveau sprite potentiel suivant la direction
	char newSprFileName[SIZE_MAX_NAME], direction[SIZE_MAX_NAME];
	SDL_Rect rect = { 0, 0, 0, 0 };
	FILE *fp = NULL;
	//	Kr_Log_Print(KR_LOG_INFO, "Previous direction : %d\n", entite->direction);

	strcpy(newSprFileName, entite->pSprEntity->strSpriteName); //Nécessaire de l'initialiser même si après la direction change
	switch (newDir){									// Suivant la nouvelle direction :
	case nord:
		strcpy(direction, "nord");
		entite->direction = nord;
		break;
	case sud:
		strcpy(direction, "sud");
		entite->direction = sud;
		break;
	case ouest:
		strcpy(direction, "ouest");
		entite->direction = ouest;
		break;
	case est:
		strcpy(direction, "est");
		entite->direction = est;
		break;
	}

	if (entite->firing == 1)
		sprintf(newSprFileName, "sprites/%s_%s_firing.png", entite->strEntityName, direction);
	else
		sprintf(newSprFileName, "sprites/%s_%s.png", entite->strEntityName, direction); //on va chercher le bon fichier image
	fp = fopen(newSprFileName, "r");
	if (!fp) return; // Vérifie l'existence du fichier
	fclose(fp);
	UTIL_FreeTexture(&entite->pSprEntity->pTextureSprite);							// on libère l'ancienne texture
	entite->pSprEntity->pTextureSprite = UTIL_LoadTexture(pRenderer, newSprFileName, NULL, &rect);	//on load la nouvelle texture asssociéee à la nouvelle direction
	entite->pSprEntity->iFrameWidth = rect.w;
	entite->pSprEntity->iFrameHeight = rect.h;
	entite->pSprEntity->pRectPosition->w = rect.w / entite->pSprEntity->iNbFrames;
	entite->pSprEntity->pRectPosition->h = rect.h;

	if (entite->pSprEntity->strSpriteName != NULL)
		UTIL_Free(entite->pSprEntity->strSpriteName); // On va devoir réalloué la taille du string
	Uint32 iNameLen = strlen(newSprFileName);
	entite->pSprEntity->strSpriteName = UTIL_CopyStr(newSprFileName, iNameLen);//on change le nom du sprite (par le lien sprites/image.png pour que ca soit plus clair

	//	Kr_Log_Print(KR_LOG_INFO, "Sprite %s of %d by %dhas been loaded\n", entite->pSprEntity->strSpriteName, entite->pSprEntity->iFrameWidth, entite->pSprEntity->iFrameHeight);
	//	Kr_Log_Print(KR_LOG_INFO, "New direction : %d\n", entite->direction);
}

/*!
*  \fn     void switchToFiringTexture(Entity *entite, SDL_Renderer *pRenderer, Uint32 nbFrames)
*  \brief  Function to switch from normal to firing texture
*
*  \param  enite     a pointer to the entity
*  \param  pRenderer the renderer
*  \param  nbFrames  the number of frames of the firing texture
*  \return none
*/
void switchToFiringTexture(Entity *entite, SDL_Renderer *pRenderer, Uint32 nbFrames){
	char newSprFileName[SIZE_MAX_NAME], direction[10];
	SDL_Rect rect = { 0, 0, 0, 0 };

	switch (entite->direction)
	{
	case nord:
		strcpy(direction, "nord");
		break;
	case est:
		strcpy(direction, "est");
		break;
	case ouest:
		strcpy(direction, "ouest");
		break;
	case sud:
		strcpy(direction, "sud");
		break;
	}

	sprintf(newSprFileName, "sprites/%s_%s_firing.png", entite->strEntityName, direction); //on va chercher le bon fichier image
	UTIL_FreeTexture(&entite->pSprEntity->pTextureSprite);							// on libère l'ancienne texture
	entite->pSprEntity->pTextureSprite = UTIL_LoadTexture(pRenderer, newSprFileName, NULL, &rect);	//on load la nouvelle texture asssociéee à la nouvelle direction
	entite->pSprEntity->iNbFrames = nbFrames;
	entite->pSprEntity->iFrameWidth = rect.w;
	entite->pSprEntity->iFrameHeight = rect.h;
	entite->pSprEntity->pRectPosition->w = rect.w / nbFrames;
	entite->pSprEntity->pRectPosition->h = rect.h;
	//	Kr_Log_Print(KR_LOG_INFO, "Sprite %s of %d by %d has been loaded !\n", entite->pSprEntity->strSpriteName, entite->pSprEntity->iFrameWidth, entite->pSprEntity->iFrameHeight);
}

/*!
*  \fn	  void firingAnimation(Entity *entite, SDL_Renderer *pRenderer)
*  \brief Function to handle the firing animation
*  
*  \param *entite    a pointer to the entity
*  \param *pRenderer a pointer to the renderer
*  \return none
*/
void firingAnimation(Entity *entite, SDL_Renderer *pRenderer){
	entite->iTempoFiringAnim += 1;
	if (entite->iTempoFiringAnim == RESET_FRAME)
	{
		entite->pSprEntity->iCurrentFrame += 1;
		if (entite->pSprEntity->iCurrentFrame % 3 == 0){
			entite->iTempoAnim += 1;
			if (entite->iTempoAnim == RESET_FIRING_FRAME){
				Projectile * newProj = Projectile_Init(entite->pWeapon->strNameProjectile);
				Projectile_Load(newProj, entite->pWeapon, entite->direction, entite->pWeapon->iSpeedPrj, entite->pWeapon->prjType, 3, entite->pSprEntity->pRectPosition, pRenderer);
				insertLast(entite->pWeapon->plProjectile, newProj);
				Kr_Log_Print(KR_LOG_INFO, "Le projectile %s a ete tire en direction : %d\n", entite->pWeapon->plProjectile->last->p->strNamePrj, entite->pWeapon->plProjectile->last->p->direction);
				entite->firing = 0;
				entite->pSprEntity->iNbFrames = 3;
				switchTextureFromDirection(entite, entite->direction, pRenderer);
				entite->iTempoAnim = 0;
			}
			entite->pSprEntity->iCurrentFrame = 0 + 3 * entite->iTempoAnim;
		}
		entite->iTempoFiringAnim = 0;
		Kr_Log_Print(KR_LOG_INFO, "Current frame : %d\n", entite->pSprEntity->iCurrentFrame);
	}

}

/*!
*  \fn	  void movementAnimation(Entity *entite)
*  \brief Function to handle the movement animation
*
*  \param *entite    a pointer to the entity
*  \return none
*/
void movementAnimation(Entity *entite){
	entite->mouvement = 1;
	entite->iTempoAnim += 1;
	if (entite->iTempoAnim == RESET_FRAME)	//Si la tempo est arrivée à son terme :
	{
		entite->pSprEntity->iCurrentFrame += 1; //	- Frame suivante
		if (entite->pSprEntity->iCurrentFrame == entite->pSprEntity->iNbFrames) //Si l'animation est arrivée au bout 
		{
			entite->pSprEntity->iCurrentFrame = 0;
		}
		entite->iTempoAnim = 0;
	}
}

/*!
*  \fn	  void resetAnimation(Entity *entite)
*  \brief Function to reset the animation of the entite
*
*  \param *entite    a pointer to the entity
*  \return none
*/
void resetAnimation(Entity *entite){
	entite->mouvement = 0;
	entite->pSprEntity->iCurrentFrame = 0;
	entite->iTempoAnim = 0;
	entite->iCurrentMoveX = 0;
	entite->iCurrentMoveY = 0;
}

/*  \fn void meleeDamage(Entity *pGiver, Entity *pReceiver)
*  \brief Function to inflict damage to an entity by being touched by another entity
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
	if (pReceiver->iEntityLife < 0) pReceiver->iEntityLife = 0;
}

/*  \fn void weaponDamage(Sint32 iDamage, Entity *pEntity)
*  \brief Function to inflict damage to an entity by being touched by a projectile
*
*  \param	iDamage the value of the damage
*  \param	pEntity	a pointer to the receiver of the damage
*  \return none
*/
void weaponDamage(Sint32 iDamage, Entity *pEntity){
	if (pEntity->iArmor >(Uint32)iDamage)
		pEntity->iArmor -= iDamage;
	else if (pEntity->iArmor > 0){
		Uint32 truedamage = iDamage - pEntity->iArmor;
		pEntity->iArmor = 0;
		pEntity->iEntityLife -= truedamage;
	}
	else
		pEntity->iEntityLife -= iDamage;
	if (pEntity->iEntityLife < 0) pEntity->iEntityLife = 0;
}

/*!
*  \fn     Boolean	shoot(Kr_Input myEvent, Entity *pEntity, SDL_Renderer *pRenderer)
*  \brief  if the player use zqsd to shoot, load a new projectile
*
*  \param	myEvent	the new event
*  \param	pEntity a pointer to the entity who wanted to fire
*  \param	pRender a pointer to the renderer
*  \return	TRUE if the entity has fired, FALSE otherwise
*/
Boolean	shoot(Kr_Input myEvent, Entity *pEntity, SDL_Renderer *pRenderer){
	Direction newDir = pEntity->direction; //Défaut
	Boolean res = FALSE;

	if (myEvent.szKey[SDL_SCANCODE_W]){
		if (pEntity->firing == 0){
			switchToFiringTexture(pEntity, pRenderer, 4);
			pEntity->firing = 1;
		}
		if (pEntity->pWeapon->iMunitionWeapon != 0 && pEntity->iTempoAtk == ATTACK_SPEED){
			Projectile * newProj = Projectile_Init(pEntity->pWeapon->strNameProjectile);
			Projectile_Load(newProj, pEntity->pWeapon, nord, pEntity->pWeapon->iSpeedPrj, pEntity->pWeapon->prjType, 1, pEntity->pSprEntity->pRectPosition, pRenderer);
			insertLast(pEntity->pWeapon->plProjectile, newProj);
			pEntity->pWeapon->iMunitionWeapon -= 1;
			pEntity->iTempoAtk = 0;
			pEntity->firing = 0;
			pEntity->pSprEntity->iNbFrames = 6;

			Kr_Log_Print(KR_LOG_INFO, "Le projectile %s a ete tire en direction : %d\n", pEntity->pWeapon->plProjectile->last->p->strNamePrj, pEntity->pWeapon->plProjectile->last->p->direction);
			res = TRUE;
		}
		else{
			if (pEntity->iTempoAtk < ATTACK_SPEED)
				pEntity->iTempoAtk += 1;
		}

		newDir = nord;
	}
	else if (myEvent.szKey[SDL_SCANCODE_A]){
		if (pEntity->firing == 0){
			switchToFiringTexture(pEntity, pRenderer, 4);
			pEntity->firing = 1;
		}
		if (pEntity->pWeapon->iMunitionWeapon != 0 && pEntity->iTempoAtk == ATTACK_SPEED){
			Projectile * newProj = Projectile_Init(pEntity->pWeapon->strNameProjectile);
			Projectile_Load(newProj, pEntity->pWeapon, ouest, pEntity->pWeapon->iSpeedPrj, pEntity->pWeapon->prjType, 1, pEntity->pSprEntity->pRectPosition, pRenderer);
			insertLast(pEntity->pWeapon->plProjectile, newProj);
			pEntity->pWeapon->iMunitionWeapon -= 1;
			pEntity->iTempoAtk = 0;
			pEntity->firing = 0;
			pEntity->pSprEntity->iNbFrames = 6;

			Kr_Log_Print(KR_LOG_INFO, "Le projectile %s a ete tire en direction : %d\n", pEntity->pWeapon->plProjectile->last->p->strNamePrj, pEntity->pWeapon->plProjectile->last->p->direction);
			res = TRUE;
		}
		else{
			if (pEntity->iTempoAtk < ATTACK_SPEED)
				pEntity->iTempoAtk += 1;
		}

		newDir = ouest;
	}
	else if (myEvent.szKey[SDL_SCANCODE_S]){
		if (pEntity->firing == 0){
			switchToFiringTexture(pEntity, pRenderer, 4);
			pEntity->firing = 1;
		}
		if (pEntity->pWeapon->iMunitionWeapon != 0 && pEntity->iTempoAtk == ATTACK_SPEED){
			Projectile * newProj = Projectile_Init(pEntity->pWeapon->strNameProjectile);
			Projectile_Load(newProj, pEntity->pWeapon, sud, pEntity->pWeapon->iSpeedPrj, pEntity->pWeapon->prjType, 1, pEntity->pSprEntity->pRectPosition, pRenderer);
			insertLast(pEntity->pWeapon->plProjectile, newProj);
			pEntity->pWeapon->iMunitionWeapon -= 1;
			pEntity->iTempoAtk = 0;
			pEntity->firing = 0;
			pEntity->pSprEntity->iNbFrames = 6;

			Kr_Log_Print(KR_LOG_INFO, "Le projectile %s a ete tire en direction : %d\n", pEntity->pWeapon->plProjectile->last->p->strNamePrj, pEntity->pWeapon->plProjectile->last->p->direction);
			res = TRUE;
		}
		else{
			if (pEntity->iTempoAtk < ATTACK_SPEED)
				pEntity->iTempoAtk += 1;
		}

		newDir = sud;
	}
	else if (myEvent.szKey[SDL_SCANCODE_D]){
		if (pEntity->firing == 0){
			switchToFiringTexture(pEntity, pRenderer, 4);
			pEntity->firing = 1;
		}
		if (pEntity->pWeapon->iMunitionWeapon != 0 && pEntity->iTempoAtk == ATTACK_SPEED){
			Projectile * newProj = Projectile_Init(pEntity->pWeapon->strNameProjectile);
			Projectile_Load(newProj, pEntity->pWeapon, est, pEntity->pWeapon->iSpeedPrj, pEntity->pWeapon->prjType, 1, pEntity->pSprEntity->pRectPosition, pRenderer);
			insertLast(pEntity->pWeapon->plProjectile, newProj);
			pEntity->pWeapon->iMunitionWeapon -= 1;
			pEntity->iTempoAtk = 0;
			pEntity->firing = 0;
			pEntity->pSprEntity->iNbFrames = 6;

			Kr_Log_Print(KR_LOG_INFO, "Le projectile %s a ete tire en direction : %d\n", pEntity->pWeapon->plProjectile->last->p->strNamePrj, pEntity->pWeapon->plProjectile->last->p->direction);
			res = TRUE;
		}
		else{
			if (pEntity->iTempoAtk < ATTACK_SPEED)
				pEntity->iTempoAtk += 1;
		}

		newDir = est;
	}
	else{
		if (pEntity->iTempoAtk < ATTACK_SPEED)
			pEntity->iTempoAtk += 1;
		pEntity->firing = 0;
		pEntity->pSprEntity->iNbFrames = 6;
	}

	switchTextureFromDirection(pEntity, newDir, pRenderer);
	return res;
}

/*  \fn Boolean	changeWeapon(Entity *pEntity, Weapon *pWeapon)
 *  \brief function to change the weapon of an entity
 *  
 *  \param	pEntity	a pointer the entity which you want to change the weapon
 *  \param	pWeapon	a pointer to the new weapon
 *  \return	TRUE 
 */
Boolean	changeWeapon(Entity *pEntity, Weapon *pWeapon){
	pEntity->pWeapon = pWeapon;
	Kr_Log_Print(KR_LOG_INFO, "The entity %s with weapon %s has been changed in %s\n", pEntity->strEntityName, pEntity->pWeapon->strNameWeapon, pEntity->pWeapon->strNameProjectile);
	return TRUE;
}


/*  \fn  Uint32 Entity_NumberHP(Entity *pEntity)
*  \brief function to return the number of hearth the entity has (1 hearth = 10hp)
*
*  \remarks : Max 20 hearth
*
*  \param	pEntity	a pointer the entity
*  \return	The number of hearth ( 1 hearth = 10 hp)
*/
Uint32 Entity_NumberHP(Entity *pEntity)
{
	Uint32 iHearth = 0;
    if (pEntity->iEntityLife % 10 == 0) // Dans le cas des multiples de 10
	{
		if (pEntity->iEntityLife == 0) iHearth = 0;
		else iHearth = pEntity->iEntityLife / 10 - 1;
	}
	else // Cas général
	{
		iHearth = pEntity->iEntityLife / 10;
	}
	if (iHearth > 20) iHearth = 20;
	return iHearth;
}
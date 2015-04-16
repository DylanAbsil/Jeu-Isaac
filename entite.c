/* ========================================================================= */
/*!
* \file    kr_entite.c
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
/* ========================================================================= */



#include "entite.h"
#include "kr_input.h"
#include "kr_level.h"

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
	//entite->wpnName = NULL; ??
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
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite %s in the entity %s !\n", sprite->strName, entite->strEntityName);
		return FALSE;
	}
	entite->state = normal;
	entite->iCoordXEntity = sprite->pRectPosition->x;
	entite->iCoordYEntity = sprite->pRectPosition->y;
	entite->direction = sud;
	entite->mouvement = 0;

	Kr_Log_Print(KR_LOG_INFO, "Entity %s with sprite %s has been loaded !\n", entite->strEntityName, entite->pSprEntity->strName);
	return TRUE;
}
//Rajouter le load d'une arme plus tard ou utiliser une fonction differente

/*!
*  \fn     void Entity_Free(Entity *entite)
*  \brief  Function to freed an entity
*
*  \param  pEntity a pointer to the Entity to free
*  \return none
*/
void Entity_Free(Entity *entite){
	UTIL_Free(entite->strEntityName);
	Kr_Sprite_Free(entite->pSprEntity);
	UTIL_Free(entite);
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
	Kr_Log_Print(KR_LOG_INFO, "The entity %s has been draw on the window on coordonates x = %d and y = %d\n", entite->strEntityName, entite->pSprEntity->pRectPosition->x, entite->pSprEntity->pRectPosition->y);
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
*	\fn	    Direction foundDirection(Sint32 vx, Sint32 vy, Entity *pEntity)
*  \brief  Function to get a direction from a vector
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
*  \fn     void updatePlayerVector(Kr_Input myEvent,Kr_Level *pLevel, Entity *pPlyer, SDL_Renderer *pRenderer)
*  \brief  Function to update the direction and the position on the map of the entite
*
*	\todo rajouter la fonction de gestion des collisions
*  \param  inEvent Structure which handle the input
*  \param  pLevel  a pointer to the Level
*  \param  pPlayer  a pointer to the player
*  \param pRenderer a pointer to the renderer
*  \return Boolean true if the vector has been updated false either
*/
Boolean updatePlayerVector(Kr_Input myEvent, Kr_Level *pLevel, Entity *pPlayer, SDL_Renderer *pRenderer){
	Sint32 vx, vy;

	//Obtention des déplacements générés par le clavier
	getVector(myEvent, &vx, &vy);
//	Kr_Log_Print(KR_LOG_INFO, "Move vector = { %d , %d }\n", vx, vy);


	// Changement de l'animation
	if ((vx == 0) && (vy == 0)){						//Si pas de mouvement :
		pPlayer->mouvement = 0;									//
		pPlayer->pSprEntity->iCurrentFrame = 0;					// reset de l'animation
		pPlayer->iTempoAnim = 0;											// reset de la tempo
//		Kr_Log_Print(KR_LOG_INFO, " the entity %s hasn't moved\n", entite->strEntityName);
		return TRUE;
	}
	else{												//Sinon
		pPlayer->mouvement = 1;
		pPlayer->iTempoAnim += 1;
//		Kr_Log_Print(KR_LOG_INFO, "tempoAnim = %d\n", *tempoAnim);

		if (pPlayer->iTempoAnim == RESET_FRAME){						//Si la tempo est arrivée à son terme :
			pPlayer->pSprEntity->iCurrentFrame += 1;				//	- Frame suivante
			if (pPlayer->pSprEntity->iCurrentFrame == pPlayer->pSprEntity->iNbFrames)   //Si l'animation est arrivée au bout 
				pPlayer->pSprEntity->iCurrentFrame = 0;								  //	-> on revient au début
//			Kr_Log_Print(KR_LOG_INFO, "Frame counter = %d\n", entite->pSprEntity->iCurrentFrame);


			pPlayer->iTempoAnim = 0;

//			Kr_Log_Print(KR_LOG_INFO, "The animation has changed to the next frame\n");
		}
		
		switchTextureFromDirection(pPlayer, vx, vy, pRenderer);

		//Gestion des collisions (à venir)
		if (Kr_Collision_Move(pLevel, pPlayer->pSprEntity->pRectPosition, vx, vy) == 3)
		{
			vx = vy = 0;
		}
		//Deplacement final prévu
		pPlayer->iCoordXEntity = pPlayer->pSprEntity->pRectPosition->x; // Modification des coordonnées de l'entité, celles du sprite sont modifiées par les fonctions de collision
		pPlayer->iCoordYEntity = pPlayer->pSprEntity->pRectPosition->y;
		
//		Kr_Log_Print(KR_LOG_INFO, "The entity %s has moved of %d in x and of %d in y\nNew Position : %d ; %d\n", entite->strEntityName, vx, vy, entite->iCoordXEntity, entite->iCoordYEntity);
		return TRUE;
	}
}



/*!
*  \fn     void updateEntityVector(Kr_Level *pLevel, Entity *entite, SDL_Renderer *pRenderer)
*  \brief  Function to update the direction and the position on the map of the entite
*
*  \param  pLevel  a pointer to the Level
*  \param  pEntity  a pointer to the entity
*  \param  pRenderer a pointer to the renderer
*  \return Boolean true if the vector has been updated false either
*/
Boolean updateEntityVector(Kr_Level *pLevel, Entity *pEntity, SDL_Renderer *pRenderer){
	return TRUE;
}

/*!
*  \fn     void switchTextureFromDirection(Entity *entite, Sint32 vx, Sint32 vy, SDL_Renderer *pRenderer){
*  \brief  Function to switch the texture of the entity according to the new direction
*
*  \param  enite  a pointer to the entity
*  \param  vx the vector in x
*  \param  vy the vector in y
*  \param  pRenderer the renderer
*  \return none
*/
void switchTextureFromDirection(Entity *entite, Sint32 vx, Sint32 vy, SDL_Renderer *pRenderer){
	// Nouveau sprite potentiel suivant la direction
	char newSprFileName[SIZE_MAX_NAME];
	Direction newDir = foundDirection(vx, vy, entite);			//  - on cherche la nouvelle direction
//	Kr_Log_Print(KR_LOG_INFO, "Previous direction : %d\n", entite->direction);

	strcpy(newSprFileName, entite->pSprEntity->strName); //Nécessaire de l'initialiser même si après la direction change
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
	//if (entite->pSprEntity->strName != NULL) free(entite->pSprEntity->strName); // On va devoir réalloué la taille du string

	strcpy(entite->pSprEntity->strName, newSprFileName);//on change le nom du sprite (par le lien sprites/image.png pour que ca soit plus clair
//	Kr_Log_Print(KR_LOG_INFO, "Sprite %s has been loaded\n", entite->pSprEntity->strName);
//	Kr_Log_Print(KR_LOG_INFO, "New direction : %d\n", entite->direction);

}
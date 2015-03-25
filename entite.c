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
/*               |            |                                              */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#include "entite.h"
#include "kr_input.h"
#include "kr_level.h"

/*!
*  \fn     Entity * init_Entity()
*  \brief  Function to init an entity
*
*  \todo   use this function at first to create an entity then load
*
*  \param  none
*  \return Entity* a pointer to the empty created entity 
*/
Entity * init_Entity(){
	Entity * entite = (Entity *)malloc(sizeof(Entity));
	entite->strEntityName = "Nom indefini";
	entite->pSprEntity = NULL;
	entite->state = normal;
	entite->iEntityLife = 0;
	entite->iSpeedEntity = 0;
	//entite->wpnName = NULL; ??
	return entite;
}

/*!
*  \fn     void load_Entity(Entity *entite, char * name, Uint32 life, Uint32 armor, Kr_Sprite *sprite)
*  \brief  Function to load the entity
*
*  \todo   use this function after the entity had been inited
*
*  \param entite a pointer to an entity
*  \param name the name of the entity
*  \param life his life with an integer
*  \param armor his armor with an integer
*  \param sprite a pointer to the sprite of the entity
*  \return boolean verify if the load is correct or no
*/
Boolean load_Entity(Entity *entite, char * name, Uint32 life, Uint32 armor, Kr_Sprite *sprite){
	entite->strEntityName = name;
	entite->iEntityLife = life;
	entite->iArmor = armor;
	entite->pSprEntity = sprite;
	if (sprite == NULL){
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite %s in the entity %s !\n", sprite->strName, name);
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
*  \fn     draw_Entite(SDL_Renderer * renderer, Entity entite)
*  \brief  Function to draw an entity
*
*  \todo   Update this function when the entity have moved
*
*  \param  pRenderer
*  \param  entite
*  \return boolean if the entite has been draw on the screen or not
*/
Boolean draw_Entity(SDL_Renderer * pRenderer, Entity *entite){
	SDL_Rect frameToDraw;
	int largeur = entite->pSprEntity->iFrameWidth / entite->pSprEntity->iNbFrames;
	if ((largeur < 0) | (entite == NULL)){
		Kr_Log_Print(KR_LOG_ERROR, "Impossible to access to the entity\n");
		return FALSE;
	}
	frameToDraw.x = (entite->pSprEntity->iCurrentFrame) * largeur;
	frameToDraw.y = 0;
	frameToDraw.h = entite->pSprEntity->iFrameHeight;
	frameToDraw.w = largeur;
	Kr_Log_Print(KR_LOG_INFO, "Frame : { x = %d ; y = %d ; h = %d ; w = %d }\n", frameToDraw.x, frameToDraw.y, frameToDraw.h, frameToDraw.w);
	if (SDL_RenderCopy(pRenderer, entite->pSprEntity->pTextureSprite, &frameToDraw, entite->pSprEntity->pRectPosition) == -1){
		Kr_Log_Print(KR_LOG_ERROR, "The entity %s hasn't been draw on the window\n", entite->strEntityName);
		return FALSE;
	}
	Kr_Log_Print(KR_LOG_INFO, "The entity %s has been draw on the window\n", entite->strEntityName);
	return TRUE;

}



/*!
*  \fn     void getVector(Kr_Input inEvent, Sint32 *vx, Sint32 *vy)
*  \brief  Function to get the vector of the player
*
*  \todo   Update this function when the Player structure will be define
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
 *	\fn	    Direction foundDirection(Sint32 vx, Sint32 vy)
 *  \brief  Function to get a direction from a vector
 *
 *  \param vx a move in x
 *  \param vy a move in y
 *  \return the direction associated to the vector
 */
Direction foundDirection(Sint32 vx, Sint32 vy){
	Direction newDir;
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
*  \fn     void updatePlayerVector(Kr_Input myEvent,Kr_Level *pLevel, Entity *entie, int *tempoAnim)
*  \brief  Function to get the vector of the player
*
*  \todo   Update this function when the Player structure will be define
*
*  \param  inEvent Structure which handle the input
*  \param  pLevel  a pointer to the Level
*  \param  pPlayer  a pointer to the player
*  \param tempoAnim a int to make a teporosation of the animation
*  \return Boolean true if the vector has been updated false either
*/
Boolean updatePlayerVector(Kr_Input myEvent, Kr_Level *pLevel, Entity *entite, int *tempoAnim){
	Sint32 vx, vy;

	// Nouveau sprite potentiel suivant la direction
	char newSprFileName[SIZE_MAX_NAME];

	//Obtention des déplacements générés par le clavier
	getVector(myEvent, &vx, &vy);
	Kr_Log_Print(KR_LOG_INFO, "Move vector = { %d , %d }\n", vx, vy);

	//Gestion des collisions
	//Kr_Collision_Move(pLevel, pPlayer, vx, vy);

	// Changement de l'animation
	if ( (vx == 0) && (vy ==0) ){
		entite->mouvement = 0;
		entite->pSprEntity->iCurrentFrame = 0;
		*tempoAnim = 0;
		Kr_Log_Print(KR_LOG_INFO, " the entity %s hasn't moved\n", entite->strEntityName);
		return TRUE;
	}
	else{
		entite->mouvement = 1;
		*tempoAnim += 1;
		Kr_Log_Print(KR_LOG_INFO, "tempoAnim = %d\n", *tempoAnim);
		if (*tempoAnim == RESET_FRAME){
			entite->pSprEntity->iCurrentFrame += 1;
			if (entite->pSprEntity->iCurrentFrame == 8)
				entite->pSprEntity->iCurrentFrame = 0;
			Kr_Log_Print(KR_LOG_INFO, "Frame counter = %d\n", entite->pSprEntity->iCurrentFrame);
			Direction newDir = foundDirection(vx, vy);
			Kr_Log_Print(KR_LOG_INFO, "Previous direction : %d\n", entite->direction);
			switch (newDir){
			case nord:
				if (entite->direction != nord){
					entite->direction = nord;
					sprintf(newSprFileName, "sprites/%s_%s.png", entite->strEntityName, "nord");
					UTIL_FreeTexture(&entite->pSprEntity->pTextureSprite);
					entite->pSprEntity->pTextureSprite = UTIL_LoadTexture(newSprFileName, NULL, entite->pSprEntity->pRectPosition);
				}
				break;
			case sud:
				if (entite->direction != sud){
					entite->direction = sud;
					sprintf(newSprFileName, "sprites/%s_%s.png", entite->strEntityName, "sud");
					UTIL_FreeTexture(&entite->pSprEntity->pTextureSprite);
					entite->pSprEntity->pTextureSprite = UTIL_LoadTexture(newSprFileName, NULL, entite->pSprEntity->pRectPosition);
				}
				break;
			case ouest:
				if (entite->direction != ouest){
					entite->direction = ouest;
					sprintf(newSprFileName, "sprites/%s_%s.png", entite->strEntityName, "ouest");
					UTIL_FreeTexture(&entite->pSprEntity->pTextureSprite);
					entite->pSprEntity->pTextureSprite = UTIL_LoadTexture(newSprFileName, NULL, entite->pSprEntity->pRectPosition);
				}
				break;
			case est:
				if (entite->direction != est){
					entite->direction = est;
					sprintf(newSprFileName, "sprites/%s_%s.png", entite->strEntityName, "est");
					UTIL_FreeTexture(&entite->pSprEntity->pTextureSprite);
					entite->pSprEntity->pTextureSprite = UTIL_LoadTexture(newSprFileName, NULL, entite->pSprEntity->pRectPosition);
				}
				break;
			default:
				break;
			}
			Kr_Log_Print(KR_LOG_INFO, "New direction : %d\n", entite->direction);
			*tempoAnim = 0;
			Kr_Log_Print(KR_LOG_INFO, "The animation has changed to the next frame\n");
		}
		//Deplacement final prévu
		entite->iCoordXEntity += vx;
		entite->iCoordYEntity += vy;
		Kr_Log_Print(KR_LOG_INFO, "The entity %s has moved of %d in x and of %d in y\n", entite->strEntityName, vx, vy);
		return TRUE;
	}


	
	
}
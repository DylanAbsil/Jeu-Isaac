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
	entite->strEntityName = NULL;
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
*  \param life his life with integer
*  \param armor his armor with an integer
*  \param sprite a pointer to the sprite of the entity
*  \return none
*/
void load_Entity(Entity *entite, char * name, Uint32 life, Uint32 armor, Kr_Sprite *sprite){
	entite->strEntityName = name;
	entite->iEntityLife = life;
	entite->iArmor = armor;
	entite->pSprEntity = sprite;
	entite->state = normal;
	entite->iCoordXEntity = sprite->pRectPosition->x;
	entite->iCoordYEntity = sprite->pRectPosition->y;
	entite->direction = sud;
	entite->mouvement = 0;
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
*  \return none
*/
void draw_Entite(SDL_Renderer * pRenderer, Entity entite){
	SDL_Rect frameToDraw;
	int largeur = entite.pSprEntity->iFrameWidth / entite.pSprEntity->iNbFrames;
	frameToDraw.x = entite.pSprEntity->iCurrentFrame * largeur;
	frameToDraw.y = 0;
	frameToDraw.h = entite.pSprEntity->iFrameHeight;
	frameToDraw.w = largeur;
	SDL_RenderCopy(pRenderer, entite.pSprEntity->pTextureSprite, &frameToDraw, entite.pSprEntity->pRectPosition);
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
		newDir = nord;
	if (vy < 0)
		newDir = sud;
	if (vx > 0)
		newDir = est;
	if (vx < 0)
		newDir = ouest;
	return newDir;
}

/*!
*  \fn     void updatePlayerVector(Kr_Input inEvent,Kr_Level *pLevel, SDL_Rect *pPlayer)
*  \brief  Function to get the vector of the player
*
*  \todo   Update this function when the Player structure will be define
*
*  \param  inEvent Structure which handle the input
*  \param  pLevel  a pointer to the Level
*  \param  pPlayer  a pointer to the player
*  \return none
*/
void updatePlayerVector(Kr_Input myEvent, Kr_Level *pLevel, Entity *entite){
	Sint32 vx, vy;
	char *newSprFileName = entite->strEntityName;		/* Nouveau sprite potentiel suivant la direction */


	//Obtention des déplacements générés par le clavier
	getVector(myEvent, &vx, &vy);

	// Changement de l'animation meme si l'entite ne bouge pas
	if ((vx & vy) == 0){
		entite->mouvement = 0;
		entite->pSprEntity->iCurrentFrame = 0;
	}
	else{
		entite->mouvement = 1;
		entite->pSprEntity->iCurrentFrame++;
		if (entite->pSprEntity->iCurrentFrame == 8)
			entite->pSprEntity->iCurrentFrame = 0;
		Direction newDir = foundDirection(vx, vy);
		switch(newDir){
		case nord:
			if (entite->direction != nord){
				entite->direction = nord;
				strcat(newSprFileName, "_nord");
				UTIL_FreeTexture(&entite->pSprEntity->pTextureSprite);
				entite->pSprEntity->pTextureSprite = UTIL_LoadTexture(UTIL_BuildPath("sprites", newSprFileName, strlen(newSprFileName) - 1, "png"), NULL, entite->pSprEntity->pRectPosition);
			}
		case sud:
			if (entite->direction != sud){
				entite->direction = sud;
				strcat(newSprFileName, "_sud");
				UTIL_FreeTexture(&entite->pSprEntity->pTextureSprite);
				entite->pSprEntity->pTextureSprite = UTIL_LoadTexture(UTIL_BuildPath("sprites", newSprFileName, strlen(newSprFileName) - 1, "png"), NULL, entite->pSprEntity->pRectPosition);
			}
		case ouest:
			if (entite->direction != ouest){
				entite->direction = ouest;
				strcat(newSprFileName, "_ouest");
				UTIL_FreeTexture(&entite->pSprEntity->pTextureSprite);
				entite->pSprEntity->pTextureSprite = UTIL_LoadTexture(UTIL_BuildPath("sprites", newSprFileName, strlen(newSprFileName) - 1, "png"), NULL, entite->pSprEntity->pRectPosition);
			}
		case est:
			if (entite->direction != est){
				entite->direction = est;
				strcat(newSprFileName, "_est");
				UTIL_FreeTexture(&entite->pSprEntity->pTextureSprite);
				entite->pSprEntity->pTextureSprite = UTIL_LoadTexture(UTIL_BuildPath("sprites", newSprFileName, strlen(newSprFileName) - 1, "png"), NULL, entite->pSprEntity->pRectPosition);
			}
		}
	}

	//Gestion des collisions
	//Kr_Collision_Move(pLevel, pPlayer, vx, vy);

	//Deplacement final prévu
	entite->iCoordXEntity += vx;
	entite->iCoordYEntity += vy;
	
	
}
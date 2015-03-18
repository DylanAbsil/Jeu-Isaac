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

#include "kr_entite.h"

Kr_Sprite * createSprite(char * name, SDL_Texture * texture, Uint32 frameWidth, Uint32 nbFrames, SDL_Rect * ptRectPosition){
	Kr_Sprite * sprite = (Kr_Sprite *)malloc(sizeof(Kr_Sprite));
	sprite->sprName = name;
	sprite->ptextureSprite = texture;
	sprite->lFrameWidth = frameWidth;
	sprite->lNbFrames = nbFrames;
	sprite->lCurrentFrames = 0;
	sprite->rectPosition = ptRectPosition;
	return sprite;
}


Entity * createEntity(char * name, Uint32 life, Uint32 armor, Kr_Sprite sprite){
	Entity * entite = (Entity *)malloc(sizeof(Entity));
	entite->szEntityName = name;
	entite->iArmor = armor;
	entite->iEntityLife = life;
	entite->sprEntity = sprite;
	entite->state = normal;
	entite->iCoordXEntity = sprite.rectPosition->x;
	entite->iCoordYEntity = sprite.rectPosition->y;
	return entite;
}

void printZelda(SDL_Renderer * renderer, Entity entity){
	Uint32 mouvement = entity.sprEntity.lCurrentFrames / 8;
	Uint32 avancee = entity.sprEntity.lCurrentFrames % 8;
	SDL_Rect ptiZelda;
	ptiZelda.y = mouvement * 26;
	ptiZelda.x = avancee * 30;
	ptiZelda.h = 26;
	ptiZelda.w = 30;
	SDL_RenderCopy(renderer, entity.sprEntity.ptextureSprite, &ptiZelda, entity.sprEntity.rectPosition);
}
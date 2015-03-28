/* ========================================================================= */
/*!
* \file    kr_entite.h
* \brief   Contains the structure to manage the entities.
* \author  Alexandre Robin
* \version 1.0
* \date    14 Mars 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Robin         | 14/03/2015 | Création.                                    */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#ifndef __ENTITE_H__
#define __ENTITE_H__

#include "kr_sprite.h"
#include "kr_input.h"
#include "kr_level.h"
#include "kr_log.h"


/*!
* \enum EntityState
* \brief Enumeration to describe the state of the entity.
*/
typedef enum {
	normal,
	invincible,
	slowed,
	feared,
	poisoned,
}Entity_state;

/*!
* \enum EntityType
* \brief Enumeration to describe the type of the entity : player, boss, monster ...
*/
typedef enum {
	player,
	monster,
	small_monster,
	boss,
	pnj,
}Entity_type;

/*!
 * \enum Direction
 * \brief Enumaration to describe the direction of the mouvement of the entity
 */
typedef enum {
	nord,
	est,
	sud,
	ouest,
}Entity_direction;


typedef struct {
	char	*strNamePrj;
	Sint32	*iDamagePrj;
	Uint32	*iSpeedPrj;
	Kr_Sprite *pSprProjectile;
	Sint32	iCoordPrj_XStart;
	Sint32	iCoordPrj_YStart;
	Sint32	iCoordPrj_XCurrent;
	Sint32	iCoordPrj_YCurrent;
}Projectile;


typedef struct {
	char	*strNameWeapon;
	Uint32	iRangeWeapon;
	Sint32	iMunitionWeapon;
	Sint32	iDamageWeapon;
}Weapon;


typedef struct {
	char	*strEntityName;		/* Name of the entity */
	Entity_type type;			/* Type of the entity */
	Uint32	iEntityLife;		/* Life of the entity */
	Uint32	iArmor;				/* Armor of the entity */
	Weapon	wpnName;
	Kr_Sprite	*pSprEntity;	/* Pointer to his sprite */
	Entity_state	state;
	Sint32	iCoordXEntity;
	Sint32	iCoordYEntity;
	Uint32	iSpeedEntity;
	Entity_direction direction;		/* Direction : nord, est, sud, ouest */
	Boolean mouvement;			/* Mouvement : 0 static, 1 in movement */
}Entity;

Entity * Entity_init();
Boolean Entity_load(Entity *entite, char * name, Entity_type type, Uint32 life, Uint32 armor, Kr_Sprite *sprite); /*!< creationd'une entite >*/
void Entity_free(Entity *entite);
Boolean Entity_draw(SDL_Renderer *pRenderer, Entity *entite);


Entity_direction foundDirection(Sint32 vx, Sint32 vy);
void switchTextureFromDirection(Entity *entite, Sint32 vx, Sint32 vy);

void getVector(Kr_Input myEvent, Sint32 *vx, Sint32 *vy);

Boolean updateEntityVector(Kr_Input myEvent, Kr_Level *pMyLevel, Entity *entite, int *tempoAnim);

#endif /* __KR_ENTITE_H__ */
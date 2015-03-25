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


#include "kr_sprite.h"
#include "kr_input.h"
#include "kr_level.h"
#include "kr_log.h"


/*!
* \enum EntityState
* \brief Enumeration to describe the stae of the entity.
*/
typedef enum {
	normal,
	invincible,
	slowed,
	feared,
	poisoned,
}EntityState;

/*!
 * \enum Direction
 * \brief Enumaration to describe the direction of the mouvement of the entity
 */
typedef enum {
	nord,
	est,
	sud,
	ouest,
}Direction;


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
	Uint32	iEntityLife;		/* Life of the entity */
	Uint32	iArmor;				/* Armor of the entity */
	Weapon	wpnName;
	Kr_Sprite	*pSprEntity;
	EntityState	state;
	Sint32	iCoordXEntity;
	Sint32	iCoordYEntity;
	Uint32	iSpeedEntity;
	Direction direction;
	Boolean mouvement;			/* Mouvement : 0 static, 1 in movement */
}Entity;

Entity * init_Entity();
Boolean load_Entity(Entity *entite, char * name, Uint32 life, Uint32 armor, Kr_Sprite *sprite); /*!< creationd'une entite >*/
void free_Entity(Entity *entite);
Boolean draw_Entity(SDL_Renderer *pRenderer, Entity *entite);


Direction foundDirection(Sint32 vx, Sint32 vy);

void getVector(Kr_Input myEvent, Sint32 *vx, Sint32 *vy);
Boolean updatePlayerVector(Kr_Input myEvent, Kr_Level *pMyLevel, Entity *entite, int *tempoAnim);

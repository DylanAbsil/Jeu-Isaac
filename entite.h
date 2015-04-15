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
/* Herrou        | 28/03/2015 | Harmoniser les prototypes avec le projet     */
/*               |            | Mise en forme et corrections                 */
/*               |            | Ajout du SDL_Renderer pour updateEntityVector*/
/*               |            | Prévention des inclusions multiples          */
/* Herrou        | 04/04/2015 | Initialisation du nom faite par UTIL_CopyStr */
/*               |            | Le nom du sprite est donnée à Sprite_Init    */
/*               |            |    et non pas à Entity_Load                  */
/* Herrou        | 05/04/2015 | Ajout du param Entity à foundDirection       */
/* ========================================================================= */

#ifndef __ENTITE_H__
#define __ENTITE_H__

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
	char	*strEntityName;		/*! Name of the entity */
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
	Uint32	iTempoAnim			/*! int for the temporisation of the animation */
}Entity;

Entity *	Entity_Init(char *szFileName);
Boolean		Entity_Load(Entity *entite, Uint32 life, Uint32 armor, Kr_Sprite *sprite); /*!< creationd'une entite >*/
void		Entity_Free(Entity *entite);
Boolean		Entity_Draw(SDL_Renderer *pRenderer, Entity *entite);


Direction	foundDirection(Sint32 vx, Sint32 vy, Entity *pEntity);
void		getVector(Kr_Input myEvent, Sint32 *vx, Sint32 *vy);
void		switchTextureFromDirection(Entity *entite, Sint32 vx, Sint32 vy, SDL_Renderer *pRenderer);

Boolean		updatePlayerVector(Kr_Input myEvent, Kr_Level *pMyLevel, Entity *pPlayer, SDL_Renderer *pRenderer);
Boolean		updateAllEntites(Kr_Level *pLevel, SDL_Renderer *pRenderer);
Boolean		updateEntityVector(Kr_Level *pLevel, Entity *pEntity, SDL_Renderer *pRenderer);

#endif /* __KR_ENTITE_H__ */
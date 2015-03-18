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

#include "kr_util.h"



/*===========================================================================*/
/*	  				       GESTION DES SPRITES								 */
/*===========================================================================*/

typedef struct {
	char		*sprName;			/*!< Name of the sprite */
	SDL_Texture *ptextureSprite;	/*!< Texture of the sprite */
	Uint32		lFrameWidth;		/*!< Frame width of the sprite */  // Un sprite n'a qu'une seule largeur
	Uint32		lNbFrames;			/*!< Number of frames in the sprite */
	Uint32		lCurrentFrames;		/*!< The current frame */
	SDL_Rect	*rectPosition;
}Kr_Sprite;

Kr_Sprite * createSprite(char * name, SDL_Texture * texture, Uint32 frameWidth, Uint32 nbFrames, SDL_Rect * ptRectPosition);




/*=========================================================================== */
/*					   	   GESTION DES ENTITES								  */
/*============================================================================*/

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

typedef struct {
	char	*szNamePrj;
	Sint32	*iDamagePrj;
	Uint32	*iSpeedPrj;
	Kr_Sprite sprProjectile;
	Sint32	iCoordPrj_XStart;
	Sint32	iCoordPrj_YStart;
	Sint32	iCoordPrj_XCurrent;
	Sint32	iCoordPrj_YCurrent;
}Projectile;

typedef struct {
	char	*szNameWeapon;
	Uint32	iRangeWeapon;
	Sint32	iMunitionWeapon;
	Sint32	iDamageWeapon;
	Kr_Sprite	sprWeapon;
}Weapon;

typedef struct {
	char	*szEntityName;		/*!< Name of the entity */
	Uint32	iEntityLife;		/*!< Life of the entity */
	Uint32	iArmor;				/*!< Armor of the entity */
	Weapon	wpnName;
	Kr_Sprite	sprEntity;
	EntityState	state;
	Sint32	iCoordXEntity;
	Sint32	iCoordYEntity;
	Uint32	iSpeedEntity;
}Entity;


Entity * createEntity(char * name, Uint32 life, Uint32 armor, Kr_Sprite sprite); /*!< creationd'une entite >*/
void printZelda(SDL_Renderer * renderer, Entity entity);
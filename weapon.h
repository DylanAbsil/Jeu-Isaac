/* ========================================================================= */
/*!
* \file    weapon.c
* \brief   Contains the structure to handle the weapons and the projectiles.
* \author  Alexandre Robin
* \version 1.0
* \date    20 Avril 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* ========================================================================= */

#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "kr_common.h"
#include "kr_sprite.h"
#include "kr_collision.h"

#define ATTACK_SPEED 20			/*< Number to handle how many projectile do you fire at the same time >*/
#define RESET_PROJECTILE_FRAME 10

/*!
* \enum PrjState
* \brief Enumeration to describe the state of the projectile.
*/
typedef enum {
	hard,
	ghost,
	piercing,
	poisonning,
	burning,
}PrjType;

typedef struct {
	char		*strNamePrj;			/*< Name of the projectile >*/
	Sint32	    iDamagePrj;				/*< Damage of the projectile >*/
	Uint32	    iSpeedPrj;				/*< Speed of the projectile >*/
	Direction	direction;				/*< Direction of the projectile's movement >*/
	PrjType		prjType;				/*< The type of projectile used >*/
	Kr_Sprite	*pSprProjectile;		/*< A pointer to the sprite of the projectile >*/
	Uint32		iTempoAnim;
	Sint32		iCoordPrj_XEnd;			/*< Limit position of the projectile in x>*/
	Sint32		iCoordPrj_YEnd;			/*< Limit position of the projectile in y*/
}Projectile;

/*! \struct nodeListProjectile
* \brief Data element of a list, containing a projectile
*/
typedef struct{
	Projectile * p;				/*!< A pointer to a projectile */
	struct NodeListProj * next;	/*!< A pointer to the next projectile in the list*/
}NodeListProj;


/*! \struct list
* \brief list of projectile nodes
*/
typedef struct{
	NodeListProj *first;		/*!< A pointer to the first list element*/
	NodeListProj *current;		/*!< A pointer to the current list element*/
	NodeListProj *last;			/*!< A pointer to the last list element*/
}ListProj;

typedef struct {
	char		*strNameWeapon;			/*< Name of the weapon >*/
	char		*strNameProjectile;		/*< Name of the projective contained in the weapon >*/
	Uint32		iRangeWeapon;			/*< Range of the weapon >*/
	Sint32		iMunitionWeapon;		/*< Munitions currently in the weapon >*/
	Sint32		iDamageWeapon;			/*< Damage of the weapon <*/
	Uint32	    iSpeedPrj;				/*< Speed of the projectile >*/
	Uint32		iattackSpeed;
	PrjType		prjType;				/*< The type of projectile used >*/
	ListProj	*plProjectile;			/*< Liste containing all the projectile currently still fired (under range of the weapon and before collisions >*/
}Weapon;


/* ======================================== */
/*				PROJECTILES					*/
/* ======================================== */

Projectile* Projectile_Init(char *strProjName);
Boolean		Projectile_Load(Projectile *pProj, Weapon *pWeapon, Direction dir, Uint32 speed, PrjType prjType, Uint32 nbFrames, SDL_Rect *pRect, SDL_Renderer *pRenderer); 
void		Projectile_Free(Projectile *pProj);
Boolean		Projectile_Draw(SDL_Renderer *pRenderer, Projectile *pProj);



/* ======================================== */
/*			LIST OF PROJECTILES				*/
/* ======================================== */

NodeListProj *	newNodeListProj(Projectile *p, NodeListProj *n);
void			deleteNodeListProj(NodeListProj * n);

void			initList(ListProj *lProj);
void			deleteList(ListProj *lProj);

Boolean			emptyList(ListProj *lProj);
Boolean			first(ListProj *lProj);
Boolean			last(ListProj *lProj);
Boolean			outOfList(ListProj *lProj);

void			setOnFirst(ListProj *lProj);
void			setOnLast(ListProj *lProj);
void			next(ListProj *lProj);
Projectile *	getCurrentProj(ListProj *lProj);

void			deleteCurrent(ListProj *lProj, Boolean *nextL);

Boolean			insertLast(ListProj *lProj, Projectile *p);

Boolean			drawProjectilesWeapon(ListProj *lProj, SDL_Renderer *pRenderer);


/* ======================================== */
/*				  WEAPONS				    */
/* ======================================== */

Weapon *		Weapon_Init(char *strWeaponName);
Boolean			Weapon_Load(Weapon *pWeapon, char *strProjName, Uint32 range, Uint32 munition, Uint32 damage, Uint32 speedPrj, PrjType prjType);
void			Weapon_Free(Weapon *pWeapon);


#endif
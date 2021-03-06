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
/* Robin         | 14/03/2015 | Cr?ation.                                    */
/* Herrou        | 28/03/2015 | Harmoniser les prototypes avec le projet     */
/*               |            | Mise en forme et corrections                 */
/*               |            | Ajout du SDL_Renderer pour updateEntityVector*/
/*               |            | Pr?vention des inclusions multiples          */
/* Herrou        | 04/04/2015 | Initialisation du nom faite par UTIL_CopyStr */
/*               |            | Le nom du sprite est donn?e ? Sprite_Init    */
/*               |            |    et non pas ? Entity_Load                  */
/* Herrou        | 05/04/2015 | Ajout du param Entity ? foundDirection       */
/* Herrou        | 22/04/2015 | Ajout de l'include kr_collision			     */
/* ========================================================================= */

#ifndef __ENTITE_H__
#define __ENTITE_H__



#include "weapon.h"
#include "kr_input.h"

#define MOVESPEED 3					/*< Spped of the player >*/
#define RESET_FRAME	 10				/*< Number to handle the reset of the frame >*/
#define RESET_FIRING_FRAME 3		/*< Number to handle the reset of the frame of firing animation >*/
#define MOB_INFIGHTING_DAMAGE 2		/*< The damage of the monsters when we touch them >*/

/*!
* \enum EntityState
* \brief Enumeration to describe the state of the entity.
*/
typedef enum {
	normal,
	invincible,
	slowed,
	invisible,
	poisoned,
	noclip,
	touched,
}EntityState;

/*!
* \struct Entity
* \brief  Structure to handle the entity.
*/
typedef struct Entity{
	char		*strEntityName;		/*< Name of the entity >*/
	Sint32		iEntityLife;		/*< Life of the entity >*/
	Uint32		iArmor;				/*< Armor of the entity >*/
	Weapon		*pWeapon;			/*< A pointer to his weapon (can be NULL) >*/
	Kr_Sprite	*pSprEntity;		/*<	A pointer to his sprite >*/
	EntityState	state;				/*< State of the entity : normal, invincible, slowed, ...) >*/
	Uint32		iSpeedEntity;		/*< Speed of the entity */
	Direction	direction;			/*< Direction which the entity is facing >*/
	Boolean		mouvement;			/*< Mouvement : 0 (static) or 1 (in movement) >*/
	Boolean		firing;				/*< Firing : 1 or 0 >*/
	Uint32		iTempoAnim;			/*< Int for the temporisation of the animation >*/
	Uint32		iTempoFiringAnim;   /*< Int for the temporisation of the animation of the attacks >*/
	Uint32		iTempoAtk;			/*< Int to handle the attack speed >*/
	Boolean     bFriendly;			/*< TRUE if the entity is friendly ? Does it deal damage  >*/
	Uint32      iTempoMovement;     /*< Value which can be used to compute a random movement   >*/
	Sint32      iCurrentMoveX;		/*< Value which can be used to compute a random movement   >*/
	Sint32      iCurrentMoveY;		/*< Value which can be used to compute a random movement   >*/
}Entity;


/*! 
* \struct nodeListeEntity
* \brief Data element of a list, containing an entity
*/
typedef struct{
	Entity * e;					/*!< A pointer to an entity */
	struct NodeListEnt * next;	/*!< A pointer to the next entity in the list*/
}NodeListEnt;

/*! 
* \struct list
* \brief list of entity nodes
*/
typedef struct{
	NodeListEnt *first;		/*!< A pointer to the first list element*/
	NodeListEnt *current;		/*!< A pointer to the current list element*/
	NodeListEnt *last;			/*!< A pointer to the last list element*/
}ListEnt;


/* ======================================== */
/*				ENTITIES					*/
/* ======================================== */

Entity *	Entity_Init(char *szFileName);
Boolean		Entity_Load(Entity *entite, Uint32 life, Uint32 armor, Uint32 iSpeed, EntityState state, Boolean bFriendly, Kr_Sprite *sprite); /*< cr?ation d'une entite >*/
void		Entity_Free(Entity *entite);
Boolean		Entity_Draw(SDL_Renderer *pRenderer, Entity *entite);
void        Entity_Log(Entity *pEntity);
Uint32      Entity_NumberHP(Entity *pEntity);

/* ======================================== */
/*			LIST OF ENTITIES				*/
/* ======================================== */

NodeListEnt *	newNodeListEnt(Entity *e, NodeListEnt *n);
void			deleteNodeListEnt(NodeListEnt *n);

void			initListEnt(ListEnt *lEnt);
void			deleteListEnt(ListEnt *lEnt);

Boolean			emptyListEnt(ListEnt *lEnt);
Boolean			firstEnt(ListEnt *lEnt);
Boolean			lastEnt(ListEnt *lEnt);
Boolean			outOfListEnt(ListEnt *lEnt);

void			setOnFirstEnt(ListEnt *lEnt);
void			setOnLastEnt(ListEnt *lEnt);
void			nextEnt(ListEnt *lEnt);
Entity *		getCurrentEnt(ListEnt *lEnt);

void			deleteCurrentEnt(ListEnt *lEnt, Boolean *nextL);

Boolean			insertLastEnt(ListEnt *lEnt, Entity *e);


/* ======================================== */
/*			MATHS FOR FIGHTS				*/
/* ======================================== */

Direction	foundDirection(Sint32 vx, Sint32 vy, Entity *pEntity);
void		foundWayToPlayer(Entity *pEntity, Entity *pPlayer, Sint32 movex, Sint32 movey);
void		getVector(Kr_Input myEvent, Sint32 *vx, Sint32 *vy, Uint32 speed);
double		getVectorToPlayer(Entity *pEntity, Entity *pPlayer, Sint32 *vx, Sint32 *vy);
void		switchTextureFromDirection(Entity *entite, Direction newdir, SDL_Renderer *pRenderer);
void		switchToFiringTexture(Entity *entite, SDL_Renderer *pRenderer, Uint32 nbFrames);


/* ======================================== */
/*				FIGHTS						*/
/* ======================================== */

void		firingAnimation(Entity *entite, SDL_Renderer *pRenderer);
void		movementAnimation(Entity *entite);
void		resetAnimation(Entity *entite);

void		meleeDamage(Entity *pGiver, Entity *pReceiver);
void		weaponDamage(Sint32 iDamage, Entity *pEntity);
Boolean		shoot(Kr_Input myEvent, Entity *pEntity, SDL_Renderer *pRenderer);
Boolean		changeWeapon(Entity *pEntity, Weapon *pWeapon);

#endif /* __KR_ENTITE_H__ */
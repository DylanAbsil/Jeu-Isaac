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
/* Herrou        | 22/04/2015 | Ajout de l'include kr_collision			     */
/* ========================================================================= */

#ifndef __ENTITE_H__
#define __ENTITE_H__



#include "weapon.h"
#include "kr_input.h"
#include "kr_level.h"
#include "kr_collision.h"

#define MOVESPEED 5		/*< Movespeed of the player >*/
#define MOB_MOVESPEED 3 /*< Basic movespeed of the monstres >*/
#define RESET_FRAME 10	/*< Number to handle the reset of the frame >*/

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
}EntityState;

typedef struct {
	char		*strEntityName;		/*< Name of the entity >*/
	Uint32		iEntityLife;		/*< Life of the entity >*/
	Uint32		iArmor;				/*< Armor of the entity >*/
	Weapon		*pWeapon;			/*< A pointer to his weapon (can be NULL) >*/
	Kr_Sprite	*pSprEntity;		/*<	A pointer to his sprite >*/
	EntityState	state;				/*< State of the entity : normal, invincible, slowed, ...) >*/
	Sint32		iCoordXEntity;		
	Sint32		iCoordYEntity;
	Uint32		iSpeedEntity;		/*< */
	Direction	direction;			/*< Direction which the entity is facing >*/
	Boolean		mouvement;			/*< Mouvement : 0 (static) or 1 (in movement) >*/
	Uint32		iTempoAnim;			/*< Int for the temporisation of the animation >*/
	Uint32		iTempoAtk;			/*< Int to handle the attack speed >*/
}Entity;

Entity *	Entity_Init(char *szFileName);
Boolean		Entity_Load(Entity *entite, Uint32 life, Uint32 armor, Kr_Sprite *sprite); /*< création d'une entite >*/
void		Entity_Free(Entity *entite);
Boolean		Entity_Draw(SDL_Renderer *pRenderer, Entity *entite);


Direction	foundDirection(Sint32 vx, Sint32 vy, Entity *pEntity);
void		getVector(Kr_Input myEvent, Sint32 *vx, Sint32 *vy);
void		getVectorToPlayer(Entity *pEntity, Entity *pPlayer, Sint32 *vx, Sint32 *vy);
void		switchTextureFromDirection(Entity *entite, Direction newdir, SDL_Renderer *pRenderer);

Boolean		updatePlayerVector(Kr_Input myEvent, Kr_Level *pMyLevel, Entity *pPlayer, SDL_Renderer *pRenderer);
Boolean		updateEntityVector(Kr_Level *pLevel, Entity *pEntity, Entity *pPlayer, SDL_Renderer *pRenderer);

Boolean		Shoot(Kr_Input myEvent, Entity *pEntity, SDL_Renderer *pRenderer);
Boolean		ChangeWeapon(Entity *pEntity, Weapon *pWeapon);

#endif /* __KR_ENTITE_H__ */
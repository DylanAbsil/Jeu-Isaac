/* ========================================================================= */
/*!
* \file    animaux.c
* \brief   Contains the function to handle the animal IA
* \author  Olivier Herrou
* \version 1.0
* \date    29 Avril 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 29/04/2015 | Creation.                                    */
/*               |            |                                              */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#include "animaux.h"

/*
Remarque : Le déplacement de l'oiseau n'est pas interrompu même lors d'un changement de niveau





*/
/*!
*  \fn     CalculPassageOiseau(Entity *pEntity, Boolean bLaunch, Sint32 *movex, Sint32 *movey)
*  \brief  Function to compute the movement of the entity
*
*  \param  pEntity Pointeur vers l'entité
*  \param  bLaunch lancer un calcul de vecteurs
*  \param  movex   movement vector on X
*  \param  movey   movement vector on X
*  \return none
*/
void CalculPassageOiseau(Entity *pEntity, Boolean bLaunch, Sint32 *movex, Sint32 *movey)
{
	Uint32 iDir = 0, iMaxSpeed = 1, iTmp = 0, xMin = 0, xMax = 0, yMin = 0, yMax = 0;

	if (!bLaunch) return;
	*movex = *movey = 0;
	//Choix de la direction vers laquelle l'oiseau se dirigera
	iDir = rand() % 4;
	// Préparation de la position initial de l'oiseau
	switch (iDir)
	{
	case 0: // nord
		xMin = 0;
		xMax = KR_WIDTH_WINDOW - pEntity->pSprEntity->pRectPosition->w;
		yMin = yMax = KR_HEIGHT_WINDOW - pEntity->pSprEntity->pRectPosition->h;
		break;
	case 1: // est
		xMin = xMax = KR_WIDTH_WINDOW - pEntity->pSprEntity->pRectPosition->w;
		yMin = 0;
		yMax = KR_HEIGHT_WINDOW - pEntity->pSprEntity->pRectPosition->h;
	case 2: // sud
		xMin = 0;
		xMax = KR_WIDTH_WINDOW - pEntity->pSprEntity->pRectPosition->w;
		yMin = yMax = 0;
		break;
	case 3: // ouest
		xMin = xMax = 0;
		yMin = 0;
		yMax = KR_HEIGHT_WINDOW - pEntity->pSprEntity->pRectPosition->h;
		break;
		break;
	default: return;
	}
	// Choix d'une position aléatoire du début
	Uint32 x = 0, y = 0;
	x = (rand() % (xMax - xMin + 1)) + xMin;
	y = (rand() % (yMax - yMin + 1)) + yMin;
	pEntity->pSprEntity->pRectPosition->x = x;
	pEntity->pSprEntity->pRectPosition->y = y;

	//Calcul des vecteurs 
	*movex = rand() % (iMaxSpeed + 1);
	*movey = rand() % (iMaxSpeed + 1);
	// Si le random génère un vecteur nul sur x et y alors on doit quand même le faire se déplacer de manière cohérente
	if (iDir == 0 && *movey == 0) *movey = 1;
	if (iDir == 1 && *movex == 0) *movex = 1;
	if (iDir == 2 && *movey == 0) *movey = 1;
	if (iDir == 3 && *movex == 0) *movex = 1;
	if (*movex == 0 && *movey == 0) *movex = 1;


	//Calcul du nombre de répétition du déplacement avant que l'entité soit sortie de l'écran
	if (iDir == 0) *movey = *movey * -1; // Il doit descendre s'il commence au nord
	if (iDir == 1) *movex = *movex * -1; // s'il commence à l'est
	Kr_Log_Print(KR_LOG_INFO, "Début du passage avec %d %d\n", *movex, *movey);

}

/*!
*  \fn     Entity *ChargementOiseau(SDL_Renderer *pRenderer)
*  \brief  Function to load the entities
*
*  \param  pRenderer a pointer to the renderer
*  \return none
*/
Entity *ChargementOiseau(SDL_Renderer *pRenderer)
{
	Entity *pEntity = NULL;
	Kr_Sprite *pSprite = NULL;
	SDL_Rect *pRectMouette = NULL;
	pRectMouette = (SDL_Rect*)malloc(sizeof(SDL_Rect));
	pRectMouette->x = 0;
	pRectMouette->y = 0;
	pRectMouette->w = 27;
	pRectMouette->h = 27;

	/* Chargement des sprites */
	pSprite = Kr_Sprite_Init("mouette");
	if (Kr_Sprite_Load(pSprite, sud, 96, 27, 3, pRectMouette, pRenderer) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite mouette !\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}


	/* Chargement des personnages */
	pEntity = Entity_Init("mouette");
	if (Entity_Load(pEntity, 100, 0, MOVESPEED, noclip, TRUE, pSprite) == FALSE)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the entity !\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	return pEntity;
}



/*!
*  \fn     Boolean PassageOiseau(Entity *pEntity, Boolean bPassage, Sint32 movex, Sint32 movey, SDL_Renderer *pRenderer)
*  \brief  Function to compute the movement of the entity
*
*  \param  pEntity  Pointeur vers l'entité
*  \param  bPassage lancer un passage d'oiseau
*  \param  movex    movement vector on X
*  \param  movey    movement vector on X
*  \param  pRenderer a pointer to the renderer
*  \return TRUE if the entity is still on the screen, FALSE otherwise
*/
Boolean PassageOiseau(Entity *pEntity, Boolean bPassage, Sint32 movex, Sint32 movey, SDL_Renderer *pRenderer)
{
	static Boolean bPassageEnCours = FALSE;
	Direction newDir = nord;
	if (bPassage || bPassageEnCours) bPassageEnCours = TRUE;
	else return FALSE;

	newDir = foundDirection(movex, movey, pEntity);
	switchTextureFromDirection(pEntity, newDir, pRenderer);
	pEntity->mouvement = 1;
	pEntity->iTempoAnim += 1;
	if (pEntity->iTempoAnim == RESET_FRAME)	//Si la tempo est arrivée à son terme :
	{
		pEntity->pSprEntity->iCurrentFrame += 1; //	- Frame suivante
		if (pEntity->pSprEntity->iCurrentFrame == pEntity->pSprEntity->iNbFrames) //Si l'animation est arrivée au bout 
		{
			pEntity->pSprEntity->iCurrentFrame = 0;
		}
		pEntity->iTempoAnim = 0;
	}

	pEntity->pSprEntity->pRectPosition->x += movex;
	pEntity->pSprEntity->pRectPosition->y += movey;


	if (pEntity->pSprEntity->pRectPosition->x < 0 || pEntity->pSprEntity->pRectPosition->x > KR_WIDTH_WINDOW ||
		pEntity->pSprEntity->pRectPosition->y < 0 || pEntity->pSprEntity->pRectPosition->y > KR_HEIGHT_WINDOW)
	{
		Kr_Log_Print(KR_LOG_INFO, "Fin du passage\n");
		bPassageEnCours = FALSE;
		return FALSE;
	}
	return TRUE;
}

/*!
*  \fn     void FreeOiseau(pEntity)
*  \brief  Function free the entities

*  \param pEntity a pointer to the first entity to free
*  \return none
*/
void FreeOiseau(Entity *pEntity)
{
	Entity_Free(pEntity);
}
/* ========================================================================= */
/*!
* \file    kr_input.c
* \brief   Contains the structure to handle the input (keyboard and mouse).
* \author  Olivier Herrou
* \version 1.0
* \date    5 Février 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 05/02/2015 | Creation.                                    */
/*               | 08/02/2015 | SDL_KEYDOWN gère maintenant les répétitions  */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#include "kr_input.h"


/* ========================================================================= */

/*!
*  \fn     void UpdateEvents( Kr_Input* inEvent )
*  \brief  Function to update the Input structure
*
*  \param  inEvent      Structure which handle the input
*  \return None
*/
void UpdateEvents(Kr_Input* inEvent)
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_KEYDOWN: 
			if (event.key.repeat == 0) // Permet de gérer le repeat
			{
				inEvent->szKey[event.key.keysym.scancode] = 1;
			}			
			break;
		case SDL_KEYUP:
			inEvent->szKey[event.key.keysym.scancode] = 0;
			break;
		case SDL_MOUSEMOTION:
			inEvent->iMouseX = event.motion.x;
			inEvent->iMouseY = event.motion.y;
			inEvent->iMouseXRel = event.motion.xrel;
			inEvent->iMouseYRel = event.motion.yrel;
			break;
		case SDL_MOUSEBUTTONDOWN:
			inEvent->szMouseButtons[event.button.button - 1] = 1;
			break;
		case SDL_MOUSEBUTTONUP:
			inEvent->szMouseButtons[event.button.button - 1] = 0;
			break;
		case SDL_QUIT:
			inEvent->bQuit = 1;
			break;
		default:
			break;
		}
	}
}

/*!
*  \fn     void InitEvents( Kr_Input* inEvent )
*  \brief  Function to initialize the Input structure
*
*  \param  inEvent      Structure which handle the input
*  \return None
*/
void   InitEvents(Kr_Input* inEvent)
{
	memset(inEvent, 0, sizeof(*inEvent)); // Mise à 0 de toutes les données de la structure Kr_Input
}


/* Utilisation de cette bibliothèque */
/* Remarque, ne pas compiler ce code, il faut l'adapter pour effectuer un code exploitable/analysable */
/*void main(void)
{
	Kr_Input inEvent; // Structure pour la gestion des événements
	InitEvents(&inEvent);

	while (!inEvent.szKey[SDL_SCANCODE_ESCAPE] && !inEvent.bQuit) // On vérifie si l'utilisateur quitte le programme
	{
		UpdateEvents(&inEvent);
		if (inEvent.szMouseButtons[0]) // Clique Gauche
		{
			// Mon action // J'effectue ce que je veux si l'utilisateur  à appuyer sur son clique gauche
			inEvent.szMouseButtons[0] = 0; // Un seul clique, si je ne met pas ça, le son sera joué en boucle. La l'utilisateur va devoir relever son doig
		}
		if (inEvent.szMouseButtons[1]) // Clique molette
		{
			// Mon action
			// J'ai pas précisé ici un seul clique, donc on rentrera ici en permanence tant que le bouton ne sera pas relaché
		}
		if (inEvent.szMouseButtons[2]) // Clique droit
		{
			// Mon action
			inEvent.szMouseButtons[2] = 0; // Un seul clique
		}
		if (inEvent.szKey[SDL_SCANCODE_UP]) // Touche flèche du haut
		{
			// Mon action
		}
		if (inEvent.szKey[SDL_SCANCODE_P])
		{
			// Mon action
			inEvent.szKey[SDL_SCANCODE_P] = 0; // Un seul clique, comme pour la souris
		}
}*/
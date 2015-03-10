/* ========================================================================= */
/*!
* \file    kr_quit.c
* \brief   Contains the functions to quit the application
* \author  Olivier Herrou
* \version 1.0
* \date    21 Février 2015
*/ 
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 21/02/2015 | Creation.                                    */
/*               |            | TODO : Gérer les libérations de mémoire en   */
/*               |            |		   fonction de quel fonction l'appele    */
/*               |            |		   à kr_quit est fait                    */
/* ========================================================================= */

#include "kr_quit.h"

void Kr_Quit(void)
{
	Kr_Log_Print(KR_LOG_INFO, "Kr_Quit\n");
	Mix_CloseAudio(); // On quitte SDL_MIXER
	TTF_Quit(); // On quitte SDL_TTF
	SDL_Quit(); // On quitte SDL
	Kr_Log_Quit(); // On ferme les logs
	exit(EXIT_FAILURE);
}
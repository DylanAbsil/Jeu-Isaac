/* ========================================================================= */
/*!
* \file    kr_config.c
* \brief   Contains the functions to configure and initialize the graphic motor
* \author  Olivier Herrou
* \version 1.0
* \date    01 Mars 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 01/03/2015 | Creation.                                    */
/* ========================================================================= */



#include "kr_config.h"



/*!
*  \fn     Boolean Kr_Init(Uint32 iWidthWindow, Uint32 iHeightWindow)
*  \brief  Function to initialize the SDL

*  \return TRUE if everything is ok, FALSE otherwise
*/
Boolean Kr_Init(void)
{
	if (SDL_Init(SDL_INIT_VIDEO) == -1)
	{
		Kr_Log_Print(KR_LOG_ERROR, "SDL failed to initialize (%s)\n", SDL_GetError());
		return FALSE;
	}

	/* Initialisation de SDL_mixer*/
	if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 1024) == -1)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't initialize SDL_Mixer : %s", Mix_GetError());
		SDL_Quit();
		return FALSE;
	}

	/* Initialisation de SDL_ttf*/
	if (TTF_Init() == -1)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't initialize SDL_ttf : %s", TTF_GetError());
		SDL_Quit();
		Mix_CloseAudio();
		return FALSE;
	}
	Kr_Log_Print(KR_LOG_INFO, "All SDL modules has been loaded !\n");
	return TRUE;
}

/*!
*  \fn     void Kr_Quit(void)
*  \brief  Function to quit the exe in case of an error

*  \return EXIT_FAILURE
*/
void Kr_Quit(void)
{
	Kr_Log_Print(KR_LOG_WARNING, "EXIT FAILURE\n");
	Mix_CloseAudio();		// SDL_MIXER
	TTF_Quit();				// SDL_TTF
	SDL_Quit();				// SDL
	Kr_Log_Quit();			// Logs
	exit(EXIT_FAILURE);
}


/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#pragma comment( lib, "SDL2.lib" )
#pragma comment( lib, "SDL2main.lib" )
#pragma comment( lib, "SDL2_mixer.lib" )
#pragma comment( lib, "SDL2_ttf.lib" )
#pragma comment( lib, "SDL2_image.lib" )

#include "kr_common.h"
#include "kr_log.h"
#include "kr_util.h" 
#include "kr_input.h"
#include "kr_config.h"

// Banque de son : http://www.wavsource.com/
// Utiliser Uint32 gérer la portabilité !!!
#define KR_FPS 30// Nombre de FPS
#define KR_WIDTH_WINDOW  1280
#define KR_HEIGHT_WINDOW  704

SDL_Renderer *gpRenderer = NULL;

int main(int argc, char** argv)
{

	Kr_Log_Init(KR_LOG_INFO); // Mise en place d'un fichier de log

	/* ========================================================================= */
	/*                          INITIALISATION DE LA SDL                         */
	/* ========================================================================= */
	SDL_Window *pWindow = NULL;
	if (!Kr_Init())
	{
		exit(EXIT_FAILURE);
	}
	
	/* ========================================================================= */
	/*                           CONFIGURATION GENERALE                          */
	/* ========================================================================= */


	Kr_Input inEvent; // Structure pour la gestion des événements
	Uint32   iPreviousTime = 0, iCurrentTime = 0; // Variable pour la gestion des FPS

	/* Création de la fenêtre */
	pWindow = SDL_CreateWindow("Jeu 2D - Isaac", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, KR_WIDTH_WINDOW, KR_HEIGHT_WINDOW, SDL_WINDOW_SHOWN); // SDL_WINDOW_FULLSCREEN
	if (pWindow == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't create the Window : %s\n", SDL_GetError());
		SDL_Quit();
		return FALSE;
	}
	SDL_Surface *pscreenSurface = SDL_GetWindowSurface(pWindow);
	if (pscreenSurface == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't create the Surface Window : %s\n", SDL_GetError());
		SDL_Quit();
		return FALSE;
	}
	/* Création du renderer */
	gpRenderer = SDL_CreateRenderer(pWindow, -1, SDL_RENDERER_ACCELERATED);
	if (gpRenderer == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't create the Renderer", SDL_GetError());
		SDL_Quit();
		return FALSE;
	}
	/* Initialisation de la structure pour gérer les événements*/
	InitEvents(&inEvent);

	/* ========================================================================= */
	/*                                 EVENEMENT                                 */
	/* ========================================================================= */

	while (!inEvent.szKey[SDL_SCANCODE_ESCAPE] && !inEvent.bQuit)
	{

		UpdateEvents(&inEvent);


		if (inEvent.szMouseButtons[0])
		{
			Kr_Log_Print(KR_LOG_ERROR, "CLIQUE GAUCHE : %d %d \n", inEvent.iMouseX, inEvent.iMouseY);
			inEvent.szMouseButtons[0] = 0; // Un seul clique, si je ne met pas ça, le son sera joué en boucle. La l'utilisateur va devoir relever son doigt
		}
		if (inEvent.szMouseButtons[1])
		{
			Kr_Log_Print(KR_LOG_INFO, "CLIQUE MOLETTE\n");
			inEvent.szMouseButtons[1] = 0; // Un seul clique
		}
		if (inEvent.szMouseButtons[2])
		{
			Kr_Log_Print(KR_LOG_INFO, "CLIQUE DROIT\n");
			inEvent.szMouseButtons[2] = 0; // Un seul clique
		}
		

		/* ========================================================================= */
		/*                                    FPS                                    */
		/* ========================================================================= */

		iCurrentTime = SDL_GetTicks();
		if (iCurrentTime - iPreviousTime > (1000 / KR_FPS))
		{
			iPreviousTime = iCurrentTime;
		}
		else
		{
			//Kr_Log_Print(KR_LOG_INFO, "Attente de %d \n", (1000 / KR_FPS) - (iCurrentTime - iPreviousTime));
			SDL_Delay((1000 / KR_FPS) - (iCurrentTime - iPreviousTime));
		}

		/* ========================================================================= */
		/*                                  DIVERS                                   */
		/* ========================================================================= */
		

		/* ========================================================================= */
		/*                                  RENDER                                   */
		/* ========================================================================= */
		// Ici on gère l'affichage des surfaces
		SDL_RenderClear(gpRenderer); // Dans un premier temps on Clear le renderer
		SDL_RenderPresent(gpRenderer); // Lorsque toutes les surfaces ont été placé on affiche le renderer (l'écran quoi...)
	}

	/* ========================================================================= */
	/*                            LIBERATION MEMOIRE                             */
	/* ========================================================================= */


	SDL_DestroyRenderer(gpRenderer);	// Libération mémoire du renderer
	SDL_DestroyWindow(pWindow);			// Libération mémoire de la fenetre

	Mix_CloseAudio();	// On quitte SDL_MIXER
	TTF_Quit();			// On quitte SDL_TTF
	SDL_Quit();			// On quitte SDL
	Kr_Log_Quit();		// On ferme les logs

	return EXIT_SUCCESS;
}
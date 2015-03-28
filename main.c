/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 18/02/2015 | TTF encapsuler dans kr_text                  */
/*               |            | Renderer en variable global ? Si oui où ?    */
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
#include "kr_text.h"
#include "kr_level.h"
#include "kr_config.h"
#include "entite.h"

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
	/*                                  PRECACHE                                 */
	/* ========================================================================= */

	/* Préparation d'images que l'on souhaitera afficher */
	Kr_Sprite	 *pSpriteZelda = NULL;
	Kr_Sprite	 *pSpriteMonstre = NULL;
	Entity		 *pZelda = NULL;
	Entity		 *pMonstre = NULL;
	SDL_Rect     rectPositionZelda;
	SDL_Rect     rectPositionMonstre;
	Uint32		 tempoAnim = 0;
	Uint32		 tempoAnim2 = 0;

	rectPositionZelda.x = 0; 
	rectPositionZelda.y = 0;
	rectPositionZelda.w = 32; //Il est nécessaire de fournir la taille de l'image avec .w et .h sinon rien n'apparaitra
	rectPositionZelda.h = 32;

	rectPositionMonstre.x = 0;
	rectPositionMonstre.y = 0;
	rectPositionMonstre.h = 120;
	rectPositionMonstre.w = 120;

	/* Chargement des sprites */
	pSpriteZelda = Sprite_init();			//D'abord création et load du sprite (ici le nom du sprite est "sprites/zelda_sud.png"
	pSpriteMonstre = Sprite_init();

	if ( Sprite_load(pSpriteZelda, "zelda", 26, 240, 8, &rectPositionZelda) == FALSE ){		
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite !\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}
	if (Sprite_load(pSpriteMonstre, "dragon", 64, 420, 8, &rectPositionMonstre) == FALSE){
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite !\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	/* Chargement des personnages */
	pZelda = Entity_init();				//Ensuite création et load du sprite (il faut préciser la taille de l'image png)
	pMonstre = Entity_init();
	
	if( Entity_load(pZelda, "zelda", player, 100, 50, pSpriteZelda) == FALSE ){		
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite !\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	if (Entity_load(pMonstre, "dragon", boss, 200, 10, pSpriteMonstre) == FALSE){
		Kr_Log_Print(KR_LOG_ERROR, "Cant load the sprite !\n");
		SDL_Quit();
		exit(EXIT_FAILURE);
	}

	

	/* Chargement du niveau */
	SDL_Texture *pLimitation = NULL;
	pLimitation = UTIL_LoadTexture("Visu.png", NULL, NULL);
	Kr_Level *pMonLevel = NULL;
	SDL_Rect rLevel; // où sera afficher le level sur le renderer
	SDL_Rect rLimitation; 

	rLevel.x = 0;
	rLevel.y = 0;
	rLevel.h = KR_HEIGHT_WINDOW;
	rLevel.w = KR_WIDTH_WINDOW;

	rLimitation.x = 0;
	rLimitation.y = 0;
	rLimitation.w = 64;
	rLimitation.h = 64;

	pMonLevel = Kr_Level_Init("level2"); // Ne pas préciser l'extension
	if (!Kr_Level_Load(pMonLevel, gpRenderer))
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't Load a level\n");
		exit(EXIT_FAILURE);
	}
	Kr_Level_Focus(pMonLevel, &rectPositionZelda, &rLimitation);

	/* ========================================================================= */
	/*                                 EVENEMENT                                 */
	/* ========================================================================= */
	// Cette boucle va permettre mettre à jour la structure qui gère les entrées clavier/souris
	// On aura plus qu'à regarder si notre touche a été appuyé et si oui agir en conséquence
	while (!inEvent.szKey[SDL_SCANCODE_ESCAPE] && !inEvent.bQuit)
	{
		
		UpdateEvents(&inEvent);					//chargement du nouvel evenements dans la variable inevent
		
		/*Gestion des evenement de la souris */
		if (inEvent.szMouseButtons[0])
		{
			Kr_Log_Print(KR_LOG_INFO, "CLIQUE GAUCHE\n");
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
		
		/*Gestion des evenements clavier*/
		if (updateEntityVector(inEvent, pMonLevel, pMonstre, &tempoAnim) == FALSE){				//Update la position et l'animation du perso principal
			Kr_Log_Print(KR_LOG_ERROR, "Couldn't update player vector\n");
			SDL_Quit();
			return FALSE;
		}
		Kr_Log_Print(KR_LOG_INFO, "Player vector has been updated\n");

		
		if (inEvent.szKey[SDL_SCANCODE_P])
		{
			inEvent.szKey[SDL_SCANCODE_P] = 0; // Un seul clique, comme pour la souris
			if (Mix_PausedMusic()) // Si le son est en pause
			{
				Mix_ResumeMusic(); // On le relance
			}
			else
			{
				Mix_PauseMusic();//Sinon je le met en pause
			}
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

		/* Autres évenements à gérer */


		/* ========================================================================= */
		/*                                  RENDER                                   */
		/* ========================================================================= */

		// Ici on gère l'affichage des surfaces
		SDL_RenderClear(gpRenderer); // Dans un premier temps on Clear le renderer
		
		// Remarque, en inversant les deux SDL_RenderCopy, on peut choisir qu'elle image sera en arrière-plan de l'autre
		
		Kr_Level_Draw(gpRenderer, pMonLevel, &rLevel);				//Affichage du level	
		//draw_Entity(gpRenderer, pZelda);							//Affichage du perso principale
		Entity_draw(gpRenderer, pMonstre);

		SDL_RenderPresent(gpRenderer); // Lorsque toutes les surfaces ont été placé on affiche le renderer (l'écran quoi...)
	}
	//Fin de la boucle d'affichage puis fin du programme

	/* ========================================================================= */
	/*                            LIBERATION MEMOIRE                             */
	/* ========================================================================= */


	UTIL_FreeTexture(&pLimitation);		// Libération mémoire de la texture 
	SDL_DestroyRenderer(gpRenderer);	// Libération mémoire du renderer
	SDL_DestroyWindow(pWindow);			// Libération mémoire de la fenetre
	Kr_Level_Free(pMonLevel);			// Libération mémoire du niveau
	Entity_free(pZelda);				// Libération mémoire du zelda
	Entity_free(pMonstre);
	Mix_CloseAudio();	// On quitte SDL_MIXER
	TTF_Quit();			// On quitte SDL_TTF
	SDL_Quit();			// On quitte SDL
	Kr_Log_Quit();		// On ferme les logs
	return EXIT_SUCCESS;
}
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
#include "HUD.h"
#include "kr_text.h"
#include "Menu.h"


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
	/*                           CONFIGURATION HUD                               */
	/* ========================================================================= */

	/* Initialisation des variables de la police pour les HUD */
	TTF_Font *policeHUD = NULL; //Variable pour la police des HUD
	SDL_Color CouleurHUD = { 255, 255, 255 }; //Variable pour la couleur de la police des HUD

	/* Ouverture de la police */
	policeHUD = Kr_Text_OpenFont("upheavtt", 26);
	TTF_SetFontStyle(policeHUD, TTF_STYLE_BOLD);
	
	/* Initialisation des structures HUD */
	HUD *Vie = NULL; //HUD vie
	HUD * BombeImage = NULL; //HUD Munitions (l'image seulement)
	HUD * BombeTexte = NULL; //HUD Munitions (le nombre de munition)
	HUD * CleImage = NULL; //HUD Bombe (l'image seulement)
	HUD * CleTexte = NULL; //HUD Bombe (le nombre de bombes)

	/* Initialisation des positions des HUD */
	// Pour mettre texte et images à la même hauteru (pour les HUD Bombe et Cle) :
	// écart en x : les parties textes doivent être décalées de la largeur de l'image + l'espacement HUD_ESPACEMENT
	// écart en y : normalement 0 mais comme la police est plus grande -> décaler pour avoir le texte 5 pixels plus haut
	SDL_Rect RectVie = { 1250, 22, 16, 16 };
	SDL_Rect RectBombeImage = { 50, 35, 18, 18 };
	SDL_Rect RectBombeTexte = { 70, 30, 16, 16 };
	SDL_Rect RectCleImage = { 50, 12, 18, 18 };
	SDL_Rect RectCleTexte = { 70, 7, 16, 16 };
	
	/* Initialisation des textures pour les HUD "texte" */
	SDL_Texture *CleTexteTexture = NULL; //Texture pour le texte
	SDL_Texture *BombeTexteTexture = NULL; //Texture pour le texte
	
	/* Initialisation d'un compteur pour tester les HUD */
	Uint32 iCount = 0; //initialisation du compteur
	char szCounter[100];
	
	/* Initialisation des différents HUD */
	/* Initialisation HUD Vie */
	Vie = HUD_Init("SDL_heat", FALSE);
	HUD_Load(Vie, RectVie);
	/* InitialisationHUD BombeImage */
	BombeImage = HUD_Init("SDL_Bomb", FALSE);
	HUD_Load(BombeImage, RectBombeImage);
	/* InitialisationHUD CleImage */
	CleImage = HUD_Init("SDL_Key", FALSE);
	HUD_Load(CleImage, RectCleImage);
	/* InitialisationHUD BombeTexte */
	BombeTexte = HUD_Init("BombeImage", TRUE);
	HUD_Load(BombeTexte, RectBombeTexte);
	/* InitialisationHUD CleTexte */
	CleTexte = HUD_Init("CleImage", TRUE);
	HUD_Load(CleTexte, RectCleTexte);


	/* ========================================================================= */
	/*                          CONFIGURATION BOUTON                             */
	/* ========================================================================= */

	/* Initialisation des variables de la police pour les Boutons */
	TTF_Font *policeBouton = NULL; //Variable pour la police des Boutons
	SDL_Color CouleurBouton = { 255, 255, 255 }; //Variable pour la couleur de la police des Boutons

	/* Ouverture de la police */
	policeBouton = Kr_Text_OpenFont("upheavtt", 40);
	TTF_SetFontStyle(policeBouton, TTF_STYLE_BOLD);

	/* Initialisation structure Bouton */
	Bouton *Charger = NULL;

	/* Initialisation du rectangle pour le bouton*/
	SDL_Rect RectImageCharger = { 440, 296, 400, 100 };
	SDL_Rect RectTexteCharger = { 540, 326, 40, 40 };

	/* Initialisation bouton Charger */
	Charger = Bouton_Init("CHARGER");
	Bouton_Load(gpRenderer, Charger, policeBouton, CouleurBouton, RectImageCharger, RectTexteCharger);

	/* Modification pour tester Bouton_Update */
	Bouton_Selectionne(Charger);
	Bouton_Update(Charger);



	
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
		/* Affichage/masquage des HUD grâce à la touche H */
		if (inEvent.szKey[SDL_SCANCODE_H])
		{
			Kr_Log_Print(KR_LOG_INFO, "TOUCHE H\n");
			Vie->estAffiche = ~Vie->estAffiche;
			BombeImage->estAffiche = ~BombeImage->estAffiche;
			BombeTexte->estAffiche = ~BombeTexte->estAffiche;
			CleImage->estAffiche = ~CleImage->estAffiche;
			CleTexte->estAffiche = ~CleTexte->estAffiche;
			inEvent.szKey[SDL_SCANCODE_H] = 0;
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
		
		/* Incrémentation du compteur : sert juste pour tester les HUD */
		iCount++;

		/* Génération d'un texte à partir du compteur (mise de la valeur dans un tableau) */
		sprintf(szCounter, "x%d", iCount); 

		/* Création des textures à partir du texte pour le nombre de munition ou de clés*/
		CleTexteTexture = Kr_Text_FontCreateTexture(gpRenderer, policeHUD, szCounter, CouleurHUD, TRUE, &(CleTexte->RectDest));
		BombeTexteTexture = Kr_Text_FontCreateTexture(gpRenderer, policeHUD, szCounter, CouleurHUD, TRUE, &(BombeTexte->RectDest));
		
		/* Mise à jour des HUD "texte" avec la nouvelle texture générée */
		HUD_Update(CleTexte, CleTexteTexture);
		HUD_Update(BombeTexte, BombeTexteTexture);
		

		/* ========================================================================= */
		/*                                  RENDER                                   */
		/* ========================================================================= */
		// Ici on gère l'affichage des surfaces
		SDL_RenderClear(gpRenderer); // Dans un premier temps on Clear le renderer
		HUD_Draw(gpRenderer, Vie, 5); // Affichage HUD Vie
		HUD_Draw(gpRenderer, BombeImage, 0); // Affichage HUD BombeImage
		HUD_Draw(gpRenderer, BombeTexte, 0); // Affichage HUD BombeTexte
		HUD_Draw(gpRenderer, CleImage, 0); // Affichage HUD CleImage
		HUD_Draw(gpRenderer, CleTexte, 0); // Affichage HUD CleTexte
		Bouton_Draw(gpRenderer, Charger); // Affichage Bouton Charger
		SDL_RenderPresent(gpRenderer); // Lorsque toutes les surfaces ont été placé on affiche le renderer (l'écran quoi...)

		/* Libération des deux textures HUD crées dans la boucle */
		UTIL_FreeTexture(&CleTexteTexture); 
		UTIL_FreeTexture(&BombeTexteTexture);

	}

	/* ========================================================================= */
	/*                            LIBERATION MEMOIRE                             */
	/* ========================================================================= */


	SDL_DestroyRenderer(gpRenderer);	// Libération mémoire du renderer
	SDL_DestroyWindow(pWindow);			// Libération mémoire de la fenetre
	/* Libération mémoire des HUD */
	HUD_free(Vie);
	HUD_free(CleImage);
	HUD_free(BombeImage);
	HUD_free(BombeTexte);
	HUD_free(CleTexte);
	Bouton_free(Charger);


	Mix_CloseAudio();	// On quitte SDL_MIXER
	TTF_Quit();			// On quitte SDL_TTF
	SDL_Quit();			// On quitte SDL
	Kr_Log_Quit();		// On ferme les logs

	return EXIT_SUCCESS;
}
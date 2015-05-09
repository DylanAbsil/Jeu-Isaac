/* ========================================================================= */
/*!
* \file    kr_sound.c
* \brief   Contains the functions to handle the sounds.
* \author  Olivier Herrou
* \version 1.0
* \date    28 Mars 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 28/03/2015 | Creation.                                    */
/* Herrou        | 31/03/2015 | Fix                                          */
/* Herrou        | 27/04/2015 | Ajout d'une structure Kr_Music				 */
/*               |            |                                              */
/* ========================================================================= */


#include "kr_sound.h"

#include "kr_util.h"
#include "kr_log.h"

/* ========================================================================= */

/*!
*  \fn     Mix_Chunk* Kr_Sound_LoadChunk( const char *szPath )
*  \brief  Function to load a sound.
*
*  \param  szPath Path of the sound to open. (with the extension)
*  \return A pointer on the loaded sound, or NULL if error.
*/
Mix_Chunk* Kr_Sound_LoadChunk(const char *szPath)
{
	Mix_Chunk *pChunk = Mix_LoadWAV(szPath);

	if (pChunk == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't load a sound!\n");
		Kr_Log_Print(KR_LOG_ERROR, ">> Path \"%s\".\n", szPath);
	}

	return pChunk;
}

/*!
*  \fn     void Kr_Sound_FreeChunk( Mix_Chunk **ppChunk )
*  \brief  Function to free a sound.
*
*  \param  ppChunk Pointer to pointer of the sound to free.
*  \return None.
*/
void Kr_Sound_FreeChunk(Mix_Chunk **ppChunk)
{
	Mix_FreeChunk(*ppChunk);
	*ppChunk = NULL;
}

/*!
 *  \fn     Kr_Sound* Kr_Sound_Alloc( const char *szSndName )
 *  \brief  Function to load a sound.
 *
 *  \param  szSndName Name of the sound. (Without the extension)
 *  \return A pointer on the loaded sound, or NULL if error.
 */
Kr_Sound* Kr_Sound_Alloc( const char *szSndName )
{
    Kr_Sound *pSound;
	char      pSoundPath[50];
	size_t    iSoundLen;
    
    pSound   = NULL;
	iSoundLen = strlen(szSndName);
	sprintf(pSoundPath, "sounds\\%s.wav", szSndName);
    
	if (pSoundPath)
    {
        pSound = ( Kr_Sound * ) UTIL_Malloc( sizeof( Kr_Sound ));

        if( pSound )
        {
			pSound->pChunk = Kr_Sound_LoadChunk(pSoundPath);
			pSound->szSndName = UTIL_CopyStr(szSndName, iSoundLen);

            if( !pSound->pChunk || !pSound->szSndName ) // Error: must free...
            {
                if( pSound->pChunk )
                {
					Kr_Sound_FreeChunk(&pSound->pChunk);
                }
                if( pSound->szSndName )
                {
                    UTIL_Free( pSound );
                }
                
                UTIL_Free( pSound );
            }
        }
    }

    return pSound;
}

/*!
 *  \fn     void Kr_Sound_Print( Kr_Sound *pSound )
 *  \brief  Function to print the data of a sound.
 *
 *  \param  pSound Pointer on the sound.
 *  \return None.
 */
void Kr_Sound_Print( Kr_Sound *pSound )
{
    Kr_Log_Print( KR_LOG_INFO, "Precache sound \"%s\".\n", pSound->szSndName );
}

/*!
 *  \fn     void Kr_Sound_Play( Kr_Sound *pSound, Uint32 iChannel, Uint32 iVolume, Sint32 iLoops )
 *  \brief  Function to play a sound.
 *
 *  \param  pSound   Pointer on a loaded sound.
 *  \param  iChannel Channel to play the sound.
 *  \param  iVolume  Volume to play the sound.
 *  \param  iLoops   Number of times the sound must be looped.
 *  \return None.
 */
void Kr_Sound_Play( Kr_Sound *pSound, Uint32 iChannel, Uint32 iVolume, Sint32 iLoops )
{
	if (!pSound) return;
    Mix_VolumeChunk( pSound->pChunk, iVolume );
    Mix_PlayChannel( iChannel, pSound->pChunk, iLoops );
}

/*!
 *  \fn     void Kr_Sound_Free( Kr_Sound **ppSound )
 *  \brief  Function to free a sound.
 *
 *  \param  ppSound Pointer to pointer of the sound to free.
 *  \return None.
 */
void Kr_Sound_Free( Kr_Sound **ppSound )
{
    UTIL_Free(( *ppSound )->szSndName );
	Kr_Sound_FreeChunk(&(*ppSound)->pChunk);
    UTIL_Free( *ppSound );
}



/*!
*  \fn     void Kr_Sound_PlayOnce(const char *szSndName, Uint32 iChannel, Uint32 iVolume)
*  \brief  Function to play a sound one time on a specific canal
*
*  \param  szSndName the name of the sound to be played
*  \param  iChannel  Channel to play the sound.
*  \param  iVolume   Volume to play the sound.
*  \return None.
*/
void Kr_Sound_PlayOnce(const char *szSndName, Uint32 iChannel, Uint32 iVolume)
{
	Kr_Sound *pSound = NULL;

	pSound = Kr_Sound_Alloc(szSndName);
	if (!pSound)
	{
		Kr_Log_Print(KR_LOG_WARNING, "Can't load the sound \"%s\".\n", szSndName);
		return;
	}

	Kr_Sound_Play(pSound, iChannel, iVolume, 0);
// Problème, ici il faut attendre le temps du son avant de free sinon le son ne sera pas jouer
	Kr_Sound_Free(&pSound);
}




/* ========================================================================= */



/*!
*  \fn     Kr_Music* Kr_Sound_InitMusic(void);
*  \brief  Function to initialize a music structure.

*  \return A pointer on the loaded structure, or NULL if error.
*/
Kr_Music* Kr_Sound_InitMusic(void)
{
	Kr_Music *pMusic = NULL;
	pMusic = (Kr_Music *)UTIL_Malloc(sizeof(Kr_Music));
	return pMusic;
}


/*!
*  \fn     Boolean Kr_Sound_LoadMusic(Kr_Music *pMusic, const char *szMusicName )
*  \brief  Function to load a music.
*
*  \param  szMusicName Name of the music. (Without the extension)
*  \return True if everything is ok, False otherwise
*/
Boolean Kr_Sound_LoadMusic(Kr_Music *pMusic, const char *szMusicName)
{
	Mix_Music *pMsc = NULL;
	char	   szMusicPath[50];
	size_t     iPathLen;
	Uint32     iNameLen = 0;
	iPathLen = strlen(szMusicName);
	sprintf(szMusicPath, "sounds\\%s.mp3", szMusicName);

	iNameLen = strlen(szMusicName) - 1;
	pMusic->szMscName = UTIL_CopyStr(szMusicName, iNameLen);
	pMusic->pMsc = NULL;
	if (!szMusicPath)
	{
		Kr_Log_Print(KR_LOG_ERROR, "The file %s.mp3 is not in the folder \"sounds\"\n", szMusicName);
		return FALSE;
	}

	pMsc = Mix_LoadMUS(szMusicPath);
	pMusic->pMsc = pMsc;
	if (pMsc == NULL)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Can't load a music!\n");
		Kr_Log_Print(KR_LOG_ERROR, ">> Path \"%s\".\n", szMusicPath);
		return FALSE;
	}

	return TRUE;
}

/*!
*  \fn     void Kr_Sound_FreeMusic(Kr_Music *pMusic)
*  \brief  Function to free a music.
*
*  \param  pMusic a pointer to the Kr_Music structure to free
*  \return None.
*/
void Kr_Sound_FreeMusic(Kr_Music *pMusic)
{
	Mix_FreeMusic(pMusic->pMsc);
	UTIL_Free(pMusic);
}

void Kr_Sound_AllocInterract(void)
{
	Uint32 i = 0;
	Kr_Sound *aSoundInterraction[10];

	for (i = 0; i < 10; i++)
	{
		aSoundInterraction[i] = NULL;
	}
	aSoundInterraction[0] = Kr_Sound_Alloc("ouverture_coffre");
}



/* Utilisation de cette bibliothèque */
/* Remarque, ne pas compiler ce code, il faut l'adapter pour effectuer un code exploitable/analysable */
/*void main(void)
{
	// Documentation : http://sdl.beuc.net/sdl.wiki/SDL_mixer

	// Gestion de la musique
	Mix_Music   *pMusique = NULL; // Initialisation 
	pMusique = Kr_Sound_LoadMusic("Adele"); // Cette fonction va renvoyé une structure Mix_Music avec la musique chargée, ou NULL si une erreur
	if (!pMusique)
	{
		Kr_Log_Print(KR_LOG_ERROR, "La musique n'a pas été chargé correctement !\n");
	}
	Mix_PlayMusic(pMusique, -1);// Permet de jouer la musique une seul fois
	Mix_VolumeMusic(100);		// Réglage du volume de la musique (0 à 128)
	if (Mix_PausedMusic())		// Renvoie 1 si le son est en pause
		Mix_ResumeMusic();		// Relance la musique
	else Mix_PauseMusic();		// Met en pause la musique
	Mix_HaltMusic();			// Arrete complétement la musique

	Kr_Sound_FreeMusic(&pMusique);// Libération mémoire de la musique !!

	// Gestion des Sons
	Kr_Sound *pSon = NULL; // Initialisation
	Mix_AllocateChannels(1); // Allocation du nombre de cannaux

	pSon = Kr_Sound_Alloc("arrow_x"); // Cette fonction va renvoyé une structure Kr_Sound avec le son chargé, ou NULL si une erreur
	if (!pSon)
	{
		Kr_Log_Print(KR_LOG_ERROR, "Le son n'a pas été chargé correctement\n");
	}

	Kr_Sound_Play(pSon, 0, 100, 0); // Joue le son contenu dans la structure pSon, sur le channel 0 avec un volume de 100 et 0 répétions
	if (Mix_Paused(0))				// renvoie 1 si le canal est en pause
		Mix_Resume(0);				// Reprend le son sur le canal 0
	else Mix_Pause(0);				// Met en pause le canal 0	
	Mix_HaltChannel(-1);			// Arrete le son sur le canal indiqué, ici -1 pour tous les canaux

	Kr_Sound_Free(&pSon);			// Libération mémoire du son !!
	
}*/



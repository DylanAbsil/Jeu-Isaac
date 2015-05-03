/* ========================================================================= */
/*!
* \file    kr_sound.c
* \brief   Contains the structure to handle the sounds.
* \author  Olivier Herrou
* \version 1.0
* \date    28 Mars 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 28/03/2015 | Creation.                                    */
/* Herrou        | 21/04/2015 | Ajout de define pour la gestion des canaux   */
/* Herrou        | 27/04/2015 | Ajout d'une structure Kr_Music				 */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */


#ifndef __KR_SOUND_H__
#define __KR_SOUND_H__
    
    #include "kr_common.h"
	
#define KR_SOUND_MAP_EVENT_CANAL 0
#define KR_SOUND_ANIMAL_CANAL 1
#define KR_SOUND_NATURE_CANAL 2
    /*! 
     * \struct Kr_Sound
     * \brief  Structure to handle the sound.
     */
    typedef struct
    {
        char      *szSndName; /*!< Name of the sound. */
        Mix_Chunk *pChunk;    /*!< Pointer on the sound structure. */
    } Kr_Sound;

	/*!
	* \struct Kr_Music
	* \brief  Structure to handle the music.
	*/
	typedef struct
	{
		char      *szMscName; /*!< Name of the music. */
		Mix_Music *pMsc;    /*!< Pointer on the music structure. */
	} Kr_Music;

    Kr_Sound*  Kr_Sound_Alloc( const char *szSndName );
    void       Kr_Sound_Print( Kr_Sound *pSound );
    void       Kr_Sound_Play( Kr_Sound *pSound, Uint32 iChannel, Uint32 iVolume, Sint32 iLoops );
    void       Kr_Sound_Free( Kr_Sound **ppSound );
	Mix_Chunk* Kr_Sound_LoadChunk(const char *szPath);
	void       Kr_Sound_FreeChunk(Mix_Chunk **ppChunk);
	void	   Kr_Sound_PlayOnce(const char *szSndName, Uint32 iChannel, Uint32 iVolume);

	Kr_Music* Kr_Sound_InitMusic(void);
	Boolean   Kr_Sound_LoadMusic(Kr_Music *pMusic, const char *szPath);
	void      Kr_Sound_FreeMusic(Kr_Music *pMusic);

	void Kr_Sound_AllocInterract(void);
	void Kr_Sound_FreeInterract(Kr_Sound **aSoundInterract);

#endif /* __KR_SOUND_H__ */

/* ========================================================================= */

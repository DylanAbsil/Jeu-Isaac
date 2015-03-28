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
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */


#ifndef __KR_SOUND_H__
#define __KR_SOUND_H__
    
    #include "kr_common.h"

    /*! 
     * \struct Kr_Sound
     * \brief  Structure to handle the sound.
     */
    typedef struct
    {
        char      *szSndName; /*!< Name of the sound. */
        Mix_Chunk *pChunk;    /*!< Pointer on the sound structure. */
    } Kr_Sound;

    Kr_Sound*  Kr_Sound_Alloc( const char *szSndName );
    void       Kr_Sound_Print( Kr_Sound *pSound );
    void       Kr_Sound_Play( Kr_Sound *pSound, Uint32 iChannel, Uint32 iVolume, Sint32 iLoops );
    void       Kr_Sound_Free( Kr_Sound **ppSound );
	Mix_Chunk* Kr_Sound_LoadChunk(const char *szPath);
	void       Kr_Sound_FreeChunk(Mix_Chunk **ppChunk);


	Mix_Music* Kr_Sound_LoadMusic(const char *szPath);
	void       Kr_Sound_FreeMusic(Mix_Music **ppMusic);

#endif /* __KR_SOUND_H__ */

/* ========================================================================= */

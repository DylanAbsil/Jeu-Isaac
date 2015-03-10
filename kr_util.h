/* ========================================================================= */
/*!
 * \file    kr_util.h
 * \brief   Contains some useful and standard functions.
 * \author  Olivier Herrou
 * \version 1.0
 * \date    1 Mars 2015
 */
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 01/03/2015 | Creation.                                    */
/*               |            |                                              */
/* ========================================================================= */

#ifndef __KR_UTIL_H__
#define __KR_UTIL_H__

    #include "kr_common.h"

extern SDL_Renderer *gpRenderer; // Renderer is a global var from main
    
    Uint32       UTIL_Sqrt32( Uint32 iValue );
    FILE*        UTIL_OpenFile( const char *szPath, const char *szMode );
    void         UTIL_CloseFile( FILE **ppFile );
    SDL_Texture* UTIL_LoadTexture( const char *szPath, SDL_Color *pTransColor, SDL_Rect *pTextureSize ); 
    void         UTIL_FreeTexture( SDL_Texture **ppTexture );
    void*        UTIL_Malloc( size_t iSize );
    void*        UTIL_Realloc( void *pData, size_t iSize );
    char*        UTIL_CopyStr( const char *szSrc, size_t iSrcLen );
    char*        UTIL_BuildPath( const char *szFolder, const char *szFile, size_t iFileLen, const char *szExt );


    /*! Macro to free memory block. */
    #define      UTIL_Free(x) do { free( x ); x = NULL; } while( 0 )

#endif /* __KR_UTIL_H__ */

/* ========================================================================= */

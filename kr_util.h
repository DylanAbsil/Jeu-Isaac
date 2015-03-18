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
/* Herrou        | 18/03/2015 | Suppression UTIL_BuildPath                   */
/*               |            | Ajout des macro UTIL_ABS et UTIL_SGN         */
/*               |            |                                              */
/*               |            |                                              */
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


    /*! Macro to free memory block. */
    #define      UTIL_Free(x) do { free( x ); x = NULL; } while( 0 )

	/*! Macro to check the signe of a value - Return 1 if positive, -1 if negative, 0 if null. */
	#define UTIL_SGN(X) (((X)==0)?(0):(((X)<0)?(-1):(1)))

	/*! Macro to return the absolute value */
	#define UTIL_ABS(X) ((((X)<0)?(-(X)):(X)))

#endif /* __KR_UTIL_H__ */

/* ========================================================================= */

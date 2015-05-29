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
/* Herrou        | 24/03/2015 | MAJ LoadTexture, le renderer n'est plus une  */
/*				 |            | var global, il est mit en paramètre          */
/*               |            | Add UTIL_SousChaine                          */
/* Herrou        | 05/04/2015 | Add UTIL_StrToUint32						 */
/* Herrou        | 06/04/2015 | Remove gpRenderer  							 */
/* Herrou        | 07/04/2015 | Add UTIL_FileCopy 							 */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#ifndef __KR_UTIL_H__
#define __KR_UTIL_H__

    #include "kr_common.h"

    Uint32       UTIL_Sqrt32( Uint32 iValue );
	Sint32		 UTIL_FindMiddle(Sint32 coord1, Sint32 coord2);
    FILE*        UTIL_OpenFile( const char *szPath, const char *szMode );
    void         UTIL_CloseFile( FILE **ppFile );
    SDL_Texture* UTIL_LoadTexture(SDL_Renderer *pRenderer, const char *szPath, SDL_Color *pTransColor, SDL_Rect *pTextureSize ); 
    void         UTIL_FreeTexture( SDL_Texture **ppTexture );
    void*        UTIL_Realloc( void *pData, size_t iSize );
    char*        UTIL_CopyStr( const char *szSrc, size_t iSrcLen );
	void		 UTIL_SousChaine(const char *szChaine1, Uint32 iPosDebut, Uint32 iPosFin, char *p_szExtrait);
	Sint32		 UTIL_StrToUint32(char *szString);
	Boolean		 UTIL_FileCopy(FILE *pFileSrc, FILE *pFileDst, char *szEnd);

    /*! Macro to free memory block. */
    #define      UTIL_Free(x) do { free( x ); x = NULL; } while( 0 )

	/*! Macro to check the signe of a value - Return 1 if positive, -1 if negative, 0 if null. */
	#define UTIL_SGN(X) (((X)==0)?(0):(((X)<0)?(-1):(1)))

	/*! Macro to return the absolute value */
	#define UTIL_ABS(X) ((((X)<0)?(-(X)):(X)))

#endif /* __KR_UTIL_H__ */

/* ========================================================================= */

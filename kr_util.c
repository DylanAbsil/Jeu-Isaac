/* ========================================================================= */
/*!
 * \file    kr_util.c
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
/*               |            |                                              */
/* ========================================================================= */
#include "kr_util.h"

#include "kr_log.h"
/* ========================================================================= */

/*!
 *  \fn     Uint32 UTIL_Sqrt32( Uint32 iValue )
 *  \brief  Function to compute the sqroot of a value.
 *
 *  \param  iValue Value to compute.
 *  \return The sqroot of the value.
 */
Uint32 UTIL_Sqrt32( Uint32 iValue )
{
    Uint32 iCompute;
    Uint32 iIndex;

    if( iValue > 1 )
    {
        iCompute = iValue;

        for( iIndex = 0 ; iIndex < 13 ; iIndex++ )
        {
            iCompute = ( iCompute + iValue / iCompute ) >> 1;
        }

        iValue = iCompute;
    }
    
    return iValue;
}

/* ========================================================================= */

/*!
 *  \fn     FILE* UTIL_OpenFile( const char *szPath, const char *szMode )
 *  \brief  Function to open a file.
 *
 *  \param  szPath Path of the file to open.
 *  \param  szMode Mode used to open the file.
 *  \return A pointer on the opened file, or NULL if error.
 */
FILE* UTIL_OpenFile( const char *szPath, const char *szMode )
{
    FILE *pFile = fopen( szPath, szMode );
    
    if( pFile == NULL )
    {
        Kr_Log_Print( KR_LOG_ERROR, "Can't open a file ( Mode : %s ) !\n", strchr( szMode, 'r' ) ? "Read" : "Write" );
        Kr_Log_Print( KR_LOG_ERROR, ">> Path \"%s\".\n", szPath );
    }
    
    return pFile;
}

/*!
 *  \fn     void UTIL_CloseFile( FILE **ppFile )
 *  \brief  Function to close a file.
 *
 *  \param  ppFile Pointer to pointer of the file to close.
 *  \return None.
 */
void UTIL_CloseFile( FILE **ppFile )
{
    fclose( *ppFile );
    *ppFile = NULL;
}

/*!
 *  \fn     SDL_Texture* UTIL_LoadTexture( const char *szPath, SDL_Color *pTransColor, SDL_Rect *pTextureSize )
 *  \brief  Function to load a texture.
 *
 *  \param  szPath       Path of the texture to open.
 *  \param  pTransColor  Pointer to set the transparency of the texture (Can be NULL).
 *  \param  pTextureSize Pointer to get the size of the texture loaded (Can be NULL).
 *  \return A pointer on the loaded texture, or NULL if error.
 */

SDL_Texture* UTIL_LoadTexture( const char *szPath, SDL_Color *pTransColor, SDL_Rect *pTextureSize )
{
    Uint32       iColorKey;
    SDL_Texture *pTexture;
    SDL_Surface *pSurface;

    pTexture = NULL;
    pSurface = IMG_Load( szPath );

    if( pSurface == NULL )
    {
        Kr_Log_Print( KR_LOG_ERROR, "Can't load an image!\n" );
        Kr_Log_Print( KR_LOG_ERROR, ">> Path \"%s\".\n", szPath );
    }
    else
    {
        if( pTransColor && ( pTransColor->r || pTransColor->g || pTransColor->b ))
        {
            iColorKey = SDL_MapRGB( pSurface->format, pTransColor->r, pTransColor->g, pTransColor->b );
            SDL_SetColorKey( pSurface, SDL_TRUE, iColorKey );
        }

        if( pTextureSize )
        {
            pTextureSize->w = pSurface->w;
            pTextureSize->h = pSurface->h;
        }

		pTexture = SDL_CreateTextureFromSurface(gpRenderer, pSurface);
        
        if( pTexture == NULL )
        {
             Kr_Log_Print( KR_LOG_ERROR, "Can't create a texture!\n" );
             Kr_Log_Print( KR_LOG_ERROR, ">> Path \"%s\".\n", szPath );
        }

        SDL_FreeSurface( pSurface );
    }
    return pTexture;
}

/*!
 *  \fn     void UTIL_FreeTexture( SDL_Texture **ppTexture )
 *  \brief  Function to free a texture.
 *
 *  \param  ppTexture Pointer to pointer of the texture to free.
 *  \return None.
 */

void UTIL_FreeTexture( SDL_Texture **ppTexture )
{
    SDL_DestroyTexture( *ppTexture );
    *ppTexture = NULL;
}



/*!
 *  \fn     void* UTIL_Malloc( size_t iSize )
 *  \brief  Function to allocate memory block.
 *
 *  \param  iSize Size to allocate in bytes.
 *  \return A pointer on the allocated memory, or NULL if error.
 */
void* UTIL_Malloc( size_t iSize )
{
    void *pData = malloc( iSize );
    
    if( pData == NULL )
    {
        Kr_Log_Print( KR_LOG_ERROR, "Insufficient memory ( Size : %u ) !\n", iSize );
    }

    return pData;
}

/*!
 *  \fn     void* UTIL_Realloc( void *pData, size_t iSize )
 *  \brief  Function to reallocate memory block.
 *
 *  \param  pData Pointer to reallocate ( Can be NULL ).
 *  \param  iSize Size to reallocate in bytes.
 *  \return A pointer on the reallocated memory, or NULL if error.
 */
void* UTIL_Realloc( void *pData, size_t iSize )
{
    void *pNewData = realloc( pData, iSize );
    
    if( pNewData == NULL )
    {
        Kr_Log_Print( KR_LOG_ERROR, "Insufficient memory ( Size : %u ) !\n", iSize );
    }

    return pNewData;
}

/* ========================================================================= */

/*!
 *  \fn     char* UTIL_CopyStr( const char *szSrc, size_t iSrcLen )
 *  \brief  Function to allocate and copy a string.
 *
 *  \param  szSrc   String to copy.
 *  \param  iSrcLen Size of the string to copy ( Without '\0' ).
 *  \return A pointer on the allocated string, or NULL if error.
 */
char* UTIL_CopyStr( const char *szSrc, size_t iSrcLen )
{
    char *pDest = ( char * ) UTIL_Malloc( iSrcLen + 1 ); // + '\0'
    
    if( pDest )
    {
        strcpy( pDest, szSrc );
    }

    return pDest;
}

/*!
 *  \fn     char* UTIL_BuildPath( const char *szFolder, const char *szFile, size_t iFileLen, const char *szExt )
 *  \brief  Function to build a path.
 *
 *  \param  szFolder Name of the folder.
 *  \param  szFile   Name of the file.
 *  \param  iFileLen Size of the filename to copy ( Without '\0' ).
 *  \param  szExt    Name of the extension.
 *  \return A pointer on the allocated path, or NULL if error.
 */
char* UTIL_BuildPath( const char *szFolder, const char *szFile, size_t iFileLen, const char *szExt )
{
    size_t  iPathLen = strlen( szFolder ) + iFileLen + strlen( szExt );
    char   *pPath    = ( char * ) UTIL_Malloc( iPathLen + 3 ); // + '/' + '.' + '\0'
    
    if( pPath )
    {
        sprintf( pPath, "%s/%s.%s", szFolder, szFile, szExt );
    }
    
    return pPath;
}

/* ========================================================================= */

/* ========================================================================= */

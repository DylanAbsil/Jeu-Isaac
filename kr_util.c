/* ========================================================================= */
/*!
 * \file    kr_util.c
 * \brief   Contains some useful and standard functions.
 * \author  Olivier Herrou
 * \version 1.0
 * \date    1 Mars 2015
 */
/* ======================================================================================================== */
/* Developers    | Date       | Comments																	*/
/* --------------+------------+---------------------------------------------------------------------------- */
/* Herrou        | 01/03/2015 | Creation.																	*/
/* Herrou        | 18/03/2015 | Suppression UTIL_BuildPath													*/
/* Herrou        | 24/03/2015 | MAJ LoadTexture, le renderer n'est plus une									*/
/*				 |            | var global, il est mit en paramètre											*/
/*               |            | Add UTIL_SousChaine															*/
/* Herrou        | 04/04/2015 | Modification UTIL_SousChaine :											    */
/*               |            |  - La chaine à traiter ne doit pas contenir de caractère particulier		*/
/* Herrou        | 07/04/2015 | Add UTIL_FileCopy 															*/
/* ======================================================================================================== */
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

SDL_Texture* UTIL_LoadTexture(SDL_Renderer *pRenderer, const char *szPath, SDL_Color *pTransColor, SDL_Rect *pTextureSize )
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

		pTexture = SDL_CreateTextureFromSurface(pRenderer, pSurface);
        
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


/** \fn   void UTIL_SousChaine(const char *szChaine1, Uint32 iPosDebut, Uint32 iPosFin, char *p_szExtrait)
* \brief  Cette fonction permet d'extraire dans une chaine la sous-chaine comprise entre deux positions
* \param  szChaine1   Const chaine de caractere que l'on souhaite traiter, supprimer tout caractère particulier !!!! \0 et \n par exemple
* \param  iPosDebut   Entier indiquant le début de la sous-chaine, La premiere valeur de la chaine correspond à 0 et non 1!
* \param  iPosFin     Entier indiquant la fin de la sous-chaine
* \param  p_szExtrait Pointeur sur la sous-chaine qui a ete extraite
* \return Renvoie la sous-chaine à l'aide de p_szExtrait, si une erreur est survenue ce pointeur aura pour valeur NULL
*/
void UTIL_SousChaine(const char *szChaine1, Uint32 iPosDebut, Uint32 iPosFin, char *p_szExtrait)
{
	Uint32 iLengthChaine = 0, i = 0, j = 0;
	iLengthChaine = strlen(szChaine1);
	if (iPosDebut<0 || iPosFin> iLengthChaine  || iPosFin<iPosDebut) // Gestion des valeurs des positions
	{
		Kr_Log_Print(KR_LOG_ERROR, "UTIL_sousChaine : Can't extract the string, wrong position! \n");
		p_szExtrait = NULL;
		return;
	}
	for (i = iPosDebut; i <= iPosFin; i++) // Extraction de la sous-chaine : OK
	{
		*(p_szExtrait + j) = szChaine1[i];
		j++;
	}
}


/*!
*  \fn     Sint32 UTIL_StrToUint32(char *szString)
*  \brief  Function to convert a string to a Uint32
*
*  \param  szString  the string to convert
*  \return the Uint32 if everything is ok, -1 otherwise
*/
Sint32 UTIL_StrToUint32(char *szString)
{
	char  *p_conv;
	Sint32 iValue = 0;

	// Convertir avec strtol la chaine en chiffre 
	iValue = strtol(szString, &p_conv, 10); // Conversion en base 10
	if (p_conv != NULL)
	{
		if (*p_conv == '\0') // La conversion à réussi
		{
		}
		else // La conversion à échoué
		{
			Kr_Log_Print(KR_LOG_ERROR, "Can't convert the string '%s' to an integer, error is : %s\n ", szString, p_conv);
			return -1;
		}
	}
	return iValue;
}




/*!
*  \fn     Boolean UTIL_FileCopy(FILE *pFileSrc, FILE *pFileDst, char *szEnd)
*  \brief  Function to convert a string to a Uint32
*
*  \remarks : Longueur maximale d'une ligne 1000octets, les file pointer ne sont pas fermé
* 
*  \param  pFileSrc   file pointer to the source file
*  \param  pFileDst   file pointer to the source file
*  \param  szEnd      When this string is detected the copy of the file is stopped
*  \return TRUE if everything is ok, FALSE otherwise
*/
Boolean UTIL_FileCopy(FILE *pFileSrc, FILE *pFileDst, char *szEnd)
{
	char szBuf[1000];
	do
	{
		fgets(szBuf, sizeof(szBuf), pFileSrc);
		fputs(szBuf, pFileDst);
	}while (strstr(szBuf, szEnd) == NULL);
	return TRUE;
}

/* ========================================================================= */

/* ========================================================================= */

/* ========================================================================= */
/*!
 * \file    kr_common.h
 * \brief   Contains the standard includes, structures and enumerations.
 * \author   
 * \version 1.0
 * \date    1 Mars 2015
 */
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/*               | 01/03/2015 | Creation.                                    */
/*               |            |                                              */
/*               |            |                                              */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#ifndef __KR_COMMON_H__
#define __KR_COMMON_H__
    
    #ifdef _MSC_VER
        /*! Define to remove some useless warnings with Visual Studio. */
        #define _CRT_SECURE_NO_WARNINGS
    #endif 

    #include <stdio.h>
    #include <stdlib.h>
    #include <stdarg.h>
    #include <string.h>
    #include <time.h>

	#include <SDL2/SDL.h>
	#include <SDL2/SDL_mixer.h>
	#include <SDL2/SDL_ttf.h>
    #include <SDL2/SDL_image.h>



    /*! 
     * \enum  Boolean
     * \brief Enumeration to simulate the boolean type in C.
     */
    typedef enum
    { 
        FALSE = 0, /*!< Value for 'false' */ 
        TRUE  = 1  /*!< Value for 'true' */
    } Boolean;

    /*! Macro to set bits. */
    #define KR_BIT_SET(x,bits)   ( x |= bits )
    /*! Macro to get bits. */
    #define KR_BIT_GET(x,bits)   ( x & bits )
    /*! Macro to clear bits. */
    #define KR_BIT_CLEAR(x,bits) ( x &=~ bits )

#endif /* __KR_COMMON_H__ */

/* ========================================================================= */

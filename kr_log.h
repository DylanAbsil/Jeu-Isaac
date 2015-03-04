/* ========================================================================= */
/*!
 * \file    kr_log.h
 * \brief   Contains the structure to handle the logs.
 * \author  Olivier Herrou
 * \version 1.0
 * \date    1 Février 2015
 */
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 01/02/2015 | Creation.                                    */
/*               |            |                                              */
/*               |            |                                              */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#ifndef __KR_LOG_H__
#define __KR_LOG_H__

    #include "kr_common.h"

    /*! 
     * \enum  Kr_LogType
     * \brief Enumeration of the possible type of logs.
     */
    typedef enum
    {
        KR_LOG_INFO    = 0, /*!< Value for 'info' */
        KR_LOG_WARNING = 1, /*!< Value for 'warning' */
        KR_LOG_ERROR   = 2, /*!< Value for 'error' */
    } Kr_LogType;

    /*!
     * \struct Kr_Log
     * \brief  Structure to handle the logs.
     */
    typedef struct
    {
        FILE       *pLogFile;    /*!< Pointer on the logs file. */
        const char *szLogName;   /*!< Name of the logs file. */
        Kr_LogType  iPrintLevel; /*!< Type of logs allowed. */
		Boolean     bIsEmpty;    /*!< Boolean to indicate if the file is empty. */
    } Kr_Log;

    void Kr_Log_Init( Kr_LogType iPrintLevel );
    void Kr_Log_Print( Kr_LogType iPrintLevel, const char *szFormat, ... );
    void Kr_Log_Quit( void );

#endif /* __KR_LOG_H__ */

/* ========================================================================= */

/* Fonction main pour tester
	
	Kr_Log_Init(KR_LOG_INFO); // Si INFO alors on affiche tout, si WARNING alors seulement WARNING et ERROR, si ERROR alors juste ERREUR
	Kr_Log_Print(KR_LOG_INFO, "LOG D'INFO \n");
	Kr_Log_Print(KR_LOG_WARNING, "LOG DE WARNING \n");
	Kr_Log_Print(KR_LOG_ERROR, "LOG D'ERREUR\n");
	Kr_Log_Quit();
*/
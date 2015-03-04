/* ========================================================================= */
/*!
 * \file    kr_log.c
 * \brief   Contains the functions to handle the logs.
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

#include "kr_log.h"

/* ========================================================================= */

/*! Global variable to handle the logs. */
static Kr_Log K_Log;

/* ========================================================================= */

/*!
 *  \fn     void Kr_Log_Init( Kr_LogType iPrintLevel )
 *  \brief  Function to initialize the logs.
 *
 *  \param  iPrintLevel Level of print.
 *  \return None
 */
void Kr_Log_Init( Kr_LogType iPrintLevel )
{
    char       szTimeBuffer[ 128 ];
    time_t     sTime;
    struct tm *pTimeinfo;

    K_Log.szLogName = "LogMoteur.txt";
    K_Log.pLogFile  = fopen( K_Log.szLogName, "w" );

    if( K_Log.pLogFile )
    {
        K_Log.iPrintLevel = iPrintLevel;
        K_Log.bIsEmpty    = TRUE;

        time( &sTime );
        pTimeinfo = localtime( &sTime );
        strftime( szTimeBuffer, sizeof( szTimeBuffer ), "%A %Y-%m-%d -- %H:%M:%S", pTimeinfo );

        fprintf( K_Log.pLogFile,"***********************************************************************\n" );
        fprintf( K_Log.pLogFile,"                    %s\n", szTimeBuffer );
        fprintf( K_Log.pLogFile,"***********************************************************************\n\n" );
    }
}

/*!
 *  \fn     void Kr_Log_Print( Kr_LogType iPrintLevel, const char *szFormat, ... )
 *  \brief  Function to print a log message.
 *
 *  \param  iPrintLevel Enum which indicates the prefix of log message to use ( cf enum Kr_LogType ).
 *  \param  szFormat    The formatted message you want to write.
 *  \return None
 */
void Kr_Log_Print( Kr_LogType iPrintLevel, const char *szFormat, ... )
{
    static const char *szLogTxt[ ] = { "Info    : ", "Warning : ", "Error   : " };
    va_list            ap;

    if( K_Log.pLogFile && K_Log.iPrintLevel <= iPrintLevel )
    {
        fputs( szLogTxt[ iPrintLevel ], K_Log.pLogFile );

        va_start( ap, szFormat );
        vfprintf( K_Log.pLogFile, szFormat, ap );
        va_end( ap );

        // If crash...
        fflush( K_Log.pLogFile );

        K_Log.bIsEmpty = FALSE;
    }
}

/*!
 *  \fn     void Kr_Log_Quit( void )
 *  \brief  Function to close the logs.
 *
 *  \return None
 */
void Kr_Log_Quit( void )
{
    if( K_Log.pLogFile )
    {
        fclose( K_Log.pLogFile );
        K_Log.pLogFile = NULL;

        if( K_Log.bIsEmpty )
        {
            remove( K_Log.szLogName );
        }
    }
}

/* ========================================================================= */

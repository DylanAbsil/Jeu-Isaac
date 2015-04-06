/* ========================================================================= */
/*!
* \file    level_editor.c
* \brief   Contains the functions to handle the level editor.
* \author  Olivier Herrou
* \version 1.0
* \date    06 Avril 2015
*/
/* ================================================================================================================ */
/* Developers    | Date       | Comments																			*/
/* --------------+------------+------------------------------------------------------------------------------------ */
/* Herrou        | 06/04/2015 | Création																			*/
/*               |            |         																			*/
/*               |            |         																			*/
/*               |            |         																			*/
/*               |            |         																			*/
/*               |            |         																			*/
/*               |            |         																			*/
/* ===============================================================================================================  */


#include "level_editor.h"


/*!
*  \fn     Level_Editor *Level_Editor_Init(char *szEditorFile)
*  \brief  Function to initialize a Level Editor structure thanks to the level editor file
*
*  \param  szEditorFile the name of the level editor file
*  \return the initialize structure
*/
Level_Editor *Level_Editor_Init(char *szEditorFile)
{
	Level_Editor *pEditor= NULL;

	Uint32 iNameLen = strlen(szEditorFile);
	pEditor = (Level_Editor *)UTIL_Malloc(sizeof(Level_Editor));

	pEditor->szEditorFile	= UTIL_CopyStr(szEditorFile, iNameLen);
	pEditor->szLevelVersion = NULL;
	pEditor->szLevelName	= NULL;
	pEditor->szTilesetName	= NULL;
	pEditor->iLevelNumber	= 0;
	pEditor->iTileHeight	= 0;
	pEditor->iTileWidth		= 0;
	pEditor->iStandardTile	= 0;

	return pEditor;
}


/*!
*  \fn     Level_Editor_Load(Level_Editor *pEditor);
*  \brief  Function to load a Level Editor structure thanks to the level editor file
*
*  \param  pEditor a pointer to the level editor structure
*  \return TRUE if everything is OK, FALSE otherwise
*/
Boolean	Level_Editor_Load(Level_Editor *pEditor)
{
	Uint32 iNameLen = 0;

	return TRUE;

}

/*!
*  \fn     void Level_Editor_Log(Level_Editor *pEditor)
*  \brief  Function to log the Level Editor structure
*
*  \param  pEditor a pointer to the level editor structure
*  \return none
*/
void Level_Editor_Log(Level_Editor *pEditor)
{
	Kr_Log_Print(KR_LOG_INFO, "Level Editor Log\n\n");
	Kr_Log_Print(KR_LOG_INFO, "=============================\n");
	Kr_Log_Print(KR_LOG_INFO, "Level Editor file : %s\n",pEditor->szEditorFile);
	Kr_Log_Print(KR_LOG_INFO, "Tileset File : %s\n", pEditor->szTilesetName);
	Kr_Log_Print(KR_LOG_INFO, "Level Version : %s\n", pEditor->szLevelVersion);
	Kr_Log_Print(KR_LOG_INFO, "Level Name : %s\n", pEditor->szLevelName);
	Kr_Log_Print(KR_LOG_INFO, "Level Number : %d\n", pEditor->iLevelNumber);
	Kr_Log_Print(KR_LOG_INFO, "Map Size : %d * %d = %d\n", pEditor->iTileWidth, pEditor->iTileHeight, pEditor->iTileWidth * pEditor->iTileHeight);
	Kr_Log_Print(KR_LOG_INFO, "Standard Tile : %d\n", pEditor->iStandardTile);
	Kr_Log_Print(KR_LOG_INFO, "End of log \n");
	Kr_Log_Print(KR_LOG_INFO, "=============================\n");
}

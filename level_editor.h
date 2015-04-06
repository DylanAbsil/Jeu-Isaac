/*!
* \file    level_editor.h
* \brief   Contains the structure to handle the level editor.
* \author  Olivier Herrou
* \version 1.0
* \date    06 Avril 2015
*/
/* ================================================================================================================ */
/* Developers    | Date       | Comments																			*/
/* --------------+------------+------------------------------------------------------------------------------------ */
/* Herrou        | 06/04/2015 | Cr�ation																			*/
/*               |            |         																			*/
/*               |            |         																			*/
/*               |            |         																			*/
/*               |            |         																			*/
/*               |            |         																			*/
/*               |            |         																			*/
/* ===============================================================================================================  */

#ifndef __LEVEL_EDITOR_H__
#define __LEVEL_EDITOR_H__

#include "kr_common.h"
#include "kr_log.h"

/*!
* \struct Level_Editor
* \brief  Structure to handle the level editor.
*/
typedef struct
{
	char *szEditorFile;		/*!< The name of the editor file */
	char *szLevelVersion;	/*!< The version of the level the editor will generate */
	char *szLevelNumber;	/*!< The number of the level */
	char *szLevelName;		/*!< The name of the level */
	char *szTilesetName;	/*!< The name of the tileset file */
	Uint32 iTileWidth;		/*!< The number of tile (horizontal) of the level */
	Uint32 iTileHeight;		/*!< The number of tile (vertical) of the level */
	Uint32 iStandardTile;	/*!< The number of the standard tile (background) */

}Level_Editor;





#endif /* __LEVEL_EDITOR_H__ */
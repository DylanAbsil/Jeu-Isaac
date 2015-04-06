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
#include "kr_util.h"
#include "kr_tileset.h"
#include "kr_level.h"
#include "kr_log.h"

/*!
* \struct Level_Editor
* \brief  Structure to handle the level editor.
*/
typedef struct
{
	char		*szEditorFile;		/*!< The name of the editor file */
	Uint32		 iStandardTile;		/*!< The number of the standard tile (background) */
	Kr_Level	*pLevel;			/*!< The level */
}Level_Editor;

/*!
* \struct Grid
* \brief  Structure to handle the Grid which square the map
*/
typedef struct
{
	SDL_Texture *pTexture;			/*!< The texture */
	SDL_Rect     Rect;				/*!< The rectangle*/
}Grid;

Level_Editor *Level_Editor_Init(char *szEditorFile);
Boolean		  Level_Editor_Load(Level_Editor *pEditor, SDL_Renderer *pRenderer);
void		  Level_Editor_Log(Level_Editor *pEditor);
void		  Level_Editor_Free(Level_Editor *pEditor);
Boolean	      Level_Editor_LoadLevel(Level_Editor *pEditor, char *szLevelFile, SDL_Renderer *pRenderer);
Boolean		  Level_Editor_CreateLevelFile(Kr_Level *pLevel);

Grid  *Grid_Init(char *szFileName, Kr_Level *pLevel, SDL_Renderer *pRenderer);
void   Grid_Free(Grid *pGrid);
void   Grid_Draw(Grid *pGrid, Kr_Level *pLevel, Boolean bMustDraw, SDL_Renderer *pRenderer);

void Level_Editor_DrawTiles(Kr_Tileset *pTileset, Boolean bMustPrint, SDL_Renderer *pRenderer);

#endif /* __LEVEL_EDITOR_H__ */
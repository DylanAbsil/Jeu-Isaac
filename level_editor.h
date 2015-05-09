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
/* Herrou        | 06/04/2015 | Création																			*/
/* Herrou        | 07/04/2015 | Add Level_Editor_GetTile        													*/
/* Herrou        | 09/04/2015 | Sauvegarde des données, gestion de la sélection de groupe de tiles					*/
/* Herrou        | 10/04/2015 | Ajout de la fonction Editor															*/
/* Herrou        | 20/04/2015 | Transfert des fonctions SaveLayout et WriteLayout dans Kr_Level						*/
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
#include "kr_input.h"
#include "kr_fps.h"
#include "kr_config.h"

/*!< The max number of tile in a group selection */
#define LEVEL_EDITOR_MAX_SELECTION 100

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

Uint32		  Editor(SDL_Renderer *pRenderer, SDL_Window *pWindow);


Level_Editor *Level_Editor_Init(char *szEditorFile);
Boolean		  Level_Editor_Load(Level_Editor *pEditor, SDL_Renderer *pRenderer);
void		  Level_Editor_Log(Level_Editor *pEditor);
void		  Level_Editor_Free(Level_Editor *pEditor);
Boolean	      Level_Editor_LoadLevel(Level_Editor *pEditor, char *szLevelFile, SDL_Renderer *pRenderer);
Boolean		  Level_Editor_CreateLevelFile(Kr_Level *pLevel);

Grid  *Grid_Init(char *szFileName, Kr_Level *pLevel, SDL_Renderer *pRenderer);
void   Grid_Free(Grid *pGrid);
void   Grid_Draw(Grid *pGrid, Kr_Level *pLevel, Boolean bMustDraw, SDL_Renderer *pRenderer);

void	Level_Editor_PrintTiles(Kr_Tileset *pTileset, Boolean bMustPrint, SDL_Renderer *pRenderer);
Sint32  Level_Editor_GetTile(Level_Editor *pEditor, Sint32 x, Sint32 y, Boolean tilesetIsShown);
void	Level_Editor_PreDrawTile(Level_Editor *pEditor, Uint32 iNumTile, Uint32 x, Uint32 y, Boolean bMustDraw, SDL_Renderer *pRenderer, SDL_Texture *pTextureSelected);


/* Group selection*/
Boolean Level_Editor_SelectingGroup(Sint32 *iTabCursor, Kr_Input *inEvent);
Boolean Level_Editor_GroupFill(Sint32 *iTabTile, Sint32 *iTabCursor, Level_Editor *pEditor, Boolean tilesetIsShown);
void    Level_Editor_PreDrawTileSelection(Level_Editor *pEditor, Sint32 *iTabTile, Uint32 x, Uint32 y, Boolean bPreDraw, SDL_Renderer *pRenderer, Sint32 *iTabNbTiles, SDL_Texture *pTextureSelected);
void    Level_Editor_WriteLayoutSelection(Level_Editor *pEditor, Sint32 *iTabTile, Uint32 x, Uint32 y, Sint32 *iTabCursor);


#endif /* __LEVEL_EDITOR_H__ */
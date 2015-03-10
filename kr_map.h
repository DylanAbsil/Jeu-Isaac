/* ========================================================================= */
/*!
* \file    kr_map.h
* \brief   Contains the structure to handle the map.
* \author  Olivier Herrou
* \version 1.0
* \date    18 Février 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 18/02/2015 | Creation.                                    */
/*               | 21/02/2015 | Ajout de kr_quit                             */
/*               |            |                                              */
/* ========================================================================= */

#ifndef __KR_MAP_H__
#define __KR_MAP_H__

#include "kr_common.h"
#include "kr_util.h"
#include "kr_log.h"
#include "kr_input.h"
#include "kr_quit.h"

// http://openclassrooms.com/courses/tile-mapping/proprietes-des-tiles

#define KR_WIDTH_WINDOW  1280
#define KR_HEIGHT_WINDOW 720
#define MOVESPEED 2
//1280 //720
/*!
* \struct Kr_TileProp
* \brief  Structure to handle the properties of the tile.
*/
typedef struct
{
	SDL_Rect R;
	Uint32 iPlein;
	// tout ce que vous voulez...
} Kr_TileProp; 

/*!
* \struct Kr_Map
* \brief  Structure to handle the map.
*/
typedef struct
{ 
	Sint32          iTilesWidth;		 /*!< Width of a Tile */
	Sint32          iTilesHeight;		 /*!< Height of a Tile */
	Sint32          iNbTilesX;			 /*!< Number of Tile on X on the tileset */
	Sint32          iNbTilesY;			 /*!< Number of Tile on Y on the tileset */
	SDL_Texture    *pTextureTileset;	 /*!< pointer to the Texture of the tileset */
	Kr_TileProp    *tp_Props;			 /*!< Properties of each tile */
	unsigned char **szSchema;			 /*!< 2D array of the level (unsigned char = 256 different tiles max */
	Sint32          iNbTiles_WidthLevel; /*!< Number of Tile on X of the Level */
	Sint32          iNbTiles_HeightLevel;/*!< Number of Tile on Y of the Level */
	/* Scrolling */
	Sint32          iScrollX;			 /*!< X Coordinate of top left-hand corner of the Scroll Window*/
	Sint32          iScrollY;			 /*!< Y Coordinate of top left-hand corner of the Scroll Window*/
	Sint32          iWidth_MapWindow    ; /*!< Width of the window where the map will be render*/
	Sint32          iHeight_MapWindow;/*!< Height of the window where the map will be render  */
	SDL_Rect        rectLimitScroll;/*!< Rectangle to limit the scroll window  */
	SDL_Rect       *rectToFocus; /*!< Rectangle which will be focus by the scroll window  */
}Kr_Map;

void    Kr_Map_LoadLevel(FILE *pFile, Kr_Map *pMap);
void    Kr_Map_LoadTileset(FILE *pFile, Kr_Map *pMap);
Kr_Map* Kr_Map_LoadMap(const char *szLevel, Uint32 iWidth_ScrollWindow, Uint32 iHeight_ScrollWindow);
void    Kr_Map_Draw(SDL_Renderer *pRenderer, Kr_Map *pMap);
void    Kr_Map_Free(Kr_Map *pMap);
void    Kr_Map_UpdateScroll(Kr_Map *pMap, Kr_Input *inEvent);

/* Scrolling auto*/
void    Kr_Map_FocusScrollBox(Kr_Map *pMap, SDL_Rect *rectToFocus, SDL_Rect rectLimitScroll);
void    Kr_Map_ClampScroll(Kr_Map *pMap);
void    Kr_Map_UpdateScroll2(Kr_Map *pMap);
#endif /* __KR_MAP_H__ */

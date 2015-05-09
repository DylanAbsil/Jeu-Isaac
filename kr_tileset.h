/* ========================================================================= */
/*!
* \file    kr_tileset.h
* \brief   Contains the structure to handle the tileset.
* \author  Olivier Herrou
* \version 1.0
* \date    01 Mars 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 01/03/2015 | Création.                                    */
/* Herrou        | 22/03/2015 | Tiles Properties : Add iWater                */
/* Herrou        | 23/03/2015 | Tiles Properties : Add iPorteLevel           */
/* Herrou        | 08/04/2015 | Tiles Properties : Remove iPorteLevel        */
/*               |            | Tiles Properties : Add    iPorteMaison       */
/* Herrou        | 21/04/2015 | Tiles Properties : Add iCoffreFerme          */
/*               |            | Tiles Properties : Add iPanneau			     */
/* Herrou        | 01/05/2015 | Tiles Properties : Add iArbre		         */
/*               |            |                                              */
/* ========================================================================= */

#ifndef __KR_TILESET_H__
#define __KR_TILESET_H__

#include "kr_common.h"
#include "kr_util.h"
#include "kr_log.h"

#define KR_TILESET_VERSION "Tileset Version 1.0"
/*!
* \struct Kr_TileProp
* \brief  Structure to handle the properties of the tile.
*/
typedef struct
{
	SDL_Rect rTile;
	Uint32 iPlein;
	Uint32 iWater;
	Uint32 iPorteMaison;
	Uint32 iCoffreFerme;
	Uint32 iPanneau;
	Uint32 iArbre;
	// tout ce que vous voulez...
} Kr_TileProp;


/*!
* \struct Kr_Tileset
* \brief  Structure to handle the tileset.
*/
typedef struct
{
	char		*szTilesetName;		/*!< Name of the tileset */
	Sint32       iTilesWidth;		/*!< Width of a Tile */
	Sint32       iTilesHeight;		/*!< Height of a Tile */
	Sint32       iNbTilesX;			/*!< Number of Tiles on X axe on the tileset */
	Sint32       iNbTilesY;			/*!< Number of Tiles on Y axe on the tileset */
	SDL_Texture *pTextureTileset;   /*!< pointer to the Texture of the tileset */
	Kr_TileProp *pTilesProp;		/*!< properties of each tile */
}Kr_Tileset;



Kr_Tileset *Kr_Tileset_Init(const char *szFileName);
Boolean		Kr_Tileset_Load(Kr_Tileset *pTileset, SDL_Renderer *pRenderer);
void	    Kr_Tileset_Free(Kr_Tileset *pTileset);
void		Kr_Tileset_Log (Kr_Tileset *pTileset);

#endif /* __KR_TILESET_H__ */
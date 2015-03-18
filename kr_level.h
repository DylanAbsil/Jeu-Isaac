/* ========================================================================= */
/*!
* \file    kr_level.h
* \brief   Contains the structure to handle the level.
* \author  Olivier Herrou
* \version 1.0
* \date    01 Mars 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 01/03/2015 | Cr�ation.                                    */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#ifndef __KR_LEVEL_H__
#define __KR_LEVEL_H__

#include "kr_common.h"
#include "kr_util.h"
#include "kr_log.h"
#include "kr_tileset.h"

#define KR_LEVEL_VERSION "Level Version 1.0"
#define KR_WIDTH_WINDOW  32*40 // 1280
#define KR_HEIGHT_WINDOW 32*22 // 704
/*!
* \struct Kr_Level
* \brief  Structure to handle the level.
*/
typedef struct
{
	const char     *szLevelName;		/*!< Name of the level  */
	Sint32          iLevel_TileWidth;	/*!< Width of the level in Tile number */
	Sint32          iLevel_TileHeight;  /*!< Height of the level in Tile number */
	unsigned char **szLayout;			/*!< 2D array of the level (unsigned char = 256 different tiles max */
	Kr_Tileset     *pLevel_Tileset;		/*!< pointer to the tileset of this level */

}Kr_Level;




Kr_Level *Kr_Level_Init(const char *szFileName);
Boolean   Kr_Level_Load(Kr_Level *pLevel, SDL_Renderer *pRenderer);
void	  Kr_Level_Free(Kr_Level *pLevel);
Boolean	  Kr_Level_Layout(Kr_Level *pLevel, FILE *pFile);
void	  Kr_Level_Draw(SDL_Renderer *pRenderer, Kr_Level *pLevel);

/* Sauvegarde etc*/
Boolean	  Kr_Level_Save(Kr_Level *pLevel);



#endif /* __KR_LEVEL_H__ */
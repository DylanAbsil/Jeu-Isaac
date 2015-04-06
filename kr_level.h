/* ========================================================================= */
/*!
* \file    kr_level.h
* \brief   Contains the structure to handle the level.
* \author  Olivier Herrou
* \version 1.0
* \date    01 Mars 2015
*/
/* ============================================================================================ */
/* Developers    | Date       | Comments                                                        */
/* --------------+------------+---------------------------------------------------------------- */
/* Herrou        | 01/03/2015 | Création.                                    					*/
/* Herrou        | 18/03/2015 | Suppression des fonctions sur le scrolling   					*/
/*               |            | Mise à jour de la structure sans scrolling   					*/
/*               |            | Define pour taille de la fenêtre             					*/
/*               |            | Ajout fonction pour la gestion des collisions					*/
/* Herrou        | 21/03/2015 | MAJ szLayout unsigned char => Uint32         					*/
/* Herrou        | 22/03/2015 | Proto Kr_Level_GetBlock et Kr_Level_Event                       */  
/* Herrou        | 23/03/2015 | Renommer GetBlock en GetTile									*/
/*               |            | Kr_Level_Event retourne un code selon l'event					*/
/* Herrou        | 24/03/2015 | MAJ Free, Add Change											*/
/*               |            | MAJ Structure Kr_Level, ajout des numéros des levels adjacents	*/
/* Herrou        | 04/04/2015 | Modification paramètre de la structure :						*/
/*               |            |  - Modification du nom pour décrire le level, ce n'est plus le  */
/*               |            |    du fichier mais un nom plus spécifique au level              */
/*               |            |  - Ajout d'un numéro pour chaque niveau        					*/
/*               |            |  - Ajout d'un paramètre pour stocker le nom du fichier level    */
/*               |            | Création de la fonction Kr_Level_GetLevelNumber  				*/
/*               |            | Passage des fichiers levels en version: Level Version 1.1 		*/
/* Herrou        | 05/04/2015 | Suppression de Kr_Level_Event et Kr_GetLevelNumber				*/
/*               |            | Kr_Level_Change, prend en paramètre le numéro du level			*/
/* ============================================================================================ */

#ifndef __KR_LEVEL_H__
#define __KR_LEVEL_H__

#include "kr_common.h"
#include "kr_util.h"
#include "kr_log.h"
#include "kr_tileset.h"

#define KR_LEVEL_VERSION "Level Version 1.1"
/*!
* \struct Kr_Level
* \brief  Structure to handle the level.
*/
typedef struct
{
	char           *szLevelFile;		/*!< Name of the file of the level  */
	char		   *szLevelName;		/*!< Name of the level  */
	Sint32          iLevelNum;          /*!< Number of the level */
	Sint32          iLevel_TileWidth;	/*!< Width of the level in Tile number */
	Sint32          iLevel_TileHeight;  /*!< Height of the level in Tile number */
	Uint32        **szLayout;			/*!< 2D array of the level (sizeof(Uint32) tiles possible */
	Kr_Tileset     *pLevel_Tileset;		/*!< pointer to the tileset of this level */
	Uint32          iNumNord;		    /*!< The numero of the level with which is connected to the north */
	Uint32          iNumSud;		    /*!< The numero of the level with which is connected to the south */
	Uint32          iNumEst;		    /*!< The numero of the level with which is connected to the east */
	Uint32          iNumOuest;		    /*!< The numero of the level with which is connected to the west */
}Kr_Level;




Kr_Level *Kr_Level_Init(char *szFileName);
Boolean   Kr_Level_Load(Kr_Level *pLevel, SDL_Renderer *pRenderer);
void	  Kr_Level_Free(Kr_Level *pLevel);
Boolean	  Kr_Level_Layout(Kr_Level *pLevel, FILE *pFile);
void	  Kr_Level_Draw(SDL_Renderer *pRenderer, Kr_Level *pLevel);

/* Sauvegarde etc */
Boolean	  Kr_Level_Save(Kr_Level *pLevel);
Kr_Level *Kr_Level_Change(Kr_Level *pCurrentLevel, Uint32 iCurrentLevelNumber, SDL_Renderer *pRenderer);

/* Gestion des collisions */
Uint32  Kr_Collision_Move(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy);
Boolean Kr_Collision_IsCollisionDecor(Kr_Level *pLevel, SDL_Rect *pRect1);
Boolean Kr_Collision_TryMove(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy);
void    Kr_Collision_Affine(Kr_Level *pLevel, SDL_Rect *pRect1, Sint32 vx, Sint32 vy);

/* Détection d'évenement sur la carte */
Sint32 Kr_Level_GetTile(Kr_Level *pLevel, Uint32 x, Uint32 y);



#endif /* __KR_LEVEL_H__ */
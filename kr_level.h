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
/* Herrou        | 01/03/2015 | Cr?ation.                                    					*/
/* Herrou        | 18/03/2015 | Suppression des fonctions sur le scrolling   					*/
/*               |            | Mise ? jour de la structure sans scrolling   					*/
/*               |            | Define pour taille de la fen?tre             					*/
/*               |            | Ajout fonction pour la gestion des collisions					*/
/* Herrou        | 21/03/2015 | MAJ szLayout unsigned char => Uint32         					*/
/* Herrou        | 22/03/2015 | Proto Kr_Level_GetBlock et Kr_Level_Event                       */  
/* Herrou        | 23/03/2015 | Renommer GetBlock en GetTile									*/
/*               |            | Kr_Level_Event retourne un code selon l'event					*/
/* Herrou        | 24/03/2015 | MAJ Free, Add Change											*/
/*               |            | MAJ Structure Kr_Level, ajout des num?ros des levels adjacents	*/
/* Herrou        | 04/04/2015 | Modification param?tre de la structure :						*/
/*               |            |  - Modification du nom pour d?crire le level, ce n'est plus le  */
/*               |            |    du fichier mais un nom plus sp?cifique au level              */
/*               |            |  - Ajout d'un num?ro pour chaque niveau        					*/
/*               |            |  - Ajout d'un param?tre pour stocker le nom du fichier level    */
/*               |            | Cr?ation de la fonction Kr_Level_GetLevelNumber  				*/
/*               |            | Passage des fichiers levels en version: Level Version 1.1 		*/
/* Herrou        | 05/04/2015 | Suppression de Kr_Level_Event et Kr_GetLevelNumber				*/
/*               |            | Kr_Level_Change, prend en param?tre le num?ro du level			*/
/* Herrou        | 20/04/2015 | Transfert des fonctions SaveLayout et WriteLayout dans Kr_Level	*/
/* Herrou        | 22/04/2015 | Gestion des collisions effectu?s dans kr_collision				*/
/* Herrou        | 27/04/2015 | Ajout du param?tre szLevelMessage qui contient le message		*/
/*               |            |  qui est affich? sur les panneaux, maximum un panneau par level */
/*               |            | Passage en Level Version 1.3									*/
/*               |            | Passage en Level Version 1.4 et ajout de la musique du level	*/
/* ============================================================================================ */

#ifndef __KR_LEVEL_H__
#define __KR_LEVEL_H__

#include "kr_common.h"
#include "kr_util.h"
#include "kr_log.h"
#include "kr_tileset.h"
#include "kr_sound.h"

#define KR_LEVEL_VERSION "Level Version 1.4"
/*!
* \struct Kr_Level
* \brief  Structure to handle the level.
*/
typedef struct Kr_Level
{
	char           *szLevelFile;		/*!< Name of the file of the level  */
	char		   *szLevelName;		/*!< Name of the level  */
	char		   *szLevelMessage;		/*!< Message of the level  */
	Kr_Music	   *pMusic;				/*!< a pointer to the music structure of the level*/
	Sint32          iLevelNum;          /*!< Number of the level */
	Sint32          iLevel_TileWidth;	/*!< Width of the level in Tile number */
	Sint32          iLevel_TileHeight;  /*!< Height of the level in Tile number */
	Uint32        **szLayout;			/*!< 2D array of the level (sizeof(Uint32)) tiles possible */
	Kr_Tileset     *pLevel_Tileset;		/*!< pointer to the tileset of this level */
	Uint32          iNumNord;		    /*!< The numero of the level with which is connected to the north */
	Uint32          iNumSud;		    /*!< The numero of the level with which is connected to the south */
	Uint32          iNumEst;		    /*!< The numero of the level with which is connected to the east */
	Uint32          iNumOuest;		    /*!< The numero of the level with which is connected to the west */
	
}Kr_Level;




Kr_Level *Kr_Level_Init(char *szFileName);
Boolean   Kr_Level_Load(Kr_Level *pLevel, SDL_Renderer *pRenderer, Boolean bBackup);
void	  Kr_Level_Free(Kr_Level *pLevel);
Boolean	  Kr_Level_Layout(Kr_Level *pLevel, FILE *pFile);
void	  Kr_Level_Draw(SDL_Renderer *pRenderer, Kr_Level *pLevel);

Kr_Level *Kr_Level_Change(Kr_Level *pCurrentLevel, Uint32 iCurrentLevelNumber, SDL_Renderer *pRenderer);

/* Sauvegarde */
void      Kr_Level_WriteLayout(Kr_Level *pLevel, Uint32 iNumTile, Uint32 x, Uint32 y);
Boolean   Kr_Level_SaveLayout(Kr_Level *pLevel, Boolean bBackup);

/* D?tection d'?venement sur la carte */
Sint32   Kr_Level_GetTile(Kr_Level *pLevel, Uint32 x, Uint32 y);

#endif /* __KR_LEVEL_H__ */
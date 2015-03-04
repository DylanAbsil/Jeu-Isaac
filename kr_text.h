/* ========================================================================= */
/*!
 * \file    kr_ttf.h
 * \brief   Contains the structure to handle the use of SDL_ttf.
 * \author  Olivier Herrou
 * \version 1.0
 * \date    8 Février 2015
 */
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 08/02/2015 | Creation.                                    */
/*               |            |                                              */
/*               |            |                                              */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#ifndef __KR_TEXT_H__
#define __KR_TEXT_H__

    #include "kr_common.h"
	#include "kr_util.h"
	#include "kr_log.h"

	TTF_Font*    Kr_Text_OpenFont(const char *szFile, Uint32 iSize);
	SDL_Texture* Kr_Text_FontCreateTexture(SDL_Renderer *pRenderer, TTF_Font *pFont, const char *szText, SDL_Color Color, Boolean bBlended, SDL_Rect *pTextureSize);
	void         Kr_Text_CloseFont(TTF_Font **ppFont);
#endif /* __KR_TEXT_H__ */

/* ========================================================================= */

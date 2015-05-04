/* ========================================================================= */
/*!
* \file    message.h
* \brief   Contains the structure to handle the message
* \author  Olivier Herrou
* \version 1.0
* \date    4 Mai 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 04/05/2015 | Creation.                                    */
/*               |            |                                              */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

	#include "kr_common.h"
	#include "kr_log.h"
	#include "kr_text.h"

/*!
* \struct Message
* \brief  Structure to handle the message.
* \remarks : If the message is too long it won't be print
*/
typedef struct
{
	char			*szMessageName;			/*!< The name of the message */
	SDL_Texture		*pBackgroundTexture;	/*!< Texture of the text*/
	SDL_Renderer	*pRenderer;				/*!< The Renderer where the message will be print*/
	SDL_Rect		*pRectPosition;			/*!< The current position of the message */

	SDL_Rect		*pTextPosition;			/*!< The position of the text of the message*/
	SDL_Texture		*pTextTexture;			/*!< Texture of the text of the message*/
	SDL_Color        color;					/*!< The color of the text of the message*/
	TTF_Font		*pFont;					/*!< The Font of the textof the message */

	Uint32           iStayTime;				/*!< Number of seconds during which the message will be shown*/  //=> Besoin de calculer le temps en fonction des FPS
	Boolean          bMustShow;				/*!< Must we print the message ?*/
} Message;

Message *Message_Init(char *szMessageName, SDL_Renderer *pRenderer);
Boolean  Message_Load(Message *pMessage, SDL_Texture *pBackgroundTexture, Uint32 iStayTime, SDL_Color color, TTF_Font *pFont, SDL_Rect *pTextPosition, SDL_Texture *pTextTexture);
Boolean  Message_Update(Message *pMessage, Boolean bMustShow, char *szMessageText); // Met à jour la texture à afficher à partir du char* en entrée
void	 Message_Draw(Message *pMessage);
void	 Message_Free(Message *pMessage);
#endif /* __MESSAGE_H__ */

/* ========================================================================= */


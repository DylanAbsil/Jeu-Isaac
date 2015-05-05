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
/* Herrou        | 05/05/2015 | Gestion de l'affichage d'un message          */
/*               |            |                                              */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#ifndef __MESSAGE_H__
#define __MESSAGE_H__

	#include "kr_common.h"
	#include "kr_log.h"
	#include "kr_text.h"
	#include "kr_util.h"

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
	SDL_Rect		 rMessagePosition;		/*!< The position of the message */

	SDL_Rect		 rTextPosition;			/*!< The position of the text of the message*/
	SDL_Texture		*pTextTexture;			/*!< Texture of the text of the message*/
	SDL_Color        color;					/*!< The color of the text of the message*/
	TTF_Font		*pFont;					/*!< The Font of the text of the message */

	Uint32           iStayTime;				/*!< Number of seconds during which the message will be shown*/
	Uint32           iTime;					/*!< Number of loops during which the message has been shown*/ 
	
	Boolean          bMustShow;				/*!< Must we print the message ?*/
} Message;

Message *Message_Init(char *szMessageName, SDL_Renderer *pRenderer);
Boolean  Message_Load(Message *pMessage, char *szNameBackground, Uint32 iStayTime, SDL_Color color, TTF_Font *pFont, SDL_Rect rTextPosition);
Boolean  Message_Update(Message *pMessage, Boolean bMustShow, char *szMessageText); 
void	 Message_Draw(Message *pMessage);
void	 Message_Free(Message *pMessage);
#endif /* __MESSAGE_H__ */

/* ========================================================================= */


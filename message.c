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

#include "message.h"



/*!
*  \fn     Message *Message_Init(char *szMessageName, SDL_Renderer *pRenderer)
*  \brief  Function to initialize a Message structure
*
*  \param  szMessageName the name of the message
*  \param  pRenderer     a pointer to the renderer
*  \return the initialize structure
*/
Message *Message_Init(char *szMessageName, SDL_Renderer *pRenderer)
{
	Message *pMessage = NULL; 
	Uint32 iNameLen = strlen(szMessageName);
	SDL_Color    color = { 0, 0, 70 };

	pMessage = (Message *)malloc(sizeof(Message));

	pMessage->szMessageName = UTIL_CopyStr(szMessageName, iNameLen);
	pMessage->pBackgroundTexture = NULL;
	pMessage->pRenderer = pRenderer;
	pMessage->pTextTexture = NULL;
	pMessage->color = color;
	pMessage->pFont = NULL;
	pMessage->iStayTime = 50;
	pMessage->iTime = 0;
	pMessage->bMustShow = FALSE;

	pMessage->rMessagePosition.x = 100;
	pMessage->rMessagePosition.y = 100;
	pMessage->rMessagePosition.h = 100;
	pMessage->rMessagePosition.w = 100;
	pMessage->rTextPosition.x = 50;
	pMessage->rTextPosition.y = 50;
	pMessage->rTextPosition.h = 50;
	pMessage->rTextPosition.w = 50;

	return pMessage;
}


/*!
*  \fn   Boolean  Message_Load(Message *pMessage, char *szNameBackground, Uint32 iStayTime, SDL_Color color, TTF_Font *pFont)
*  \brief  Function to free a Message structure
*
*  \param  pMessage				a pointer to the structure to free
*  \param  szNameBackground		name of the background texture file
*  \param  iStayTime			Number of seconds during which the message will be shown
*  \param  color				The color of the text of the message
*  \param  pFont				The Font of the text of the message
*  \return TRUE if everything is ok, FALSE otherwise
*/
Boolean  Message_Load(Message *pMessage, char *szNameBackground, Uint32 iStayTime, SDL_Color color, TTF_Font *pFont)
{
	char szPath[60] = "";

	pMessage->pBackgroundTexture = NULL;
	pMessage->color = color;
	pMessage->pFont = pFont;
	pMessage->iStayTime = iStayTime;

	sprintf(szPath, "sprites/%s.png", szNameBackground);
	pMessage->pBackgroundTexture = UTIL_LoadTexture(pMessage->pRenderer, szPath, NULL, &(pMessage->rMessagePosition));
	if (pMessage->pBackgroundTexture == NULL) return FALSE;
	pMessage->rMessagePosition.x = (KR_WIDTH_WINDOW - pMessage->rMessagePosition.w) /2;
	pMessage->rMessagePosition.y = KR_HEIGHT_WINDOW - pMessage->rMessagePosition.h;
	pMessage->rMessagePosition.h = pMessage->rMessagePosition.h;
	pMessage->rMessagePosition.w = pMessage->rMessagePosition.w;
	return TRUE;
}

/*!
*  \fn     void Message_Free(Message *pMessage)
*  \brief  Function to free a Message structure
*
*  \param  pMessage a pointer to the structure to free
*  \return none
*/
void Message_Free(Message *pMessage)
{
	Kr_Text_CloseFont(&pMessage->pFont);
	UTIL_FreeTexture(&pMessage->pBackgroundTexture);
	UTIL_FreeTexture(&pMessage->pTextTexture);
	UTIL_Free(pMessage);
}



/*!
*  \fn     Boolean Message_Update(Message *pMessage, Boolean bMustShow, char *szMessageText)
*  \brief  Function to update a Message structure
*
*  \param  pMessage				a pointer to the structure message
*  \param  szMessageText		the text of the message
*  \param  bMustShow			Must we print the message ?
*  \return TRUE if everything is ok, FALSE otherwise
*/
Boolean Message_Update(Message *pMessage, Boolean bMustShow, char *szMessageText) // Met à jour la texture à afficher à partir du char* en entrée
{
	pMessage->bMustShow = bMustShow;
	UTIL_FreeTexture(&pMessage->pTextTexture);

	pMessage->pTextTexture = Kr_Text_FontCreateTexture(pMessage->pRenderer, pMessage->pFont, szMessageText, pMessage->color, TRUE, &pMessage->rTextPosition);
	if (pMessage->pTextTexture == NULL)
	{
		Kr_Log_Print(KR_LOG_WARNING, "Can't update the text of the message structure!\n");
		return FALSE;
	}
	pMessage->rTextPosition.x = (KR_WIDTH_WINDOW - pMessage->rTextPosition.w) / 2;
	pMessage->rTextPosition.y = KR_HEIGHT_WINDOW - pMessage->rMessagePosition.h / 2 - pMessage->rTextPosition.h / 2;
	return TRUE;
}


/*!
*  \fn     void Message_Draw(Message *pMessage)
*  \brief  Function to print the message
*
*  \param  pMessage	a pointer to the structure message
*  \return none
*/
void Message_Draw(Message *pMessage)
{
	if (pMessage->bMustShow == FALSE) return;
	if (pMessage->iStayTime * KR_FPS < pMessage->iTime) // On ajuste le nombre de tour de boucle avec les FPS
	{
		pMessage->bMustShow = FALSE;
		pMessage->iTime = 0;
	}
	SDL_RenderCopy(pMessage->pRenderer, pMessage->pBackgroundTexture, NULL, &pMessage->rMessagePosition);
	SDL_RenderCopy(pMessage->pRenderer, pMessage->pTextTexture, NULL, &pMessage->rTextPosition);
	pMessage->iTime++;
}


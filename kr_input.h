/* ========================================================================= */
/*!
* \file    kr_input.h
* \brief   Contains the structure to handle the input (keyboard and mouse).
* \author  Olivier Herrou
* \version 1.0
* \date    1 Mars 2015
*/
/* ========================================================================= */
/* Developers    | Date       | Comments                                     */
/* --------------+------------+--------------------------------------------- */
/* Herrou        | 01/03/2015 | Creation.                                    */
/* Herrou        | 04/04/2015 | Correction typo                              */
/*               |            |                                              */
/*               |            |                                              */
/*               |            |                                              */
/* ========================================================================= */

#ifndef __KR_INPUT_H__
#define __KR_INPUT_H__

	#include "kr_common.h"

	/*!
	* \struct Kr_Input
	* \brief  Structure to handle the input.
	*/
	typedef struct
	{
		char    szKey[SDL_NUM_SCANCODES];  /*!< array to indication the status of each key of the keyboard*/
		Uint32  iMouseX;				   /*!< Integer to indicate the absolute position of the mouse on X. */
		Uint32  iMouseY;				   /*!< Integer to indicate the absolute position of the mouse on Y. */
		Uint32  iMouseXRel; 			   /*!< Integer to indicate the relative position of the mouse on X. */
		Uint32  iMouseYRel;				   /*!< Integer to indicate the relative position of the mouse on Y. */
		char    szMouseButtons[8];         /*!< Array to indicate the status of each buttons of the mouse */
		Boolean bQuit;                     /*!< Boolean to indicate if we quit. */
	} Kr_Input; 

	
	void   InitEvents(Kr_Input* inEvent); 
	void   UpdateEvents(Kr_Input* inEvent ); 

#endif /* __KR_INPUT_H__ */

/* ========================================================================= */














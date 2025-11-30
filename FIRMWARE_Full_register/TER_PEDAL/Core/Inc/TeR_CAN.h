/*
 * TeR_CAN.h
 *
 *  Created on: Feb 2, 2024
 *      Author: Ozuba
 *
 * ████████╗███████╗██████╗          ██████╗ █████╗ ███╗   ██╗
 * ╚══██╔══╝██╔════╝██╔══██╗        ██╔════╝██╔══██╗████╗  ██║
 *    ██║   █████╗  ██████╔╝        ██║     ███████║██╔██╗ ██║
 *    ██║   ██╔══╝  ██╔══██╗        ██║     ██╔══██║██║╚██╗██║
 *    ██║   ███████╗██║  ██║███████╗╚██████╗██║  ██║██║ ╚████║
 *    ╚═╝   ╚══════╝╚═╝  ╚═╝╚══════╝ ╚═════╝╚═╝  ╚═╝╚═╝  ╚═══╝
 */


/* This file is intended to store the decoding and sending functions for all messages on a board. It includes libraries that are

automatically generated using cantools and provides a microcontroller interface with two

* functions:

* - initCAN -> Starts the CAN bus by assigning it to a timer for the transmission frequency

* - decodeMSG -> Decodes the relevant structures

* - sendCAN -> Sends the relevant messages (This will not depend on the state, since the inverters will always be at 0)

* - cmd() -> Function called when the command message is received so that each board interprets it correctly

*

* All the CAN bus memory structures that allow its use outside of the CAN bus are also created here.

*/




#ifndef INC_TER_CAN_H_
#define INC_TER_CAN_H_

#include "ter.h"
#include "stm32f1xx.h"
#include "pedal.h"
#include "can.h"

//TER.dbc
struct TeR_t{
	//Propias
	struct ter_apps_t apps; //Aceleradores
	struct ter_bpps_t bpps; //Freno
	struct ter_steer_t steer; //Volante
};
extern struct TeR_t TeR; //Expone los datos del TeR a otros archivos
/* ---------------------------------------------------------------------- */



uint8_t initCAN(void);//Initializes CAN
uint8_t decodeMsg(uint32_t canId, uint8_t *data); //Decodes message according to DBC
uint8_t sendCAN(void);
uint8_t command(uint8_t cmd); //

#endif /* INC_TER_CAN_H_ */
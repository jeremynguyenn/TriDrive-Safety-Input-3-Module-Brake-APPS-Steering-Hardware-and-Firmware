/*
 * TeR_CAN.c
 *
 *      Author: Nguyen Nhan
 *
 * ████████╗███████╗██████╗          ██████╗ █████╗ ███╗   ██╗
 * ╚══██╔══╝██╔════╝██╔══██╗        ██╔════╝██╔══██╗████╗  ██║
 *    ██║   █████╗  ██████╔╝        ██║     ███████║██╔██╗ ██║
 *    ██║   ██╔══╝  ██╔══██╗        ██║     ██╔══██║██║╚██╗██║
 *    ██║   ███████╗██║  ██║███████╗╚██████╗██║  ██║██║ ╚████║
 *    ╚═╝   ╚══════╝╚═╝  ╚═╝╚══════╝ ╚═════╝╚═╝  ╚═╝╚═╝  ╚═══╝
 */

/*

* This file is intended to store the decoding and sending functions for all messages on a board. It includes libraries that are

automatically generated using cantools and provides a microcontroller interface with two

* functions:

* - initCAN -> Starts the CAN bus by assigning it to a timer for the transmission frequency

* - decodeMSG -> Decodes the relevant structures

* - sendCAN -> Sends the relevant messages

* - command -> Function called when a command message is received so that each board interprets it correctly

* All the CAN bus memory structures are also created here

*/

#include "TeR_CAN.h"  // Include TeR_CAN header
#include "stm32f1xx.h"  // Include CMSIS header

/* ---------------------------[Estructuras del CAN]-------------------------- */
//Pointer to timer and can peripheral being used
CAN_TypeDef *can = CAN1;  // Pointer to CAN1 peripheral
TIM_TypeDef *tim = TIM2;  // Pointer to TIM2 peripheral

//Datos transmision
CAN_TxHeaderTypeDef TxHeader; //Header de transmisión  // Transmission header structure
uint8_t TxData[8]; //Header de recepción  // Transmission data buffer
uint32_t TxMailbox; //Mailbox para el CAN1  // Transmission mailbox

//Datos recepcion
CAN_RxHeaderTypeDef RxHeader;  // Reception header structure
uint8_t RxData[8];  // Reception data buffer

//Variable msgIndex para la cola de envio
uint8_t msgIndex = 0; //Hasta 255 mensajes  // Message index for sending queue

/* -------------------------------------------------------------------------- */

struct TeR_t TeR;  // TeR data structure

uint8_t initCAN(void) {
	//Inicializacion del periferico CAN
	CAN1->IER |= CAN_IER_FMPIE0;  // Enable FIFO0 message pending interrupt
	TIM2->DIER |= TIM_IT_UPDATE;  // Enable TIM2 update interrupt
	TIM2->CR1 |= TIM_CR1_CEN;  // Enable TIM2 counter
	return 1;  // Return success
}

//Envío temporizado
void TIM2_IRQHandler(void) {  // TIM2 interrupt handler
  if (TIM2->SR & TIM_SR_UIF) {  // If update interrupt flag set
    TIM2->SR &= ~TIM_SR_UIF;  // Clear update interrupt flag
    readSensors(); //Lee los sensores  // Call read sensors
    sendCAN();  // Call send CAN
  }
}

//Recepción asincrona
void USB_LP_CAN1_RX0_IRQHandler(void) {  // CAN RX interrupt handler
  if (CAN1->RF0R & CAN_RF0R_FMP0) {  // If FIFO0 has messages pending
    // Read message
    RxHeader.StdId = CAN1->sFIFOMailBox[0].RIR >> 21;  // Extract standard ID
    RxHeader.DLC = (CAN1->sFIFOMailBox[0].RDTR & CAN_RDTR_DLC);  // Extract data length code
    RxData[0] = (CAN1->sFIFOMailBox[0].RDLR) & 0xFF;  // Extract byte 0
    RxData[1] = (CAN1->sFIFOMailBox[0].RDLR >> 8) & 0xFF;  // Extract byte 1
    RxData[2] = (CAN1->sFIFOMailBox[0].RDLR >> 16) & 0xFF;  // Extract byte 2
    RxData[3] = (CAN1->sFIFOMailBox[0].RDLR >> 24) & 0xFF;  // Extract byte 3
    RxData[4] = (CAN1->sFIFOMailBox[0].RDHR) & 0xFF;  // Extract byte 4
    RxData[5] = (CAN1->sFIFOMailBox[0].RDHR >> 8) & 0xFF;  // Extract byte 5
    RxData[6] = (CAN1->sFIFOMailBox[0].RDHR >> 16) & 0xFF;  // Extract byte 6
    RxData[7] = (CAN1->sFIFOMailBox[0].RDHR >> 24) & 0xFF;  // Extract byte 7
    CAN1->RF0R |= CAN_RF0R_RFOM0;  // Release FIFO0 message

    decodeMsg(RxHeader.StdId, RxData); //llama a la decodificación  // Call decode message
  }
}
/* -------------------------------------------------------------------------- */

//Función de decodificación del CAN, si quieres que la ecu disponga de una señal hay que añadirla aquí.
uint8_t decodeMsg(uint32_t canId, uint8_t *data) {  // Function to decode CAN message

	switch (canId) {  // Switch on CAN ID
	//Attend the command
	case TER_COMMAND_FRAME_ID:  // If command frame ID
		command(data[0]); //Llama a la interpretación del comando  // Call command function with first data byte
		break;

	default:  // Default case
		return -1;  // Return error
		break;

	}
	return 1;  // Return success
}

//Función de envío de mensajes
uint8_t sendCAN(void) {  // Function to send CAN message
	TxHeader.IDE = CAN_ID_STD;  // Set ID type to standard
	TxHeader.RTR = CAN_RTR_DATA;  // Set request type to data
	if ((CAN1->TSR & CAN_TSR_TME0) || (CAN1->TSR & CAN_TSR_TME1) || (CAN1->TSR & CAN_TSR_TME2)) {  // If any mailbox is empty
		switch (msgIndex++) {  // Switch on message index and increment

		case 0: //APPS  // Case for APPS message
			TxHeader.StdId = TER_APPS_FRAME_ID;  // Set standard ID for APPS
			TxHeader.DLC = TER_APPS_LENGTH;  // Set data length for APPS
			ter_apps_pack(TxData, &TeR.apps, sizeof(TxData)); //Empaquetamos  // Pack APPS data
			break;
		case 1: //BPPS  // Case for BPPS message
			TxHeader.StdId = TER_BPPS_FRAME_ID;  // Set standard ID for BPPS
			TxHeader.DLC = TER_BPPS_LENGTH;  // Set data length for BPPS
			ter_bpps_pack(TxData, &TeR.bpps, sizeof(TxData)); //Empaquetamos  // Pack BPPS data
			break;

		case 2: //STEER  // Case for STEER message
			TxHeader.StdId = TER_STEER_FRAME_ID;  // Set standard ID for STEER
			TxHeader.DLC = TER_STEER_LENGTH;  // Set data length for STEER
			ter_steer_pack(TxData, &TeR.steer, sizeof(TxData)); //Empaquetamos  // Pack STEER data
			break;

		default: //Esto evita tener que contar mensajes  // Default case
			msgIndex = 0; //cualquier otro valor retorna al ultimo mensaje  // Reset message index
			return 1; //Evita que se envíe un mensaje doble terminando la funcion  // Return to avoid duplicate send
			break;
		}
		uint32_t mailbox = 0;  // Choose mailbox 0 by default
		if (!(CAN1->TSR & CAN_TSR_TME0)) mailbox = 1;  // If mailbox 0 not empty, use 1
		else if (!(CAN1->TSR & CAN_TSR_TME1)) mailbox = 2;  // If mailbox 1 not empty, use 2
		CAN1->sTxMailBox[mailbox].TIR = (TxHeader.StdId << 21) | (TxHeader.IDE << 2) | (TxHeader.RTR << 1);  // Set transmit ID register
		CAN1->sTxMailBox[mailbox].TDTR = TxHeader.DLC;  // Set data length register
		// Copy TxData to registers
		CAN1->sTxMailBox[mailbox].TDLR = TxData[0] | (TxData[1] << 8) | (TxData[2] << 16) | (TxData[3] << 24);  // Set low data register
		CAN1->sTxMailBox[mailbox].TDHR = TxData[4] | (TxData[5] << 8) | (TxData[6] << 16) | (TxData[7] << 24);  // Set high data register
		CAN1->sTxMailBox[mailbox].TIR |= 1;  // Request transmission
		
		if (!(CAN1->TSR & (CAN_TSR_RQCP0 | CAN_TSR_RQCP1 | CAN_TSR_RQCP2))) {  // If no request complete
			GPIOA->BSRR = (1 << 3); //Indicate Error with light (giả sử PIN_3)  // Set error LED
		}
	}
	return 1;  // Return success
}

//Implementa aqui los comandos que se han de ejecutar
uint8_t command(uint8_t cmd) {  // Function to handle commands
	switch (cmd) {  // Switch on command value
	case TER_COMMAND_CMD_CALIBRATE_APPS_MIN_CHOICE: //Calibrate ACC 0% Pos and Store  // Case for min APPS calibration
		offset.low[2] = adcReadings[2]; //Recoje el valor actual  // Set low offset for APPS2
		offset.low[1] = adcReadings[1];  // Set low offset for APPS1
		ee_writeToRam(0, sizeof(offset), (uint8_t*) &offset);  // Write to RAM
		ee_commit();  // Commit to flash
		break;

	case TER_COMMAND_CMD_CALIBRATE_APPS_MAX_CHOICE: //Calibrate ACC 100% Pos and Store  // Case for max APPS calibration
		offset.high[2] = adcReadings[2]; //Recoje el valor actual  // Set high offset for APPS2
		offset.high[1] = adcReadings[1];  // Set high offset for APPS1
		ee_writeToRam(0, sizeof(offset), (uint8_t*) &offset); //Almacena  // Write to RAM
		break;

	case TER_COMMAND_CMD_CALIBRATE_STEER_RIGHTEST_CHOICE: //Calibrate Rightest Steer Position  // Case for right steer calibration
		offset.low[0] = adcReadings[0]; //Recoje el valor actual  // Set low offset for steer
		ee_writeToRam(0, sizeof(offset), (uint8_t*) &offset); //Almacena  // Write to RAM
		break;

	case TER_COMMAND_CMD_CALIBRATE_STEER_LEFTEST_CHOICE: //Calibrate Leftest Steer Position  // Case for left steer calibration
		offset.high[0] = adcReadings[0]; //Recoje el valor actual  // Set high offset for steer
		ee_writeToRam(0, sizeof(offset), (uint8_t*) &offset); //Almacena  // Write to RAM
		break;

	default:  // Default case
		return 0;  // Return failure
		break;

	}
	ee_commit(); //Almacena en la flash la calibración  // Commit changes to flash
	return 1;  // Return success
}
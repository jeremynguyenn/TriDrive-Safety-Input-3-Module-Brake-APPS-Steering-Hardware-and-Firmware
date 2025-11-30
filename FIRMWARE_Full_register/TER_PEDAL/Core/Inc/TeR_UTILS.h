/*
 * TeR_UTILS.h
 *
 *      Author: Nguyen Nhan
 */
//This file holds diverse code utils that dont fall under any category
#ifndef INC_TER_UTILS_H_
#define INC_TER_UTILS_H_

//Voltage to ADC value
#define VOLT2ADC(V,ADCREF) ((V) * 4095.0)/ (ADCREF)



// Boolean Persistance checker 

typedef uint32_t persist_t; 

uint8_t checkPersistance(persist_t *instance, uint8_t ok, uint32_t tMax);

//MAP (Mape)
int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min,
		int32_t out_max); 
#endif /* INC_TER_UTILS_H_ */
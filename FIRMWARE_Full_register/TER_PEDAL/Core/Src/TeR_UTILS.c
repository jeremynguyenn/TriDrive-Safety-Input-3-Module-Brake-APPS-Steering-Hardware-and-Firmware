/*
 * TeR_UTILS.c
 *
 *      Author: Nguyen Nhan
 */
#include "TeR_UTILS.h"  // Include utilities header
#include "stdbool.h"  // Include boolean type
#include "stm32f1xx.h"  // Include CMSIS header for SysTick

// Thay HAL_GetTick bằng bare-metal (giả sử dùng SysTick counter)
volatile uint32_t tick = 0;  // Global volatile tick counter
void SysTick_Handler(void) {  // SysTick interrupt handler
  tick++;  // Increment tick counter
}
uint32_t GetTick(void) {  // Function to get current tick
  return tick;  // Return tick value
}

//Comprueba que un error sucede durante más de tMax (La función prefiere mantener el estado OK)
uint8_t checkPersistance(persist_t *instance, uint8_t ok, uint32_t tMax) {  // Function to check persistence of condition

	if (*instance > 0) { //Estabamos en error  // If instance timestamp > 0 (was in error)
		if (ok) { //No tenemos error  // If condition is OK now
			*instance = 0; //Ponemos el timestamp a 0, ya no hay error  // Reset timestamp
		} else if (GetTick() - *instance >= tMax) { //El error supera maxtime  // If time exceeded max
			return 0; //Damos el error  // Return error (0)
		}
	} else if (!ok) { // no estabamos en error y ahora si  // If was OK but now not
		*instance = GetTick();  // Set timestamp to current tick
	}

	return 1; //Tenemos Error pero no hemos superado maxTime  // Return no error yet (1)
}


// Mapea un intervalo
int32_t map(int32_t x, int32_t in_min, int32_t in_max, int32_t out_min,
		int32_t out_max) {  // Function to map value from one range to another
//Saturar las salidas si la entrada excede el límite de calibracion
	if (x < in_min)  // If input below min
		return out_min;  // Return output min
	if (x > in_max)  // If input above max
		return out_max;  // Return output max
//Mapear si estamos en rango seguro
	long val = (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;  // Calculate mapped value
	return val;  // Return mapped value
}
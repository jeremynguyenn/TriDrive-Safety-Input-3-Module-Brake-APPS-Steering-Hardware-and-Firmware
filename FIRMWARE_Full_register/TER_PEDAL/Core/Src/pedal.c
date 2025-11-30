/*
 * pedal.c
 *
 *      Author: Nguyen Nhan
 */
#include "pedal.h"  // Include pedal header
#include "stm32f1xx.h"  // Include CMSIS header

//ADC usado (bây giờ không cần Handle, dùng trực tiếp registers)
ADC_TypeDef *adc = ADC1;  // Pointer to ADC1 peripheral registers

//Implausabilities
persist_t DELTA_IMP;  // Persistence variable for delta implausibility
persist_t RANGE_IMP;  // Persistence variable for range implausibility
uint8_t impRange; //1 if signals are out of range  // Flag for range implausibility
uint8_t impDelta; //1 if APPS difference exceeds 10% of total range  // Flag for delta implausibility

//Offsets de los sensores {Steer,APPS1,APPS2,Brake}
struct offsets_t offset;  // Structure for sensor offsets

//Estructura de lectura para el ADC
int32_t adcReadings[4]; //32*3, el adc saca 12 bits alineados a la derecha usamos enteros con signo pa que no nos la lien las restas al comprobar offsets  // Array for ADC readings (signed 32-bit)

// Function to initialize pedal module
void initPedal(void) {
	ee_init(); //Inicializamos la flash (EEPROM virtual)  // Initialize EEPROM emulation

	//Carga de los offsets
	ee_read(0, sizeof(offset), (uint8_t*) &offset); //Lee de memoria el struct  // Read offsets from EEPROM

	//Check if there are offsets written in flash
	if (!offset.written) { // En un futuro lo ideal sería ver que los valores están en rangos lógicos  // If offsets not written
		offset.high[0] = 4096; //Valores por defecto  // Set default high offset for steer
		offset.high[1] = 4096;  // Set default high offset for APPS1
		offset.high[2] = 4096;  // Set default high offset for APPS2
		offset.low[0] = 0;  // Set default low offset for steer
		offset.low[1] = 0;  // Set default low offset for APPS1
		offset.low[2] = 0;  // Set default low offset for APPS2
		offset.written = 1; // Establece un byte en memoria que indica que la placa ha sido programada  // Set written flag
		ee_writeToRam(0, sizeof(offset), (uint8_t*) &offset);  // Write offsets to RAM
		ee_commit();  // Commit to flash
	}

	//Inicializamos el DMA para que copie nuestros datos al buffer de lecturas
	//Hemos desactivado las interrupciones del mismo en el NVIC para que no obstruyan, solo nos interesa que anden disponibles
	ADC1->CR2 |= ADC_CR2_ADON;  // Enable ADC
	ADC1->CR2 |= ADC_CR2_SWSTART;  // Start ADC conversion
}

// Function to read sensors
void readSensors() {

	//Se leen y convierten las señales
	TeR.bpps.bpps = map(adcReadings[3], MINBRAKE, MAXBRAKE, 0, 50); //Estamos con VREF de 5 porque hay un divisor  // Map brake reading
	TeR.apps.apps_2 = map(adcReadings[2], offset.low[2], offset.high[2], 0,
			255); //Lectura de APPS1  // Map APPS2 reading
	TeR.apps.apps_1 = map(adcReadings[1], offset.low[1], offset.high[1], 0,
			255); //Lectura del APPS2  // Map APPS1 reading
	TeR.steer.angle = map(adcReadings[0], offset.low[0], offset.high[0],
	-MAXWHEELANGLE, MAXWHEELANGLE); //Lectura ANGULO de giro (Poner factor)  // Map steer angle

	//Check for implausability T 11.8.9 Desviacion de 10 puntos en %
	impDelta = !checkPersistance(&RANGE_IMP,(abs(TeR.apps.apps_1 - TeR.apps.apps_2) < (255 * 0.1)),100);//Comprueba que la diferencia entre aceleradores es menor que el 10% activamente, solo falla si esta se da por más de 100ms  // Check delta implausibility with persistence
	//Check if all signals are in range
	impRange = !checkPersistance(&DELTA_IMP,((adcReadings[1] > (offset.low[1] -MARGIN)) && (adcReadings[2] > (offset.low[2]-MARGIN)) && (adcReadings[3] > (MINBRAKE-MARGIN)) && (adcReadings[1] < (offset.high[1] + MARGIN)) && (adcReadings[2] < (offset.high[2] + MARGIN)) && (adcReadings[3] < (MAXBRAKE + MARGIN))),500); //Implausible range 500 millis  // Check range implausibility with persistence
	TeR.apps.imp_flag = (impDelta || impRange); //Determine existing implausability  // Set implausibility flag
	//Computa la media
	TeR.apps.apps_av = TeR.apps.imp_flag ? 0 :(TeR.apps.apps_2 + TeR.apps.apps_1) / 2;  // Compute average if no implausibility

	GPIOA->BSRR = (TeR.apps.imp_flag << 3); //Actualizamos el estado del led (giả sử IMP_LED_Pin là GPIO_PIN_3)  // Update LED state (set if flag high)
}

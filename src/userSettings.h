/*
 * userSettings.h
 *
 *  Created on: 27.12.2016
 *      Author: cwiec
 */

#ifndef USERSETTINGS_H_
#define USERSETTINGS_H_
#include "Core/TaskMenager.h"
#include "Display/Display.h"
#include "Sensors/HIH6030.h"
#include "Sensors/MiCS6814.h"
#define USART1_TX 	GPIO_Pin_9
#define USART1_RX 	GPIO_Pin_10

/*Pins Config 	[ SPI1 ]	[ SPI2 ]
 * CLK 			PA5			PB13
 * MOSI			PA7			PB15
 * CS			PA4			PB12
 * MISO			-			PB14
 * D_C			PA6			-
 * RES			PC4			-
 * */

#define SPI1_CLK_PIN		GPIO_Pin_5
#define SPI1_CS_PIN  		GPIO_Pin_4
#define SPI1_MOSI_PIN	 	GPIO_Pin_7
#define SPI1_D_C_PIN 		GPIO_Pin_6
#define SPI1_RES_PIN 		GPIO_Pin_4

#define SPI2_CLK_PIN		GPIO_Pin_13
#define SPI2_CS_PIN  		GPIO_Pin_12
#define SPI2_MOSI_PIN	 	GPIO_Pin_15
#define SPI2_MISO_PIN 		GPIO_Pin_14

#define ADC_CHANNELS 4
extern const uint16_t colorScale[32];
extern Display* ds;
extern HIH6030* humSens;
extern MiCS_6814* gasSens;
extern void (*wsk2)();

void init();
void initUsart();
void initSPI();
void initADC();
void initButtons();
void initTimer();

#endif /* USERSETTINGS_H_ */

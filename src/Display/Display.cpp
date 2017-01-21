/*
 * Display.cpp
 *
 *  Created on: 20.01.2017
 *      Author:
 */

#include <Display/Display.h>
#include "userSettings.h"
#include <stdio.h>


Display::Display() : sp(&SPI1_class::getInstance())
{
	/*setting of chip select for display*/
	CSsetS csSetting;
	csSetting.csPin = SPI1_CS_PIN;
	csSetting.gpioType = GPIOA;
	sp->setCS(csSetting);

	GPIO_ResetBits(GPIOC, SPI1_RES_PIN);
	GPIO_SetBits(GPIOC, SPI1_RES_PIN);

	sp->storeCommand(CMD_DISPLAY_OFF);          //Display Off
	sp->storeCommand(CMD_SET_CONTRAST_A);       //Set contrast for color A
	sp->storeCommand(0xFF);                     //255
	sp->storeCommand(CMD_SET_CONTRAST_B);       //Set contrast for color B
	sp->storeCommand(0xFF );                     //255
	sp->storeCommand(CMD_SET_CONTRAST_C);       //Set contrast for color C
	sp->storeCommand(0xFF);                     //255
	sp->storeCommand(CMD_MASTER_CURRENT_CONTROL);//master current control
	sp->storeCommand(0x06);                     //6
	sp->storeCommand(CMD_SET_PRECHARGE_SPEED_A);//Set Second Pre-change Speed For ColorA
	sp->storeCommand(0x64);                     //100
	sp->storeCommand(CMD_SET_PRECHARGE_SPEED_B);//Set Second Pre-change Speed For ColorB
	sp->storeCommand(0x78);                     //120
	sp->storeCommand(CMD_SET_PRECHARGE_SPEED_C);//Set Second Pre-change Speed For ColorC
	sp->storeCommand(0x64);                     //100
	sp->storeCommand(CMD_SET_REMAP);            //set remap & data format
	sp->storeCommand(0x72);                     //0x72
	sp->storeCommand(CMD_SET_DISPLAY_START_LINE);//Set display Start Line
	sp->storeCommand(0x0);
	sp->storeCommand(CMD_SET_DISPLAY_OFFSET);   //Set display offset
	sp->storeCommand(0x0);
	sp->storeCommand(CMD_NORMAL_DISPLAY);       //Set display mode
	sp->storeCommand(CMD_SET_MULTIPLEX_RATIO);  //Set multiplex ratio
	sp->storeCommand(0x3F);
	sp->storeCommand(CMD_SET_MASTER_CONFIGURE); //Set master configuration
	sp->storeCommand(0x8E);
	sp->storeCommand(CMD_POWER_SAVE_MODE);      //Set Power Save Mode
	sp->storeCommand(0x00);                     //0x00
	sp->storeCommand(CMD_PHASE_PERIOD_ADJUSTMENT);//phase 1 and 2 period adjustment
	sp->storeCommand(0x31);                     //0x31
	sp->storeCommand(CMD_DISPLAY_CLOCK_DIV);    //display clock divider/oscillator frequency
	sp->storeCommand(0xF0);
	sp->storeCommand(CMD_SET_PRECHARGE_VOLTAGE);//Set Pre-Change Level
	sp->storeCommand(0x3A);
	sp->storeCommand(CMD_SET_V_VOLTAGE);        //Set vcomH
	sp->storeCommand(0x3E);
	sp->storeCommand(CMD_DEACTIVE_SCROLLING);   //disable scrolling
	sp->storeCommand(CMD_NORMAL_BRIGHTNESS_DISPLAY_ON);//set display on

	sp->send();
}

Display::~Display()
{

}

void Display::init()
{

}

void Display::clear()
{
	sp->storeCommand(CMD_CLEAR_WINDOW);
	sp->storeCommand(0);
	sp->storeCommand(0);
	sp->storeCommand(95);
	sp->storeCommand(63);

	sp->send();
}

void Display::setBackground(uint16_t color)
{
	sp->storeCommand(CMD_FILL_WINDOW);//fill window
	sp->storeCommand(ENABLE_FILL);
	sp->storeCommand(CMD_DRAW_RECTANGLE);//draw rectangle
	sp->storeCommand(0);//start column
	sp->storeCommand(0);//start row
	sp->storeCommand(95);//end column
	sp->storeCommand(63);//end row

	//frame
	sp->storeCommand((color >> 9) & 0x3F);//R
	sp->storeCommand((color >> 4) & 0x3F);//G
	sp->storeCommand((color << 2) & 0x3F);//B

	//fill
	sp->storeCommand((color >> 9) & 0x3F);//R
	sp->storeCommand((color >> 4) & 0x3F);//G
	sp->storeCommand((color << 2) & 0x3F);//B
	sp->send();
}

void Display::drawPixel(uint16_t x, uint16_t y, uint16_t color)
{
	if ((x < 0) || (x >= RGB_OLED_WIDTH) || (y < 0) || (y >= RGB_OLED_HEIGHT))
	return;

	//set column point
	sp->storeCommand(CMD_SET_COLUMN_ADDRESS);
	sp->storeCommand(x);
	sp->storeCommand(RGB_OLED_WIDTH-1);
	//set row point
	sp->storeCommand(CMD_SET_ROW_ADDRESS);
	sp->storeCommand(y);
	sp->storeCommand(RGB_OLED_HEIGHT-1);

	//fill 16bit colour
	sp->storeCommand((color >> 8), DATA);
	sp->storeCommand((color & 0xFF), DATA);

	sp->send();
}

void Display::drawLine(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t color)
{
	if((x0 < 0) || (y0 < 0) || (x1 < 0) || (y1 < 0))
		return;

	if (x0 >= RGB_OLED_WIDTH)  x0 = RGB_OLED_WIDTH - 1;
	if (y0 >= RGB_OLED_HEIGHT) y0 = RGB_OLED_HEIGHT - 1;
	if (x1 >= RGB_OLED_WIDTH)  x1 = RGB_OLED_WIDTH - 1;
	if (y1 >= RGB_OLED_HEIGHT) y1 = RGB_OLED_HEIGHT - 1;

	sp->storeCommand(CMD_DRAW_LINE);//draw line
	sp->storeCommand(x0);//start column
	sp->storeCommand(y0);//start row
	sp->storeCommand(x1);//end column
	sp->storeCommand(y1);//end row
	sp->storeCommand((color >> 9) & 0x3F);//R
	sp->storeCommand((color >> 4) & 0x3F);//G
	sp->storeCommand((color << 2) & 0x3F);//B

	sp->send();
}

void Display::drawFrame(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, uint16_t outColor, uint16_t fillColor)
{
	if((x0 < 0) || (y0 < 0) || (x1 < 0) || (y1 < 0))
		return;

	if (x0 >= RGB_OLED_WIDTH)  x0 = RGB_OLED_WIDTH - 1;
	if (y0 >= RGB_OLED_HEIGHT) y0 = RGB_OLED_HEIGHT - 1;
	if (x1 >= RGB_OLED_WIDTH)  x1 = RGB_OLED_WIDTH - 1;
	if (y1 >= RGB_OLED_HEIGHT) y1 = RGB_OLED_HEIGHT - 1;

	sp->storeCommand(CMD_FILL_WINDOW);//fill window
	sp->storeCommand(ENABLE_FILL);
	sp->storeCommand(CMD_DRAW_RECTANGLE);//draw rectangle
	sp->storeCommand(x0);//start column
	sp->storeCommand(y0);//start row
	sp->storeCommand(x1);//end column
	sp->storeCommand(y1);//end row

	sp->storeCommand((outColor >> 9) & 0x3F);//R
	sp->storeCommand((outColor >> 4) & 0x3F);//G
	sp->storeCommand((outColor << 2) & 0x3F);//B

	sp->storeCommand((fillColor >> 9) & 0x3F);//R
	sp->storeCommand((fillColor >> 4) & 0x3F);//G
	sp->storeCommand((fillColor << 2) & 0x3F);//B

	sp->send();
}


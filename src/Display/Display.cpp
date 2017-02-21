/*
 * Display.cpp
 *
 *  Created on: 20.01.2017
 *      Author:
 */

#include <Display/Display.h>
#include "userSettings.h"
#include <stdio.h>

const uint16_t colorScale[32] =
{
		RGB_MACRO( 0, 0, 255)	,RGB_MACRO( 0, 16, 240)	,RGB_MACRO( 0, 32, 224),
		RGB_MACRO( 0, 48, 208)	,RGB_MACRO( 0, 64, 192)	,RGB_MACRO( 0, 80, 176),
		RGB_MACRO( 0, 96, 160)	,RGB_MACRO( 0, 112, 144),RGB_MACRO( 0, 128, 128),
		RGB_MACRO( 0, 144, 112)	,RGB_MACRO( 0, 160, 96)	,RGB_MACRO( 0, 176, 80),
		RGB_MACRO( 0, 192, 64)	,RGB_MACRO( 0, 208, 48)	,RGB_MACRO( 0, 224, 32),
		RGB_MACRO( 0, 240, 16)	,RGB_MACRO( 0, 255, 0)	,RGB_MACRO( 16, 240, 0),
		RGB_MACRO( 32, 224, 0)	,RGB_MACRO( 48, 208, 0)	,RGB_MACRO( 64, 192, 0),
		RGB_MACRO( 80, 176, 0)	,RGB_MACRO( 96, 160, 0)	,RGB_MACRO( 112, 144, 0),
		RGB_MACRO( 128, 128, 0)	,RGB_MACRO( 144, 112, 0),RGB_MACRO( 160, 96, 0),
		RGB_MACRO( 176, 80, 0)	,RGB_MACRO( 192, 64, 0)	,RGB_MACRO( 208, 48, 0),
		RGB_MACRO( 224, 32, 0)	,RGB_MACRO( 255, 0, 0)
};

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
	sp->storeCommand((color >> 10) & 0x3F);//R
	sp->storeCommand((color >> 5) & 0x3F);//G
	sp->storeCommand((color) & 0x3F);//B

	//fill
	sp->storeCommand((color >> 10) & 0x3F);//R
	sp->storeCommand((color >> 5) & 0x3F);//G
	sp->storeCommand((color ) & 0x3F);//B
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
	sp->storeCommand((uint8_t)(color >> 8), DATA);
	sp->storeCommand((uint8_t)(color), DATA);

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

void Display::copyWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1,uint16_t x2, uint16_t y2)
{
    sp->storeCommand(CMD_COPY_WINDOW);//copy window
    sp->storeCommand(x0);//start column
    sp->storeCommand(y0);//start row
    sp->storeCommand(x1);//end column
    sp->storeCommand(y1);//end row
    sp->storeCommand(x2);//new column
    sp->storeCommand(y2);//new row

    sp->send();
}

void Display::dimWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
   sp->storeCommand(CMD_DIM_WINDOW);//copy area
   sp->storeCommand(x0);//start column
   sp->storeCommand(y0);//start row
   sp->storeCommand(x1);//end column
   sp->storeCommand(y1);//end row

   sp->send();
}

void Display::clearWindow(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
   sp->storeCommand(CMD_CLEAR_WINDOW);//clear window
   sp->storeCommand(x0);//start column
   sp->storeCommand(y0);//start row
   sp->storeCommand(x1);//end column
   sp->storeCommand(y1);//end row

   sp->send();
}

void Display::setScolling(ScollingDirection direction, uint8_t rowAddr, uint8_t rowNum, uint8_t timeInterval)
{
    uint8_t scolling_horizontal = 0x0;
    uint8_t scolling_vertical = 0x0;
    switch(direction){
        case Horizontal:
            scolling_horizontal = 0x01;
            scolling_vertical = 0x00;
            break;
        case Vertical:
            scolling_horizontal = 0x00;
            scolling_vertical = 0x01;
            break;
        case Diagonal:
            scolling_horizontal = 0x01;
            scolling_vertical = 0x01;
            break;
        default:
            break;
    }
   sp->storeCommand(CMD_CONTINUOUS_SCROLLING_SETUP);
   sp->storeCommand(scolling_horizontal);
   sp->storeCommand(rowAddr);
   sp->storeCommand(rowNum);
   sp->storeCommand(scolling_vertical);
   sp->storeCommand(timeInterval);
   sp->storeCommand(CMD_ACTIVE_SCROLLING);

   sp->send();
}

void Display::enableScolling(bool enable)
{
    if(enable)
       sp->storeCommand(CMD_ACTIVE_SCROLLING);
    else
       sp->storeCommand(CMD_DEACTIVE_SCROLLING);

    sp->send();
}

void Display::setDisplayMode(DisplayMode mode)
{
   sp->storeCommand(mode);
   sp->send();
}

void Display::setDisplayPower(DisplayPower power)
{
   sp->storeCommand(power);
   sp->send();
}

void Display::draw_bitmap(uint8_t chXpos, uint8_t chYpos, const uint8_t *pchBmp, uint8_t chWidth, uint8_t chHeight, uint16_t hwColor)
{
	uint16_t i, j, byteWidth = (chWidth + 7) / 8;

    for(j = 0; j < chHeight; j ++){
        for(i = 0; i < chWidth; i ++ ) {
            if(*(pchBmp + j * byteWidth + i / 8) & (128 >> (i & 7))) {
                drawPixel(chXpos + i, chYpos + j, hwColor);
            }
        }
    }
}

void Display::draw_3216char(uint8_t chXpos, uint8_t chYpos, uint8_t chChar, uint16_t hwColor)
{
	uint8_t i, j;
	uint8_t chTemp = 0, chYpos0 = chYpos;

	for (i = 0; i < 64; i ++) {
		chTemp = c_chFont3216[chChar - 0x30][i];
		for (j = 0; j < 8; j ++) {
			if (chTemp & 0x80) {
				drawPixel(chXpos, chYpos, hwColor);
    		}
			chTemp <<= 1;
			chYpos ++;
			if ((chYpos - chYpos0) == 32) {
				chYpos = chYpos0;
				chXpos ++;
				break;
			}
		}
	}
}

void Display::draw_1616char(uint8_t chXpos, uint8_t chYpos, uint8_t chChar, uint16_t hwColor)
{
	uint8_t i, j;
	uint8_t chTemp = 0, chYpos0 = chYpos;

	for (i = 0; i < 32; i ++) {
		chTemp = c_chFont1612[chChar - 0x30][i];
		for (j = 0; j < 8; j ++) {
			if (chTemp & 0x80) {
				drawPixel(chXpos, chYpos, hwColor);
    		}
			chTemp <<= 1;
			chYpos ++;
			if ((chYpos - chYpos0) == 16) {
				chYpos = chYpos0;
				chXpos ++;
				break;
			}
		}
	}
}

void Display::display_char(uint8_t chXpos, uint8_t chYpos, char chChr, uint8_t chSize, uint16_t hwColor)
{
	uint8_t i, j, chTemp = 0;
	uint8_t chYpos0 = chYpos;

	if (chXpos >= RGB_OLED_WIDTH || chYpos >= RGB_OLED_HEIGHT) {
		return;
	}

    for (i = 0; i < chSize; i ++) {
		if (FONT_1206 == chSize) {
			chTemp = c_chFont1206[(uint8_t)chChr - 0x20][i];
		} else if (FONT_1608 == chSize) {
			chTemp = c_chFont1608[(uint8_t)chChr - 0x20][i];
		}

        for (j = 0; j < 8; j ++) {
    		if (chTemp & 0x80) {
				drawPixel(chXpos, chYpos, hwColor);
    		}
			chTemp <<= 1;
			chYpos ++;
			if ((chYpos - chYpos0) == chSize) {
				chYpos = chYpos0;
				chXpos ++;
				break;
			}
		}
    }
}

void Display::display_string(uint8_t chXpos, uint8_t chYpos, const char *pchString, uint8_t chSize, uint16_t hwColor)
{
	if (chXpos >= RGB_OLED_WIDTH || chYpos >= RGB_OLED_HEIGHT) {
		return;
	}

    while (*pchString != '\0') {
        if (chXpos > (RGB_OLED_WIDTH - chSize / 2)) {
			chXpos = 0;
			chYpos += chSize;
			if (chYpos > (RGB_OLED_HEIGHT - chSize)) {
				chYpos = chXpos = 0;
			}
		}
        if(*pchString == '\n')
        {
        	pchString++;
			chXpos = 0;
			chYpos += chSize;
			if (chYpos > (RGB_OLED_HEIGHT - chSize)) {
				chYpos = chXpos = 0;
			}
        }

        display_char(chXpos, chYpos, *pchString, chSize, hwColor);
        chXpos += chSize / 2;
        pchString ++;
    }
}

static uint32_t _pow(uint8_t m, uint8_t n)
{
	uint32_t result = 1;

	while(n --) result *= m;
	return result;
}

void Display::display_num(uint8_t chXpos, uint8_t chYpos, uint32_t chNum, uint8_t chLen, uint8_t chSize, uint16_t hwColor)
{
	uint8_t i;
	uint8_t chTemp, chShow = 0;

	if (chXpos >= RGB_OLED_WIDTH || chYpos >= RGB_OLED_HEIGHT) {
		return;
	}

	for(i = 0; i < chLen; i ++) {
		chTemp = (chNum / _pow(10, chLen - i - 1)) % 10;
		if(chShow == 0 && i < (chLen - 1)) {
			if(chTemp == 0) {
				display_char(chXpos + (chSize / 2) * i, chYpos, ' ', chSize, hwColor);
				continue;
			} else {
				chShow = 1;
			}
		}
	 	display_char(chXpos + (chSize / 2) * i, chYpos, chTemp + '0', chSize, hwColor);
	}
}


void Display::draw_circle(uint8_t chXpos, uint8_t chYpos, uint8_t chRadius, uint16_t hwColor)
{
	int x = -chRadius, y = 0, err = 2 - 2 * chRadius, e2;

	if (chXpos >= RGB_OLED_WIDTH || chYpos >= RGB_OLED_HEIGHT) {
		return;
	}

    do {
        drawPixel(chXpos - x, chYpos + y, hwColor);
        drawPixel(chXpos + x, chYpos + y, hwColor);
        drawPixel(chXpos + x, chYpos - y, hwColor);
        drawPixel(chXpos - x, chYpos - y, hwColor);
        e2 = err;
        if (e2 <= y) {
            err += ++ y * 2 + 1;
            if(-x == y && e2 <= x) e2 = 0;
        }
        if(e2 > x) err += ++ x * 2 + 1;
    } while(x <= 0);

}


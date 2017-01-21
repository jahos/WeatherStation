/*
 * Display.h
 *
 *  Created on: 20.01.2017
 *      Author:
 */

#ifndef DISPLAY_DISPLAY_H_
#define DISPLAY_DISPLAY_H_

#include "Core/SPI1class.h"

#define RGB_OLED_WIDTH                      96
#define RGB_OLED_HEIGHT                     64

#define CMD_DRAW_LINE                       0x21
#define CMD_DRAW_RECTANGLE                  0x22
#define CMD_COPY_WINDOW                     0x23
#define CMD_DIM_WINDOW                      0x24
#define CMD_CLEAR_WINDOW                    0x25
#define CMD_FILL_WINDOW                     0x26
    #define DISABLE_FILL    0x00
    #define ENABLE_FILL     0x01
#define CMD_CONTINUOUS_SCROLLING_SETUP      0x27
#define CMD_DEACTIVE_SCROLLING              0x2E
#define CMD_ACTIVE_SCROLLING                0x2F

#define CMD_SET_COLUMN_ADDRESS              0x15
#define CMD_SET_ROW_ADDRESS                 0x75
#define CMD_SET_CONTRAST_A                  0x81
#define CMD_SET_CONTRAST_B                  0x82
#define CMD_SET_CONTRAST_C                  0x83
#define CMD_MASTER_CURRENT_CONTROL          0x87
#define CMD_SET_PRECHARGE_SPEED_A           0x8A
#define CMD_SET_PRECHARGE_SPEED_B           0x8B
#define CMD_SET_PRECHARGE_SPEED_C           0x8C
#define CMD_SET_REMAP                       0xA0
#define CMD_SET_DISPLAY_START_LINE          0xA1
#define CMD_SET_DISPLAY_OFFSET              0xA2
#define CMD_NORMAL_DISPLAY                  0xA4
#define CMD_ENTIRE_DISPLAY_ON               0xA5
#define CMD_ENTIRE_DISPLAY_OFF              0xA6
#define CMD_INVERSE_DISPLAY                 0xA7
#define CMD_SET_MULTIPLEX_RATIO             0xA8
#define CMD_DIM_MODE_SETTING                0xAB
#define CMD_SET_MASTER_CONFIGURE            0xAD
#define CMD_DIM_MODE_DISPLAY_ON             0xAC
#define CMD_DISPLAY_OFF                     0xAE
#define CMD_NORMAL_BRIGHTNESS_DISPLAY_ON    0xAF
#define CMD_POWER_SAVE_MODE                 0xB0
#define CMD_PHASE_PERIOD_ADJUSTMENT         0xB1
#define CMD_DISPLAY_CLOCK_DIV               0xB3
#define CMD_SET_GRAy_SCALE_TABLE            0xB8
#define CMD_ENABLE_LINEAR_GRAY_SCALE_TABLE  0xB9
#define CMD_SET_PRECHARGE_VOLTAGE           0xBB
#define CMD_SET_V_VOLTAGE                   0xBE
#define COLOR_R(X) (((X/255)*63) << 10)
#define COLOR_G(X) (((X/255)*63) << 5 )
#define COLOR_B(X) (((X/255)*63) >> 1 )

#define RGB_MACRO(X,Y,Z) (COLOR_R(X) | COLOR_G(Y) | COLOR_B(Z))

enum Color{
    BLACK     = RGB_MACRO(  0,  0,  0), // black
    GREY      = RGB_MACRO(192,192,192), // grey
    WHITE     = RGB_MACRO(255,255,255), // white
    RED       = RGB_MACRO(255,  0,  0), // red
    PINK      = RGB_MACRO(255,192,203), // pink
    YELLOW    = RGB_MACRO(255,255,  0), // yellow
    GOLDEN    = RGB_MACRO(255,215,  0), // golden
    BROWN     = RGB_MACRO(128, 42, 42), // brown
    BLUE      = RGB_MACRO(  0,  0,255), // blue
    CYAN      = RGB_MACRO(  0,255,255), // cyan
    GREEN     = RGB_MACRO(  0,255,  0), // green
    PURPLE    = RGB_MACRO(160, 32,240), // purple
};

enum DisplayMode{
    //reset the above effect and turn the data to ON at the corresponding gray level.
    DisplayNormal   = 0xA4,
    //forces the entire display to be at "GS63"
    DisplayOn       = 0xA5,
    //forces the entire display to be at gray level "GS0"
    DisplayOff      = 0xA6,
    //swap the gray level of display data
    DisplayInverse  = 0xA7
};

enum DisplayPower{
    DimMode         = 0xAC,
    SleepMode       = 0xAE,
    NormalMode      = 0xAF
};

enum ScollingDirection{
    Horizontal      = 0x00,
    Vertical        = 0x01,
    Diagonal        = 0x02
};


class Display
{
private:
	SPI1_class * sp;
public:
	Display();

	void init();

	void clear();
	void setBackground(uint16_t color);
	void drawPixel(uint16_t x, uint16_t y, uint16_t color);
	virtual ~Display();

private:

};

#endif /* DISPLAY_DISPLAY_H_ */

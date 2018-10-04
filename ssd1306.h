// Ken Hughes
// July 2016


#include <unistd.h>//Needed for I2C port
#include <fcntl.h>//Needed for I2C port
#include <sys/ioctl.h>//Needed for I2C port
#include <linux/i2c-dev.h>//Needed for I2C port
#include <stdio.h>
#include <string.h>

#ifndef BOOL
    #define BOOL
    #define FALSE 0
    #define TRUE -1
#endif


#ifndef SSD1306_H_INCLUDED
#define SSD1306_H_INCLUDED

class SSD1306
{
    public:
        enum Mode
        {
            SCROLL,
            WRAP
        };

        SSD1306();
        void clearDisplay();
        void initDisplay();
        void textDisplay(const char *message);
        void setWordWrap(int);
        void setDisplayMode(SSD1306::Mode);
	void setAltI2C();

    private:
        // line buffers (128 chars * 8 lines)
    	unsigned char displayLines[8][128];
    	int currentLine = 0;
        int wordWrap = FALSE;
        int displayMode = Mode::SCROLL;
        int currentScrollLine = 0;
        int needScroll = FALSE;

        // i2c handles
        int i2cHandle = -1;
        int i2cInitialised = FALSE;
    	int i2cAddress = 0x3C;
	int i2cAlternate = FALSE;

        // ssd1306 command sequences
        unsigned char initSequence[26] = {0x00,0xAE,0xA8,0x3F,0xD3,0x00,0x40,0xA1,0xC8,0xDA,0x12,0x81,0x7F,
                                          0xA4,0xA6,0xD5,0x80,0x8D,0x14,0xD9,0x22,0xD8,0x30,0x20,0x00,0xAF};

    	unsigned char setFullRange[7] = {0x00,0x21,0x00,0x7F,0x22,0x00,0x07};

        unsigned char scrollUpSequence[3] = {0x00,0xD3,0x08};

        // helper functions
        int addFontBytes(int curr, unsigned char c);
        void setDisplayRange(int line);
        void updateDisplayFull();
        void writeI2C(unsigned char* data, int bytes); 
        void scrollUp(int); 
};

#endif

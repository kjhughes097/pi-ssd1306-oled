#include <ctype.h>
#include "charmap.h"
#include "ssd1306.h"

#ifdef DEBUG
 #define D if(1) 
#else
 #define D if(0) 
#endif

    SSD1306::SSD1306() {
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<128; j++)
            {
                displayLines[i][j] = 0;
            }
        }
    }

    void SSD1306::initDisplay() {
        D printf("initDisplay\n");
		writeI2C(initSequence, 26);
        i2cInitialised = TRUE;
    }

    void SSD1306::setAltI2C() {
	D printf("setAltI2C\n");
	i2cAlternate = TRUE;
    }

    void SSD1306::textDisplay(const char *message) {
        if(!i2cInitialised)
        {
            initDisplay();
        }

        // and scroll up if needed (in SCROLL mode)
        if((displayMode == SSD1306::Mode::SCROLL) && needScroll)
        {
            scrollUp(1);
        } 

        D printf("textDisplay - %s\n", message);
        int currByteCount = 0;
        for(int i=0; i<strlen(message); i++)
        {
            int bytesAdded = addFontBytes(currByteCount, message[i]);
            if(bytesAdded > 0)
            {
                currByteCount += bytesAdded;
            }
            else
            {
                if(wordWrap)
                {
                    // fill the rest of this line with 0s
                    while(currByteCount < 128)
                    {
                        displayLines[currentLine][currByteCount++] = 0x00;
                    }
                    // move it onto the next line
                    currentLine++;
                    // and scroll up if needed (in SCROLL mode)
                    if((displayMode == SSD1306::Mode::SCROLL) && needScroll)
                    {
                        scrollUp(1);
                    } 
                    if (currentLine > 7) 
                    { 
                        currentLine = 0;  
                        needScroll = TRUE;                  
                    }
                    currByteCount=0;
                    i--; // move back one so we try it again
                }
            }
        }

        // now fill up any left over with 0x00s on the current line
        while(currByteCount < 128)
        {
            displayLines[currentLine][currByteCount++] = 0x00;
        }

        // now point to next line and set to first char
        currByteCount = 0;
        currentLine++;
        if(currentLine > 7) 
        { 
            currentLine = 0;
            needScroll = TRUE;
        }

        updateDisplayFull();
    }

    void SSD1306::clearDisplay() {
        // blank out the line buffers
        for(int i=0; i<8; i++)
        {
            for(int j=0; j<128; j++)
            {
                displayLines[i][j] = 0;
            }
        }
        //reset the scroll pointer and line pointer
        currentScrollLine = 0;
        currentLine = 0;
        // write it out
        updateDisplayFull();
    }

    void SSD1306::setWordWrap(int b) {
        wordWrap = b;
    }

    void SSD1306::setDisplayMode(SSD1306::Mode mode) {
        displayMode = mode;
    }

    // private functions
    void SSD1306::scrollUp(int lines) {
        if(!i2cInitialised)
        {
            initDisplay();
        }
        currentScrollLine += lines;
        if (currentScrollLine > 7) { currentScrollLine -= 8; }
        scrollUpSequence[2] = currentScrollLine * 8;
        D printf("scrollUp\n");
		writeI2C(scrollUpSequence, 3);
    }

    int SSD1306::addFontBytes(int curr, unsigned char c) {
        D printf("addFontBytes - %i - ", c);
        c = toupper(c); // we only support UPPERCASE letters
        int letterIdx = (c - ' ');
        if(letterIdx > 64)
        {
            letterIdx = 65;
        }

        int letterBytes = fontData[letterIdx][0];
        if((curr + letterBytes + 1) > 127 )
        {
            return 0;
            D printf("\n");
        }
        else
        {
            for(int i=0; i<letterBytes; i++)
            {
                D printf("%x ", fontData[letterIdx][1 + i]);
                displayLines[currentLine][curr + i] = fontData[letterIdx][1 + i]; 
            }
            displayLines[currentLine][curr + letterBytes++] = 0x00;  // single byte space / seperator
            D printf("\n");
            return letterBytes;
        }
    }

    void SSD1306::setDisplayRange(int line = -1) {
        // -1 = full range
        // 0..7 = line
        D printf("setDisplayRange (7 bytes)\n");
        if(line == -1)
        {
            writeI2C(setFullRange, 7);
        }
    }

    void SSD1306::updateDisplayFull() {
        setDisplayRange(-1);
        for(int line=0; line<8; line++)
        {
            unsigned char buffer[129] = {0};
            buffer[0] = 0x40;
            for(int i=0; i<128; i++) {
                buffer[1 + i] = displayLines[line][i];
            }
            writeI2C(buffer, 129);
            /*
            for(int chunk=0; chunk< 8; chunk++)
            {
                unsigned char buffer[17] = {0};
                buffer[0] = 0x40;
                for(int i=0; i<16;i++)
                {
                    D printf("[%i]=%x ", (i+1), displayLines[line][(chunk * 16) + i]);
                    buffer[i+1] = displayLines[line][(chunk * 16) + i];
                }
                D printf("\n");
                writeI2C(buffer, 17);
            }
            */
        }
    }

    void SSD1306::writeI2C(unsigned char* data, int bytes) {
        char *deviceName;
	if(i2cAlternate) {
		deviceName = (char*)"/dev/i2c-0";
	} else {
		deviceName = (char*)"/dev/i2c-1";
	}
	    if ((i2cHandle = open(deviceName, O_RDWR)) < 0)
	    {
            printf("error opening I2C\n");
        }
        else
        {
            D printf("Opened I2C bus (Handle = %i)\n", i2cHandle);
            if (ioctl(i2cHandle, I2C_SLAVE, i2cAddress) < 0)
            {
                printf("Error at ioctl\n");
            }
            else
            {
                D printf("writeI2C : ");
                for(int i=0; i<bytes; i++) { D printf("%x ", data[i]); }
                write(i2cHandle, data, bytes);
                D printf("\n");
            }
            
            // Close the i2c device bus
            close(*deviceName);
    	}

    }


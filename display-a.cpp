// Ken Hughes
// July 2016


#include <unistd.h>//Needed for I2C port
#include <fcntl.h>//Needed for I2C port
#include <sys/ioctl.h>//Needed for I2C port
#include <linux/i2c-dev.h>//Needed for I2C port
#include <stdio.h>
#include <string.h>



void writeI2C(unsigned char* data, int bytes) {
    int i2cAddress = 0x3C;
    int i2cHandle;

    char *deviceName = (char*)"/dev/i2c-1";
    if ((i2cHandle = open(deviceName, O_RDWR)) < 0) {
        printf("error opening I2C\n");
    }
    else {
        if (ioctl(i2cHandle, I2C_SLAVE, i2cAddress) < 0) {
            printf("Error at ioctl\n");
        }
        else {
            write(i2cHandle, data, bytes);
        }
            
        // Close the i2c device bus
        close(*deviceName);
    }
}


int main() {

    // initialise the display
    unsigned char initSequence[26] = {0x00,0xAE,0xA8,0x3F,0xD3,0x00,0x40,0xA1,0xC8,0xDA,0x12,0x81,0x7F,
                                          0xA4,0xA6,0xD5,0x80,0x8D,0x14,0xD9,0x22,0xD8,0x30,0x20,0x00,0xAF};
    writeI2C(initSequence, 26);

    // set the range we want to use (whole display)
    unsigned char setFullRange[7] = {0x00,0x21,0x00,0x7F,0x22,0x00,0x07};
    writeI2C(setFullRange,7);

    // send the letter A to the display
    unsigned char letterA[5] = {0x40,0x7E,0x12,0x12,0x7E};
    writeI2C(letterA,5);

}




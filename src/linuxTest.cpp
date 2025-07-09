#include <stdio.h>
#include <stdint.h>
#include <iostream>

#include <fcntl.h>
#include <termios.h>
#include <unistd.h>

#include <errno.h>
#include <string.h>
// #include <locale.h>

extern "C" {
#include "ableHandler.h"
}


#define SERIAL_PORT "/dev/ttyUSB0"
#define BAUD_RATE 9600

using namespace std;

int serial_port;

int initSerial(void){
    struct termios tty;
    int flag;

    serial_port = open(SERIAL_PORT, O_RDWR);
    // Check for errors
    if (serial_port < 0) {
        printf("Error %i from open: %s\n", errno, strerror(errno));
        return 1;
    }

    if(tcgetattr(serial_port, &tty) != 0) {
        printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
        return 2;
    }

    tty.c_cflag &= ~PARENB;     // parity disable
    tty.c_cflag &= ~CSTOPB;     // 1 stop bit
    tty.c_cflag &= ~CSIZE;  tty.c_cflag |= CS8;     // 8 characters
    tty.c_cflag |= CRTSCTS;    // enable hardware flow
    tty.c_cflag |= CREAD | CLOCAL;      // enable read and ignore modem lines

    tty.c_lflag &= ~ICANON;
    tty.c_lflag &= ~ECHO; // Disable echo
    tty.c_lflag &= ~ECHOE; // Disable erasure
    tty.c_lflag &= ~ECHONL; // Disable new-line echo
    tty.c_lflag &= ~ISIG;

    tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
    tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes
    tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
    tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed

    cfsetispeed(&tty, BAUD_RATE);
    cfsetospeed(&tty, BAUD_RATE);

    tty.c_cc[VMIN] = 1;
    tty.c_cc[VTIME] = 0;

    // Save tty settings, also checking for error
    if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
        printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
        return 3;
    }

    // flag = TIOCM_RTS;
    // ioctl(serial_port, TIOCMBIS, &flag);
    return 0;
}

void ableWriteCallback(uint8_t *toWrite, uint8_t writeLen){
    write(serial_port, toWrite, writeLen);
}

void ableReadCallback(uint8_t *toRead, uint8_t readLen){
    int n;

    while(readLen){
        n = read(serial_port, toRead, readLen);
        readLen -= n;
        toRead += n;
    }
}


void test1(void){
    // read(serial_port, data+n, 6);
    // string toSend = "Hello World!\n";
    // write(serial_port, toSend.c_str(), toSend.size());

    char fwRev[6];
    ableQueryFirmwareVersion(fwRev);
    cout << fwRev << endl;

    ablePrint("Hello World!");

    ableEnableUnderline(true);
    ablePrint("Hello World!");
    ableEnableUnderline(false);

    ableEnableInvertedPrint(true);
    ablePrint("Hello World!");
    ableEnableInvertedPrint(false);

    ableSetPrintMode(0, 0, 1, 0);
    ablePrint("Hello World!");
    ableSetPrintMode(0, 1, 0, 0);
    ablePrint("Hello World!");

    ablePrintAndFeed(1);

    ableSetPrintMode(0, 0, 0, 0);
    ablePrint("Hello World!");
    ableSetPrintMode(1, 0, 0, 0);
    ablePrint("Hello World!");
    ableSetPrintMode(2, 0, 0, 0);
    ablePrint("Hello World!");
    ableSetPrintMode(3, 0, 0, 0);
    ablePrint("Hello World!");

    ablePrintAndFeed(1);
}

void test2(void){
    uint8_t graphic[9] = {0xC3, 0x66, 0x3C, 0x66, 0xC3, 0x66, 0x3C, 0x66, 0xC3};

    ablePrintGraphic(graphic, 9, 0);
    ablePrintAndFeed(0);
}

void test3(void){
    uint8_t graphic[9] = {0xC3, 0x66, 0x3C, 0x66, 0xC3, 0x66, 0x3C, 0x66, 0xC3};

    ablePrintGraphic(graphic, 9, 0);
    ablePrint("Hello World!");
    ablePrintAndFeed(5);
}

int main(void){
    int stat;

    cout << "ABLE test code" << endl;

    stat = initSerial();
    if(stat != 0){
        printf("Error while starting, %d", stat);
        return 0;
    }

    ableInitialize();

    // test1();
    // test2();
    test3();

    close(serial_port);
    return 0;
}
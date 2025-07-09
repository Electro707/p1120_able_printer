/**
 * A generic C library file for handling ABLE printers
 * 
 * This is designed to be agnostic to what it's run on, allowing
 *  embedding in both microcontrollers and PC firmware
 */

#include "ableHandler.h"
#include <string.h>

#define ESCAPE  0x1B
#define GS      0x1D


#define CMD_INIT            0x40
#define CMD_UNDERLINE       0x2D
#define CMD_PRINT_FEED      0x64
#define CMD_REQUEST_INFO    0x49
#define CMD_INVERT          0x7B
#define CMD_SET_PRINT_MODE  0x21
#define CMD_PRINT_GRAPHIC   0x2A

uint8_t queryBuff[BUFFER_MAX_SIZE];
uint8_t queryBuffIdx;

void _sendCmdOneArg(uint8_t cmd, uint8_t arg1);
void _sendCmd(uint8_t cmd);
void _queryOneArg(uint8_t cmd, uint8_t arg1, uint8_t returnSize);
void ableQueryInfo(uint8_t infoIdx);

// handles reading one byte at a time
void ableParse(char r){
    if(r & 0x80){
        // status byte
    }
    else{
        // we received some information
    }
}

void ableEnableUnderline(uint8_t enable){
    _sendCmdOneArg(CMD_UNDERLINE, enable);
}

void ableEnableInvertedPrint(uint8_t enable){
    _sendCmdOneArg(CMD_INVERT, enable);
}

void ablePrintAndFeed(uint8_t lines){
    _sendCmdOneArg(CMD_PRINT_FEED, lines);
}

void ableInitialize(void){
    _sendCmd(CMD_INIT);
}

void ableSetPrintMode(uint8_t font, uint8_t doubleHeight, uint8_t doubleWidth, uint8_t underline){
    uint8_t v = 0;
    v |= (font & 0x7);
    v |= (doubleHeight << 4);
    v |= (doubleWidth << 5);
    v |= (underline << 7);
    _sendCmdOneArg(CMD_SET_PRINT_MODE, v);
}

void ablePrint(const char *str){
    uint8_t n = '\n';
    ableWriteCallback((uint8_t *)str, strlen(str));
    ableWriteCallback(&n, 1);
}

// the graphic array must be the size of dots divided by 8, rounded up
void ablePrintGraphic(uint8_t *graphic, uint8_t nDots, uint8_t mode){
    uint8_t toWrite[5] = {ESCAPE, CMD_PRINT_GRAPHIC, mode, nDots & 0xFF, nDots >> 8};
    ableWriteCallback(toWrite, 5);
    ableWriteCallback(graphic, nDots);
}

// fwRev must be of length 6
void ableQueryFirmwareVersion(char *fwRev){
    ableQueryInfo(3);
    fwRev[0] = (queryBuff[0] >> 4) + 0x30;
    fwRev[1] = '.';
    fwRev[2] = (queryBuff[0] & 0xF) + 0x30;
    fwRev[3] = '.';
    fwRev[4] = (queryBuff[1] >> 4) + 0x30;
    fwRev[5] = (queryBuff[1] & 0xF) + 0x30;
}

void ableQueryInfo(uint8_t infoIdx){
    _queryOneArg(CMD_REQUEST_INFO, infoIdx, 2);
}

void _queryOneArg(uint8_t cmd, uint8_t arg1, uint8_t returnSize){
    uint8_t toWrite[3] = {GS, cmd, arg1};

    queryBuffIdx = 0;
    ableWriteCallback(toWrite, 3);
    ableReadCallback(queryBuff, 2);

    // todo: status returns for sucessful or not
}

void _sendCmd(uint8_t cmd){
    uint8_t toWrite[2] = {ESCAPE, cmd};
    ableWriteCallback(toWrite, 2);
}

void _sendCmdOneArg(uint8_t cmd, uint8_t arg1){
    uint8_t toWrite[3] = {ESCAPE, cmd, arg1};
    ableWriteCallback(toWrite, 3);
}


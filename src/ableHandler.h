#ifndef ABLE_HANDLER
#define ABLE_HANDLER

#include "stdint.h"

#define BUFFER_MAX_SIZE     16

// // define as some embedded systems aren't a fan of stdbool
// // feel free to change "uint" to what is convenient for the end system
// typedef unsigned int bool;

// a function that must be defined externally
// that gets called when interfacing with the outside world
extern void ableWriteCallback(uint8_t *toWrite, uint8_t writeLen);

extern void ableReadCallback(uint8_t *toWrite, uint8_t writeLen);

// an accessible query buffer for any queries, such as firmware version, etc
extern uint8_t queryBuff[BUFFER_MAX_SIZE];

void ableInitialize(void);

void ableQueryFirmwareVersion(char *fwRev);

void ableEnableInvertedPrint(uint8_t enable);
void ableEnableUnderline(uint8_t enable);
void ableSetPrintMode(uint8_t font, uint8_t doubleHeight, uint8_t doubleWidth, uint8_t underline);

void ablePrintAndFeed(uint8_t lines);
void ablePrintGraphic(uint8_t *graphic, uint8_t nDots, uint8_t mode);
void ablePrint(const char *str);

#endif
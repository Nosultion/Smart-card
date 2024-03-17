#pragma once//为了避免同一个文件被include多次

#include <SPI.h>
#include <MFRC522.h>
#include "my_epd.h"

#define SS_PIN 5
#define RST_PIN 0

void printHex(byte *buffer, byte bufferSize);
void printDec(byte *buffer, byte bufferSize);
void MFRC_Init(void);
void MFRC_workLoop(void);
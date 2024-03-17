#pragma once        //为了避免同一个文件被include多次

#include <ArduinoJson.h>
#include "WiFiClient.h"
#include "my_epd.h"
#include <string>
#include <stdio.h>

extern int init_flag;
extern UBYTE *BlackImage;
extern int button_flag;
extern int current_page;

void parseInfo(WiFiClient client);
void httpRequest(String reqRes);
void callback_weather();
void weather_init(void);
#pragma once//为了避免同一个文件被include多次

#include <Arduino.h>
#include "ArduinoJson.h"
#include <stdbool.h> 
#include <stdlib.h>
#include <WIFI.h>
#include <PubSubClient.h>
#include "DEV_Config.h"
#include "my_epd.h"
#include "string.h"
#include "gps.h"

extern double jingdu;
extern double weidu;
extern int temperature;
extern int current_page;
extern bool gps_send_flag;
extern char teacher_name[];
extern char teacher_message[];
extern char parents_name[];
extern char parents_message[];
extern char school_name[];
extern char school_message[];

extern bool Key1_flag;

extern UBYTE *BlackImage,*RYImage;


void callback(char *topic, byte *payload, unsigned int length);
void Command_Response(char *topic, char *responseName, uint8_t response_Result);
void Command_Response_property(char *topic);
void Command_Response_command(char *topic, uint8_t response_Result);
void Command_Response_set(char *topic); 
void send_property_message_gps(void);
void send_property_message_sos(void);

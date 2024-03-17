#pragma once//为了避免同一个文件被include多次

#include "my_client.h"
#include <WiFiManager.h> 
#include <DNSServer.h>

extern bool mqtt_flag;
extern bool wifi_flag;
extern bool connect_flag;
extern WiFiClient espClient;
extern PubSubClient client;

void wifi_mqtt_init(void);
void subscribeTopic(void);
void callback_mqtt(void);
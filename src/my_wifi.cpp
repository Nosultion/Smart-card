#include "my_wifi.h"

int i=0;

// const char* ssid = "dduu";
// const char* password = "12345678";
// const char* ssid = "Redmi K30 Pro";
// const char* password = "Aa12345678";
const char* ssid = "爷的vivo";
const char* password = "88888888";

const char* mqtt_server = "ed9d3de13c.st1.iotda-device.cn-east-3.myhuaweicloud.com"; // 例如："iot-mqtts.cn-north-4.myhuaweicloud.com"
const int   mqtt_port = 1883; // 不使用TLS加密协议的话，MQTT默认端口号为1883                  //ed9d3de13c.st1.iotda-device.cn-east-3.myhuaweicloud.com
const char* clientId = "6451ba8c0eafe83f11ddc64d_ESP32_0_0_2023081807";
const char* mqttUser = "6451ba8c0eafe83f11ddc64d_ESP32";
const char* mqttPassword = "ebe128f236d37dadc7b64bee67f984ac2ea3121b63e3706ed200ed832f0d5f03";

void wifi_mqtt_init(void)
{
  WiFiManager wifiManager;
  
  //清除ESP32所存储的WiFi连接信息以便测试WiFiManager工作效果
  //wifiManager.resetSettings();
  //Serial.println("ESP32 WiFi Settings Cleared");

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    i++;
    delay(1000);
    if(i>=5)
    {
      break;
    }
    Serial.println("Connecting to WiFi...");
  }
  if(WiFi.status() == WL_CONNECTED)
  {
    connect_flag=1;
    Serial.println("Connected to the WiFi network");
  }
  else
  {
    // 自动连接WiFi。以下语句的参数是连接ESP8266时的WiFi名称
    wifiManager.autoConnect("智能卡片2");
    connect_flag=1;
    // 如果您希望该WiFi添加密码，可以使用以下语句：
    // wifiManager.autoConnect("智能卡片", "12345678");
    // 以上语句中的12345678是连接AutoConnectAP的密码
  }

  // WiFi连接成功后将通过串口监视器输出连接成功信息 
  Serial.println(""); 
  Serial.print("ESP32 Connected to ");
  Serial.println(WiFi.SSID());              // WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // IP

  client.setServer(mqtt_server, mqtt_port);
  
  client.setKeepAlive (60); //设置心跳时间
  
  Serial.println("Connecting to MQTT...");
  
  if (client.connect(clientId, mqttUser, mqttPassword)) 
  {
    mqtt_flag=1;
    Serial.println("connected");  
    subscribeTopic();
  } 
  else
  {
    mqtt_flag=0;
    Serial.print("failed with state ");
    Serial.print(client.state());
  }
}

// 订阅指定主题
void subscribeTopic(void)
{
  // 建立订阅主题。
  String TopicA="$oc/devices/6451ba8c0eafe83f11ddc64d_ESP32/sys/commands/#";          //平台命令下发
  char subTopicA[TopicA.length() + 1];
  strcpy(subTopicA, TopicA.c_str());

  String TopicB="$oc/devices/6451ba8c0eafe83f11ddc64d_ESP32/sys/messages/down";      //平台消息下发
  char subTopicB[TopicB.length() + 1];
  strcpy(subTopicB, TopicB.c_str());

  String TopicC="$oc/devices/6451ba8c0eafe83f11ddc64d_ESP32/sys/properties/set/#";      //平台设置属性
  char subTopicC[TopicC.length() + 1];
  strcpy(subTopicC, TopicC.c_str());

  //串口打印
  if(client.subscribe(subTopicA)){
    Serial.println("Subscrib TopicA:");
    Serial.println(subTopicA);
  } else {
    Serial.println("Subscribe Fail...");
  }  

  if(client.subscribe(subTopicB)){
    Serial.println("Subscrib TopicB:");
    Serial.println(subTopicB);
  } else {
    Serial.println("Subscribe Fail...");
  }  

  if(client.subscribe(subTopicC)){
    Serial.println("Subscrib TopicC:");
    Serial.println(subTopicC);
  } else {
    Serial.println("Subscribe Fail...");
  }    
  
}

void callback_mqtt(void)
{
  if(mqtt_flag==0)
  {
    if (client.connect(clientId, mqttUser, mqttPassword )) 
    {
      mqtt_flag=1;
      Serial.println("connected");  
      subscribeTopic();
    } 
    else
    {
      mqtt_flag=0;
      Serial.print("failed with state ");
      Serial.print(client.state());
    }  
  }
}

#include "my_client.h"
#include "my_epd.h"
#include "my_wifi.h"
#include <Ticker.h>
#include "GUI_Paint.h"
#include "gps.h"
#include "weather.h"
#include "MFRC.h"

extern WiFiClient espClient;
extern PubSubClient client;
extern String reqUserKey;
extern String reqLocation;
extern String reqUnit;
extern bool sos_flag;
extern int button_flag; //按下按键标志

Ticker button_ticker;
Ticker weather_ticker;
Ticker gps_ticker;
Ticker mqtt_ticker;
Book book={0};

int Page=0;       //背单词页数
bool mode=0;     //背单词模式和二维码模式切换标志
bool qie_flag=0; //保证切换到单词表的时候是之前所读的那一页
bool mqtt_flag=0;
bool wifi_flag=0;
bool inits_flag=1; //保证初始化时切换到第一页

void setup() 
{
  Serial.begin(9600);

  pinMode(pushButton1, INPUT_PULLUP);
  pinMode(pushButton2, INPUT_PULLUP);
  pinMode(pushButton3, INPUT_PULLUP); 
  pinMode(pushButton4, INPUT_PULLUP); 
  pinMode(mada, OUTPUT); 

  digitalWrite(pushButton1,HIGH);
  digitalWrite(pushButton2,HIGH);
  digitalWrite(pushButton3,HIGH);
  digitalWrite(pushButton4,HIGH);
  digitalWrite(mada,LOW);

  my_epd_init();

  wifi_mqtt_init();

  weather_init();

  my_epd_show_name_kebiao();

  gps_init();

  MFRC_Init();
                              
  Serial.println("---- Finish Init ----\r\n");

  client.setCallback(callback);

  weather_ticker.attach(360,callback_weather);

  gps_ticker.attach(60,callback_gps);

  mqtt_ticker.attach(20,callback_mqtt);

  button_ticker.attach_ms(100,Button_check);  //每100ms执行一次Button_check
}

void loop() 
{
  if(mqtt_flag==1)
  {
    client.loop();//mqtt保活
  }
  if(button_flag==1)
  {
    my_epd_show_name_kebiao();
  }
  else if(button_flag==2)
  {
    if(Page==0 && qie_flag==1)        //保证切回来是原来的页面
    {
      if(inits_flag==1)
      {
        inits_flag=0;
      }
      else
      {
        Page=2;
      }
    }
    else if(qie_flag==1)
    {
      Page--;
    }
    my_epd_show_word();
  }
  else if(button_flag==3)
  {
    if(mode==0)
    {
      my_epd_show_erweima();
    }
    else
    {
      switch(Page)
      {
        case 0:
          book.L3_flag=1;
          break;
        case 1:
          book.L1_flag=1;
          break;
        case 2:
          book.L2_flag=1;
          break;                  
      }
      if(Page==0)
      {
        Page=2;
      }
      else{
        Page--;
      }
      my_epd_show_word();
    }
  }
  else if(button_flag==4)
  {
    if(sos_flag==0)
    {
      sos_flag=1;
      send_property_message_sos();          //发送SOS属性信息：1
    }
    my_epd_show_sos();
  }
  MFRC_workLoop();
}



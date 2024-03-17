#include "weather.h"

int year;
int month;
int day;
int temperature;
int humidity;
char date[30];
const char* weather="晴";
char test_day[30]; //白天天气

const char* host = "api.seniverse.com";     // 将要连接的服务器地址  
const int httpPort = 80;                    // 将要连接的服务器端口      

// 心知天气HTTP请求所需信息
String reqUserKey = "SpB7B-_hD_9vWtB-a";   // 私钥
String reqLocation = "深圳";               // 城市
String reqUnit = "c";                      // 摄氏/华氏

// 建立心知天气API当前天气请求资源地址
String reqRes = "/v3/weather/daily.json?key=" + reqUserKey +
                + "&location=" + reqLocation + "&language=en&unit=" +
                reqUnit + "&start=0&days=1";

// 向心知天气服务器服务器请求信息并对信息进行解析
void httpRequest(String reqRes)
{
  WiFiClient client;

  // 建立http请求信息
  String httpRequest = String("GET ") + reqRes + " HTTP/1.1\r\n" + 
                              "Host: " + host + "\r\n" + 
                              "Connection: close\r\n\r\n";
  Serial.println(""); 
  Serial.print("Connecting to "); Serial.print(host);

  // 尝试连接服务器
  if (client.connect(host, 80)){
    Serial.println(" Success!");
 
    // 向服务器发送http请求信息
    client.print(httpRequest);
    Serial.println("Sending request: ");
    Serial.println(httpRequest);  
    
    //查看响应信息
    // Serial.println("---------------------");
    // Serial.println(client.readString());
    // Serial.println("---------------------");
    
    // 获取并显示服务器响应状态行 
    String status_response = client.readStringUntil('\n');
    Serial.print("status_response: ");
    Serial.println(status_response);
 
    // 使用find跳过HTTP响应头
    if (client.find("\r\n\r\n")) {
      Serial.println("Found Header End. Start Parsing.");
    }
    // 利用ArduinoJson库解析心知天气响应信息
    parseInfo(client); 
  } else {
    Serial.println(" connection failed!");
  }   
  //断开客户端与服务器连接工作
  client.stop(); 
}

// 利用ArduinoJson库解析心知天气响应信息
void parseInfo(WiFiClient client)
{
    // Stream& input;
    StaticJsonDocument<1024> doc;
    DeserializationError error = deserializeJson(doc, client);

    if (error) {
    Serial.print("deserializeJson() failed: ");
    Serial.println(error.c_str());
    return;
    }

    JsonObject results_0 = doc["results"][0];

    JsonObject results_0_location = results_0["location"];

    // const char* results_0_location_id = results_0_location["id"]; // "WS10730EM8EV"
    // const char* results_0_location_name = results_0_location["name"]; // "Shenzhen"
    // const char* results_0_location_country = results_0_location["country"]; // "CN"
    // const char* results_0_location_path = results_0_location["path"]; // "Shenzhen,Shenzhen,Guangdong,China"
    // const char* results_0_location_timezone = results_0_location["timezone"]; // "Asia/Shanghai"
    // const char* results_0_location_timezone_offset = results_0_location["timezone_offset"]; // "+08:00"

    JsonObject results_0_daily_0 = results_0["daily"][0];
    // const char* results_0_daily_0_date = results_0_daily_0["date"]; // "2023-06-14"
    // const char* results_0_daily_0_text_day = results_0_daily_0["text_day"]; // "Heavy rain"
    // const char* results_0_daily_0_code_day = results_0_daily_0["code_day"]; // "15"
    // const char* results_0_daily_0_text_night = results_0_daily_0["text_night"]; // "Heavy rain"
    // const char* results_0_daily_0_code_night = results_0_daily_0["code_night"]; // "15"
    // const char* results_0_daily_0_high = results_0_daily_0["high"]; // "29"
    // const char* results_0_daily_0_low = results_0_daily_0["low"]; // "25"
    // const char* results_0_daily_0_rainfall = results_0_daily_0["rainfall"]; // "12.64"
    // const char* results_0_daily_0_precip = results_0_daily_0["precip"]; // "0.89"
    // const char* results_0_daily_0_wind_direction = results_0_daily_0["wind_direction"]; // "CLM"
    // const char* results_0_daily_0_wind_direction_degree = results_0_daily_0["wind_direction_degree"];
    // const char* results_0_daily_0_wind_speed = results_0_daily_0["wind_speed"]; // "8.4"
    // const char* results_0_daily_0_wind_scale = results_0_daily_0["wind_scale"]; // "2"
    // const char* results_0_daily_0_humidity = results_0_daily_0["humidity"]; // "89"

    // const char* results_0_last_update = results_0["last_update"]; // "2023-06-14T08:00:00+08:00"

    String results_0_location_name_string = results_0_location["name"].as<String>();        //城市
    String results_0_daily_0_date_string = results_0_daily_0["date"].as<String>();          //日期
    int results_0_daily_0_high_int = results_0_daily_0["high"].as<int>();             //最高温度
    int results_0_daily_0_low_int = results_0_daily_0["low"].as<int>();               //最低温度
    int results_0_daily_0_humidity_int = results_0_daily_0["humidity"].as<int>();           //湿度
    String results_0_daily_0_text_day_String = results_0_daily_0["text_day"].as<String>();  //天气

    temperature = (results_0_daily_0_high_int +results_0_daily_0_low_int)/2;               //平均温度
    humidity = results_0_daily_0_humidity_int;
    strcpy(test_day,results_0_daily_0_text_day_String.c_str());
    
    strcpy(date,results_0_daily_0_date_string.c_str());
    sscanf(date, "%d-%d-%d", &year, &month, &day);  //提取年月日

    if(!strcmp(test_day,"Sunny"))
    {
      weather="晴";
    }
    else if(!strcmp(test_day,"Partly Cloudy"))
    {
      weather="晴间多云";
    }
    else if(!strcmp(test_day,"Mostly Cloudy"))
    {
      weather="大部多云";
    }
    else if(!strcmp(test_day,"Overcast"))
    {
      weather="阴";
    }
    else if(!strcmp(test_day,"Shower"))
    {
      weather="阵雨";
    }
    else if(!strcmp(test_day,"Thundershower"))
    {
      weather="雷阵雨";
    }
    else if(!strcmp(test_day,"Cloudy"))
    {
      weather="多云";
    }
    else if(!strcmp(test_day,"Thundershower with Hail"))
    {
      weather="雷阵雨伴有冰雹";
    }    
    else if(!strcmp(test_day,"Light rain"))
    {
      weather="小雨";
    } 
    else if(!strcmp(test_day,"Heavy rain"))
    {
      weather="大雨";
    }     
    else if(!strcmp(test_day,"Storm"))
    {
      weather="暴雨";
    }     
    else if(!strcmp(test_day,"Heavy Storm"))
    {
      weather="大暴雨";
    }     
    else if(!strcmp(test_day,"Severe Storm"))
    {
      weather="特大暴雨";
    }   
    else if(!strcmp(test_day,"Ice Rain"))
    {
      weather="冻雨";
    }
    else if(!strcmp(test_day,"Sleet"))
    {
      weather="雨夹雪";
    }    
    else if(!strcmp(test_day,"Snow Flurry"))
    {
      weather="阵雪";
    } 
    else if(!strcmp(test_day,"Light Snow"))
    {
      weather="小雪";
    }     
    else if(!strcmp(test_day,"Moderate Snow"))
    {
      weather="中雪";
    }     
    else if(!strcmp(test_day,"Heavy Snow"))
    {
      weather="大雪";
    }     
    else if(!strcmp(test_day,"Snowstorm"))
    {
      weather="暴雪";
    }   
    else if(!strcmp(test_day,"Dust"))
    {
      weather="浮尘";
    }
    else if(!strcmp(test_day,"Sand"))
    {
      weather="扬沙";
    }    
    else if(!strcmp(test_day,"Duststorm"))
    {
      weather="沙尘暴";
    } 
    else if(!strcmp(test_day,"Foggy"))
    {
      weather="雾";
    }     
    else if(!strcmp(test_day,"Haze"))
    {
      weather="霾";
    }     
    else if(!strcmp(test_day,"Windy"))
    {
      weather="风";
    }     
    else if(!strcmp(test_day,"Tornado"))
    {
      weather="龙卷风";
    }
    else if(!strcmp(test_day,"Moderate rain"))
    {
      weather="中雨";
    }
        
    Serial.println("======Today Weahter =======");
    Serial.print("Location: ");
    Serial.println(results_0_location_name_string);
    Serial.print("DATE: ");
    Serial.println(results_0_daily_0_date_string);
    Serial.print("test_day: ");
    Serial.println(results_0_daily_0_text_day_String);
    Serial.print("High: ");
    Serial.println(results_0_daily_0_high_int);
    Serial.print("LOW: ");
    Serial.println(results_0_daily_0_low_int);
    Serial.print("temperature: ");
    Serial.println(temperature);
    Serial.print("Humidity: ");
    Serial.println(results_0_daily_0_humidity_int);        

    Serial.print("weather: ");
    Serial.println(weather);
    Serial.println("==============================");
}

void callback_weather()
{
  httpRequest(reqRes);    
}

void weather_init(void)
{
  httpRequest(reqRes);  
}
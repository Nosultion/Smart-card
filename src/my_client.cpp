#include "my_client.h"

WiFiClient espClient;
PubSubClient client(espClient);

//平台消息下发
const char* topic_message_down="$oc/devices/6451ba8c0eafe83f11ddc64d_esp32wroom3d/sys/messages/down";
//设备消息上报
const char* topic_message_up="$oc/devices/6451ba8c0eafe83f11ddc64d_esp32wroom3d/sys/messages/up";
//设备上报属性数据
const char* topic_properties_report =  "$oc/devices/6451ba8c0eafe83f11ddc64d_esp32wroom3d/sys/properties/report";
//设备返回命令响应
const char* topic_Commands_Response = "$oc/devices/6451ba8c0eafe83f11ddc64d_esp32wroom3d/sys/commands/response/request_id=";
//属性查询响应结果
const char* properties_Commands_Response ="$oc/devices/6451ba8c0eafe83f11ddc64d_esp32wroom3d/sys/properties/get/response/request_id=";
//平台设置设备属性响应
const char* Properties_set_Response="$oc/devices/6451ba8c0eafe83f11ddc64d_esp32wroom3d/sys/properties/set/response/request_id=";

/*将命令响应topic与resquest_id结合起来*/
char topicRes[200] = {0};

bool sos_flag=0;
int command_flag=0;
uint8_t SUCCESS=0;

// const char* name="黄思超";
// const char* classes="高三五班";
// const char* card_number="413088";

char name[20]="王小红";
char classes[20]="高二三班";
char card_number[10]="41002";

char class_11[]="语文";
char class_12[]="数学";
char class_13[]="英语";
char class_14[]="物理";
char class_15[]="音乐";
char class_16[]="历史";

char class_21[]="化学";
char class_22[]="政治";
char class_23[]="地理";
char class_24[]="音乐";
char class_25[]="数学";
char class_26[]="语文";

char class_31[]="化学";
char class_32[]="美术";
char class_33[]="英语";
char class_34[]="语文";
char class_35[]="体育";
char class_36[]="数学";

char class_41[]="物理";
char class_42[]="语文";
char class_43[]="地理";
char class_44[]="历史";
char class_45[]="政治";
char class_46[]="生物";

char class_51[]="语文";
char class_52[]="数学";
char class_53[]="英语";
char class_54[]="体育";
char class_55[]="化学";
char class_56[]="物理";

const char* buf;

int init_flag=0;

void callback(char *topic, byte *payload, unsigned int length)
{
  char *pstr = topic; //指向topic字符串，提取request_id用
  Serial.println();
  Serial.println();
  Serial.print("Message arrived [");
  Serial.print(topic);  //将收到消息的topic展示出来
  Serial.print("] ");
  Serial.println();

  payload[length] = '\0'; //在收到的内容后面加上字符串结束符
  char strPayload[1000] = {0}; 
  strcpy(strPayload, (const char*)payload);
  //Serial.println((char *)payload);  //打印出收到的内容
  Serial.println("The content received:");
  Serial.println(strPayload);

  /*request_id解析部分*/
  //topicRes[200] = {0};
  char arr[100];  //存放request_id
  int flag = 0;
  char *p = arr;
  command_flag=0;
  while(*pstr)  //以'='为标志，提取出request_id
  {
    if(flag) *p ++ = *pstr;
    if(*pstr == '=') flag = 1;
    if(*pstr == 'c'&&*(pstr+1) == 'o') command_flag = 1;
    if(*pstr == 'g'&&*(pstr+1) == 'e') command_flag = 2;
    if(*pstr == 'd'&&*(pstr+1) == 'o'&&*(pstr+2) == 'w' && *(pstr+3) == 'n') command_flag = 3;
    if(*pstr == 'u'&&*(pstr+1) == 'p') command_flag = 4;
    if(*pstr == 's'&&*(pstr+1) == 'e'&&*(pstr+2) == 't') command_flag = 5;
    pstr++;
  }
  Serial.print("command_flag=");
  Serial.println(command_flag);

  *p = '\0';  

  if(command_flag==1)                                   //设备返回命令响应       
  {
    Serial.print("request_id:");
    Serial.println(arr);
    strcpy(topicRes, topic_Commands_Response);
    strcat(topicRes, arr);
    Serial.println("Response topic:");
    Serial.println(topicRes);
    Command_Response_command(topicRes, SUCCESS);
    //Command_Response(topicRes, "true", SUCCESS);

    // Stream& input;
    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, strPayload);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    const char* service_id = doc["service_id"]; // "ESP32Paper"
    const char* command_name = doc["command_name"]; // "学生信息"
    
    //学生信息提取
    if(!strcmp(command_name,"学生信息"))          //字符串不能直接用==进行比较，只能用strcmp函数进行比较，相等返回0
    {
      JsonObject paras_information = doc["paras"]["information"];
      Serial.println("receive content:");
      if(paras_information.containsKey("name"))
      {

        buf=paras_information["name"];
        strcpy(name,buf);
        Serial.println(name);
      }
      if(paras_information.containsKey("class"))
      {
        buf=paras_information["class"];
        strcpy(classes,buf);
        Serial.println(classes);
      }
      if(paras_information.containsKey("card"))
      {
        buf=paras_information["card"];         //card最多6位
        strcpy(card_number,buf);
        Serial.println(card_number);
      }
      Serial.println("------------------------- --");

      if(current_page==1 && Key1_flag==1)
      {
        Key1_flag=0;
        my_epd_show_name_kebiao();
      }
    }
  }
  else if(command_flag==2)                              //属性查询响应结果
  {
    Serial.print("request_id:");
    Serial.println(arr);
    strcpy(topicRes, properties_Commands_Response);
    strcat(topicRes, arr);
    Serial.println("Response topic:");
    Serial.println(topicRes);
    Command_Response_property(topicRes);                 //平台查询属性响应函数

  }
  else if(command_flag==3)                               //平台消息下发（通知消息）
  {
    // Stream& input;

    StaticJsonDocument<1024> doc;

    DeserializationError error = deserializeJson(doc, strPayload);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    const char* m_name = doc["name"]; // "ma"
    const char* m_id = doc["id"]; // "3766c2c1-baf9-458b-a1a5-54cba0c29b2b"

    JsonObject Payload_content = doc["content"];
    //老师家长通知信息
    if(Payload_content.containsKey("tongzhi"))
    {
      JsonObject content_tongzhi = doc["content"]["tongzhi"];
      Serial.println("information receive:");

      if(content_tongzhi.containsKey("teacher"))
      {
        Serial.println("---ready to teacher-------------------");
        buf = content_tongzhi["teacher"]["name"]; // "马梓游"
        strcpy(teacher_name,buf);
        buf = content_tongzhi["teacher"]["text"]; // "来吃饭"
        strcpy(teacher_message,buf);
        Serial.print("teacher name:");
        Serial.println(teacher_name);
        Serial.print("teacher said:");
        Serial.println(teacher_message);
      }
      
      if(content_tongzhi.containsKey("parents"))
      {
        Serial.println("---ready to parents-------------------");
        buf = content_tongzhi["parents"]["name"]; // "马梓游"
        strcpy(parents_name,buf);
        buf = content_tongzhi["parents"]["text"]; // "来吃饭"
        strcpy(parents_message,buf);
        Serial.print("parents name:");
        Serial.println(parents_name);
        Serial.print("parents said:");
        Serial.println(parents_message);
      }

      if(content_tongzhi.containsKey("school"))
      {
        Serial.println("---ready to parents-------------------");
        buf = content_tongzhi["school"]["name"]; // "马梓游"
        strcpy(school_name,buf);
        buf = content_tongzhi["school"]["text"]; // "来吃饭"
        strcpy(school_message,buf);
        Serial.print("school name:");
        Serial.println(school_name);
        Serial.print("school said:");
        Serial.println(school_message);
      }

      if(current_page==1)
      {
        Key1_flag=0;
        my_epd_show_name_kebiao();
      }
      
      digitalWrite(mada,HIGH);
      delay(3000);
      digitalWrite(mada,LOW);   //马达震动

    }

    //学生课表信息
    if(Payload_content.containsKey("schedule"))
    {
      JsonObject content_schedule = doc["content"]["schedule"];
      if(content_schedule.containsKey("day1"))
      {
        JsonObject content_schedule_day1 = content_schedule["day1"];
        buf = content_schedule_day1["11"]; // "语文"
        strcpy(class_11,buf);
        buf = content_schedule_day1["12"]; // "地理"
        strcpy(class_12,buf);
        buf = content_schedule_day1["13"]; // "英语"
        strcpy(class_13,buf);
        buf = content_schedule_day1["14"]; // "数学"
        strcpy(class_14,buf);
        buf = content_schedule_day1["15"]; // "语文"
        strcpy(class_15,buf);
        buf = content_schedule_day1["16"]; // "语文"
        strcpy(class_16,buf);

        Serial.println("update schedule day1 OK!"); 
      }

      if(content_schedule.containsKey("day2"))
      {
        JsonObject content_schedule_day2 = content_schedule["day2"];

        buf = content_schedule_day2["21"]; // "语文"
        strcpy(class_21,buf);
        buf = content_schedule_day2["22"]; // "地理"
        strcpy(class_22,buf);
        buf = content_schedule_day2["23"]; // "英语"
        strcpy(class_23,buf);
        buf = content_schedule_day2["24"]; // "数学"
        strcpy(class_24,buf);
        buf = content_schedule_day2["25"]; // "语文"
        strcpy(class_25,buf);
        buf = content_schedule_day2["26"]; // "语文"
        strcpy(class_26,buf);

        Serial.println("update schedule day2 OK!"); 
      }

      if(content_schedule.containsKey("day3"))
      {
        JsonObject content_schedule_day3 = content_schedule["day3"];

        buf = content_schedule_day3["31"]; // "语文"
        strcpy(class_31,buf);
        buf = content_schedule_day3["32"]; // "地理"
        strcpy(class_32,buf);
        buf = content_schedule_day3["33"]; // "英语"
        strcpy(class_33,buf);
        buf = content_schedule_day3["34"]; // "数学"
        strcpy(class_34,buf);
        buf = content_schedule_day3["35"]; // "语文"
        strcpy(class_35,buf);
        buf = content_schedule_day3["36"]; // "语文"
        strcpy(class_36,buf);

        Serial.println("update schedule day3 OK!"); 
      }

      if(content_schedule.containsKey("day4"))
      {
        JsonObject content_schedule_day4 = content_schedule["day4"];

        buf = content_schedule_day4["41"]; // "语文"
        strcpy(class_41,buf);
        buf = content_schedule_day4["42"]; // "地理"
        strcpy(class_42,buf);
        buf = content_schedule_day4["43"]; // "英语"
        strcpy(class_43,buf);
        buf = content_schedule_day4["44"]; // "数学"
        strcpy(class_44,buf);
        buf = content_schedule_day4["45"]; // "语文"
        strcpy(class_45,buf);
        buf = content_schedule_day4["46"]; // "语文"
        strcpy(class_46,buf);

        Serial.println("update schedule day4 OK!"); 
      }

      if(content_schedule.containsKey("day5"))
      {
        JsonObject content_schedule_day5 = content_schedule["day5"];

        buf = content_schedule_day5["51"]; // "语文"
        strcpy(class_51,buf);
        buf = content_schedule_day5["52"]; // "地理"
        strcpy(class_52,buf);
        buf = content_schedule_day5["53"]; // "英语"
        strcpy(class_53,buf);
        buf = content_schedule_day5["54"]; // "数学"
        strcpy(class_54,buf);
        buf = content_schedule_day5["55"]; // "语文"
        strcpy(class_55,buf);
        buf = content_schedule_day5["56"]; // "语文"
        strcpy(class_56,buf);

        Serial.println("update schedule day5 OK!"); 
      }
    }
  }
  else if(command_flag==4)                                       //消息上报
  {

  }
  else if(command_flag==5)                                       //平台设置属性
  {
    Serial.print("request_id:");
    Serial.println(arr);
    strcpy(topicRes, Properties_set_Response);
    strcat(topicRes, arr);
    Serial.println("Response topic:");
    Serial.println(topicRes);
    Command_Response_set(topicRes);

    // Stream& input;
    StaticJsonDocument<192> doc;

    DeserializationError error = deserializeJson(doc, strPayload);

    if (error) {
      Serial.print("deserializeJson() failed: ");
      Serial.println(error.c_str());
      return;
    }

    const char* services_0_service_id = doc["services"][0]["service_id"]; // "ESP32Paper"

    int services_0_properties_SOS = doc["services"][0]["properties"]["SOS"]; // 0

    sos_flag=services_0_properties_SOS;

    send_property_message_sos();

    if(current_page==4)
    {
      my_epd_show_sos();
    }

    digitalWrite(mada,HIGH);
    delay(3000);
    digitalWrite(mada,LOW);   //马达震动    
  }
}

void send_property_message_gps(void)                //上报GPS属性数据
{
    if(gps_send_flag==1)
    {
      gps_send_flag=0;
      StaticJsonDocument<192> doc;

      JsonObject services_0 = doc["services"].createNestedObject();
      services_0["service_id"] = "ESP32Paper";

      JsonObject services_0_properties_Location = services_0["properties"].createNestedObject("Location");
      services_0_properties_Location["jingdu"] = jingdu;
      services_0_properties_Location["weidu"] = weidu;

      // 序列化JSON数据为字符串
      String jsonString;
      serializeJson(doc, jsonString);

      // 打印JSON字符串
      Serial.println("jsonString:");
      Serial.println(jsonString);

      if (client.publish(topic_properties_report,jsonString.c_str()) == true)
      {
        Serial.println("Success Responed properties Request");
      } 
      else 
      {
        Serial.println("Error Responed properties Request");
      }
    }
}

void send_property_message_sos(void)                //上报SOS属性数据
{
  StaticJsonDocument<128> doc;

  JsonObject services_0 = doc["services"].createNestedObject();
  services_0["service_id"] = "ESP32Paper";
  services_0["properties"]["SOS"] = sos_flag;

  // 序列化JSON数据为字符串
  String jsonString;
  serializeJson(doc, jsonString);
  
  // 打印JSON字符串
  Serial.println("jsonString:");
  Serial.println(jsonString);

  if (client.publish(topic_properties_report,jsonString.c_str()) == true)
  {
    Serial.println("Success Responed properties Request");
  } 
  else 
  {
    Serial.println("Error Responed properties Request");
  }  
}

/*
 * 作用：   华为云IoT命令下发的响应函数
 * 参数1：  命令响应的topic
 * 参数2：  命令响应名
 * 参数3：  响应结果
 * 返回值： 无
 */
void Command_Response(char *topic, char *responseName, uint8_t response_Result) //responsename响应名称 response_Result响应参数
{
  // 创建JSON文档
  StaticJsonDocument<200> jsonDocument;

  if(response_Result == SUCCESS){
    jsonDocument["result_code"] = 0;
    //JsonObject& paras = root.createNestedObject("paras");
    jsonDocument["status"] = 200;
    jsonDocument["msg"] = "success";
  }
  else if(response_Result == FAIL){
    jsonDocument["result_code"] = 1;
    //JsonObject& paras = root.createNestedObject("paras");
    jsonDocument["status"] = 400;
    jsonDocument["msg"] = "fail";
  }
  // 序列化JSON数据为字符串
  String jsonString;
  serializeJson(jsonDocument, jsonString);
  // 打印JSON字符串
  Serial.println(jsonString);

  if (client.publish(topic,  jsonString.c_str()) == true) {
    Serial.println("Success sending response command message");
  } else {
    Serial.println("Error sending response command message");
  }

  Serial.println("-------------");
}

/*
 * 作用：   华为云IoT平台查询设备属性的响应函数
 * 参数1：  命令响应的topic
 * 返回值： 无
 */
void Command_Response_property(char *topic)           //属性响应函数
{
    //构建地址响应json     {"services": [{"service_id": "ESP32Paper","properties": { "location": {"jingdu": 18,"weidu": 18}}}]}
    StaticJsonDocument<192> doc;                        
    JsonObject services_0 = doc["services"].createNestedObject();
    services_0["service_id"] = "ESP32Paper";

    JsonObject services_0_properties_location = services_0["properties"].createNestedObject("location");
    services_0_properties_location["jingdu"] = jingdu;
    services_0_properties_location["weidu"] = weidu;

    // 序列化JSON数据为字符串
    String jsonString;
    serializeJson(doc, jsonString);
    
    // 打印JSON字符串
    Serial.println(jsonString);

    if (client.publish(topic, jsonString.c_str()) == true) 
    {
      Serial.println("Success sending response property message");
    } 
    else 
    {
      Serial.println("Error sending response property message");
    }

    Serial.println("-------------");  
}


/*
 * 作用：   华为云IoT命令下发的响应函数
 * 参数1：  命令响应的topic
 * 参数2：  命令响应结果
 * 返回值： 无
 */
void Command_Response_command(char *topic, uint8_t response_Result)     //命令响应函数
{
  // 创建JSON文档
  StaticJsonDocument<200> jsonDocument;
 
  if(response_Result == SUCCESS){
    jsonDocument["result_code"] = 0;
  }
  else if(response_Result == FAIL){
    jsonDocument["result_code"] = 1;
  }
  // 序列化JSON数据为字符串
  String jsonString;
  serializeJson(jsonDocument, jsonString);
  // 打印JSON字符串
  Serial.println(jsonString);

  if (client.publish(topic,  jsonString.c_str()) == true) {
    Serial.println("Success sending response command message");
  } else {
    Serial.println("Error sending response command message");
  }

  Serial.println("------------------------");
}


/*
 * 作用：   华为云IoT平台设置设备属性的响应函数
 * 参数1：  命令响应的topic
 * 返回值： 无
 */
void Command_Response_set(char *topic)               //平台设置属性响应函数
{
    StaticJsonDocument<96> doc;
    JsonObject response = doc.createNestedObject("response");

    response["result_code"] = 0;
    response["result_desc"] = "success";

    // 序列化JSON数据为字符串
    String jsonString;
    serializeJson(doc, jsonString);

    // 打印JSON字符串
    Serial.println("jsonString:");
    Serial.println(jsonString);

    if (client.publish(topic,jsonString.c_str()) == true)
    {
      Serial.println("Success Responed properties set");
    } 
    else 
    {
      Serial.println("Error Responed properties set");
    }
    Serial.println("------------------------");
}
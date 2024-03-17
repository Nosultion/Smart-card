#include "gps.h"
#include "string.h"
#include "Arduino.h"
#include "my_client.h"

double jingdu=113.9449;
double weidu=22.5274;
const unsigned int gpsRxBufferLength = 600;
char gpsRxBuffer[gpsRxBufferLength];
unsigned int ii = 0; 
bool gps_send_flag=0;

//#define PI 3.1415926535897932384626

int L = 13; //LED指示灯引脚

void errorLog(int num)
{
	DebugSerial.print("ERROR");
	DebugSerial.println(num);
	while (1)
	{
		digitalWrite(L, HIGH);
		delay(300);
		digitalWrite(L, LOW);
		delay(300);
	}
}

void printGpsBuffer()
{
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;
		//DebugSerial.print("Save_Data.UTCTime = ");
		//DebugSerial.println(Save_Data.UTCTime);
		if(Save_Data.isUsefull)
		{
			double c1,c2,d1,d2;
			Save_Data.isUsefull = false;
			c1=atof(Save_Data.latitude);
			c2=atof(Save_Data.longitude);
			d1=GpsDataDmToDd(c1);//经度转换
			d2=GpsDataDmToDd(c2);//纬度转换
			WG_to_GCJ(d1,d2);
			//Save_Data.isUsefull = false;
			//DebugSerial.print("Save_Data.latitude = ");
			//DebugSerial.println(Save_Data.latitude);
			//DebugSerial.print("Save_Data.N_S = ");
			//DebugSerial.println(Save_Data.N_S);
			//DebugSerial.print("Save_Data.longitude = ");
			//DebugSerial.println(Save_Data.longitude);
			//DebugSerial.print("Save_Data.E_W = ");
			//DebugSerial.println(Save_Data.E_W);
		}
		else
		{
			DebugSerial.println("GPS DATA is not usefull!");
		}	
	}
}

void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		//DebugSerial.println("**************");
		//DebugSerial.println(Save_Data.GPS_Buffer);
		for (int i = 0 ; i <= 6 ; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1);	//解析错误
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					char usefullBuffer[2]; 
					switch(i)
					{
						case 1:memcpy(Save_Data.UTCTime, subString, subStringNext - subString);break;	//获取UTC时间
						case 2:memcpy(usefullBuffer, subString, subStringNext - subString);break;	//获取UTC时间
						case 3:memcpy(Save_Data.latitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 4:memcpy(Save_Data.N_S, subString, subStringNext - subString);break;	//获取N/S
						case 5:memcpy(Save_Data.longitude, subString, subStringNext - subString);break;	//获取纬度信息
						case 6:memcpy(Save_Data.E_W, subString, subStringNext - subString);break;	//获取E/W

						default:break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if(usefullBuffer[0] == 'A')
						Save_Data.isUsefull = true;
					else if(usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

				}
				else
				{
					errorLog(2);	//解析错误
				}
			}
		}
	}
}

void gpsRead() 
{
	while (GpsSerial.available())
	{
		gpsRxBuffer[ii++] = GpsSerial.read();
		if (ii == gpsRxBufferLength)clrGpsRxBuffer();
	}

	char* GPS_BufferHead;
	char* GPS_BufferTail;
	if ((GPS_BufferHead = strstr(gpsRxBuffer, "$GPRMC,")) != NULL || (GPS_BufferHead = strstr(gpsRxBuffer, "$GNRMC,")) != NULL )
	{
		if (((GPS_BufferTail = strstr(GPS_BufferHead, "\r\n")) != NULL) && (GPS_BufferTail > GPS_BufferHead))
		{
			memcpy(Save_Data.GPS_Buffer, GPS_BufferHead, GPS_BufferTail - GPS_BufferHead);
			Save_Data.isGetData = true;

			clrGpsRxBuffer();
		}
	}
}

void clrGpsRxBuffer(void)
{
	memset(gpsRxBuffer, 0, gpsRxBufferLength);      //清空
	ii = 0;
}

//NMEA转WGS84
double GpsDataDmToDd(double data)
{
  double result;
    double dd,mm,temp;

  dd = (int)data / 100;//得到度dd
  mm = data - dd*100;//得到分mm.mmmm
  temp = mm / 60;
  result = dd + temp;
  return result;
}

//标准坐标系WGS84转GCJ-02火星坐标系
void WG_to_GCJ(double lng,double lat)
{
  double lng_r,lat_r,tran_lng,tran_lat;
  double dlat,dlng,radlat,magic,sqrtmagic;
  double lngtitude,lattitude;
  double ee = 0.00669342162296594323;
  int a = 6378245;
  lng_r = lng - 105.0;
  lat_r = lat - 35.0;
  //经度转换 
  tran_lng= 300.0 + lng_r + 2.0 * lat_r + 0.1 * lng_r * lng_r + 0.1 * lng_r * lat_r + 0.1 * sqrt(abs(lng_r));
  tran_lng = tran_lng + (20.0 * sin(6.0 * lng_r * PI) + 20.0 * sin(2.0 * lng_r * PI)) * 2.0 / 3.0;
  tran_lng = tran_lng + (20.0 * sin(lng_r * PI) + 40.0 * sin(lng_r / 3.0 * PI)) * 2.0 / 3.0;
  tran_lng = tran_lng + (150.0 * sin(lng_r / 12.0 * PI) + 300.0 * sin(lng_r / 30.0 * PI)) * 2.0 / 3.0;
  //转换纬度
  tran_lat = -100.0 + 2.0 * lng_r + 3.0 * lat_r + 0.2 * lat_r * lat_r + 0.1 * lng_r * lat_r + 0.2 * sqrt(abs(lng_r));
  tran_lat = tran_lat+(20.0 * sin(6.0 * lng_r * PI) + 20.0 * sin(2.0 * lng_r * PI)) * 2.0 / 3.0;
  tran_lat = tran_lat+(20.0 * sin(lat_r * PI) + 40.0 * sin(lat_r / 3.0 * PI)) * 2.0 / 3.0;
  tran_lat = tran_lat+(160.0 * sin(lat_r / 12.0 * PI) + 320 * sin(lat_r * PI / 30.0)) * 2.0 / 3.0;
  
  dlat = tran_lat;
  dlng = tran_lng;
  
  radlat = lat/180.0 * PI;
  magic = sin(radlat);
  magic = 1- ee * magic * magic;
  sqrtmagic = sqrt(magic);
  dlat = (dlat * 180.0)/((a * (1-ee))/(magic * sqrtmagic)*PI);
  dlng = (dlng * 180.0)/(a / sqrtmagic * cos(radlat) * PI);
  jingdu = lat + dlat;
  weidu = lng + dlng;
  Serial.println("jingdu：");
  Serial.println(jingdu);
  Serial.println("weidu：");
  Serial.println(weidu);
  Serial.println("GPS success! "); 
  gps_send_flag=1;
 } 

void gps_init()
{
	Save_Data.isGetData = false;
	Save_Data.isParseData = false;
	Save_Data.isUsefull = false;
	Serial.println("---- GPS Finish Init ----\r\n");  
	Serial.println("------------------------------\r\n");   	
}

void callback_gps()
{
  gpsRead();	//获取GPS数据
  parseGpsBuffer();//解析GPS数据
  printGpsBuffer();//输出解析后的数据

}


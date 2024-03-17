#pragma once//为了避免同一个文件被include多次

//此处为了兼容其他的多串口Arduino板子
#define GpsSerial  Serial
#define DebugSerial Serial

struct
{
	char GPS_Buffer[80];
	bool isGetData;		//是否获取到GPS数据
	bool isParseData;	//是否解析完成
	char UTCTime[11];		//UTC时间
	char latitude[11];		//纬度
	char N_S[2];		//N/S
	char longitude[12];		//经度
	char E_W[2];		//E/W
	bool isUsefull;		//定位信息是否有效
} Save_Data;

void errorLog(int num);
void printGpsBuffer();
void parseGpsBuffer();
void gpsRead() ;
void clrGpsRxBuffer(void);
double GpsDataDmToDd(double data);
void WG_to_GCJ(double lng,double lat);
void callback_gps();
void gps_init();

#pragma once        //为了避免同一个文件被include多次

#include "DEV_Config.h"
#include <Wire.h>
#include "EPD.h"
#include "GUI_Paint.h"
#include <stdlib.h>
#include "my_client.h"
#include <cstring>
#include <iostream>
#include "imageData.h"

#define pushButton1 21
#define pushButton2 16
#define pushButton3 32
#define pushButton4 17
#define mada 4

struct Book{       //是否背完该页单词标志
  bool L1_flag;
  bool L2_flag;
  bool L3_flag;
};

extern Book book;
extern int Page;
extern int year;
extern int month;
extern int day;

extern bool mode;
extern bool qie_flag;
extern bool sos_flag;
extern int init_flag;
extern int button_flag;
extern bool friend_flag; //交友标志

extern int temperature;
extern int humidity;
extern const char* weather;
extern const unsigned char gImage_erweima[];

extern char name[];
extern char classes[];
extern char card_number[];

extern char class_11[];
extern char class_12[];
extern char class_13[];
extern char class_14[];
extern char class_15[];
extern char class_16[];

extern char class_21[];
extern char class_22[];
extern char class_23[];
extern char class_24[];
extern char class_25[];
extern char class_26[];

extern char class_31[];
extern char class_32[];
extern char class_33[];
extern char class_34[];
extern char class_35[];
extern char class_36[];

extern char class_41[];
extern char class_42[];
extern char class_43[];
extern char class_44[];
extern char class_45[];
extern char class_46[];

extern char class_51[];
extern char class_52[];
extern char class_53[];
extern char class_54[];
extern char class_55[];
extern char class_56[];

void my_epd_init(void);
void my_epd_show_name_kebiao(void); 
void my_epd_show_erweima(void);
void my_epd_show_sos(void);
void my_epd_show_word(void);
void my_epd_show_friend(void);
void my_epd_show_not_connect_wifi(void);
void Button_check();

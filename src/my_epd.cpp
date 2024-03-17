#include "my_epd.h"

int count=0;     //按键按下时长标志
int current_page=1;          //标识当前处于哪个界面
int button_flag = 0;         //按键标志
int long_flag=0;             //按键按下久回到单词表第一页的标志
int newpushButton1_State=1;
int newpushButton2_State=1;
int newpushButton3_State=1;
int newpushButton4_State=1;
int oldpushButton1_State=1;
int oldpushButton2_State=1;
int oldpushButton3_State=1;   
int oldpushButton4_State=1;    //按键状态
bool Key1_flag=0; //首页切换到学生信息标志，如果不设置， 其他按键切换到第一个页面时，可能是课表也可能是学生信息
bool connect_flag=0;  //是否有联网标志

char teacher_name[20]="数学老师";
char teacher_message[100]="来喝茶";
char parents_name[20]="家长";
char parents_message[100]="来吃饭";
char phone_number[20]="1385462421";
char school_name[20]="教务处";
char school_message[100]="暴雨天气请注意安全";

UBYTE *BlackImage,*RYImage;

char acc[10];

void my_epd_init(void)
{
    Serial.println("----------------------");
    printf("EPD_4IN2_test Demo\r\n");
    DEV_Module_Init();

    printf("e-Paper Init and Clear...\r\n");
    EPD_4IN2_Init_Fast();
    EPD_4IN2_Clear();
    DEV_Delay_ms(500);
    printf("e-Paper Init and Clear Finish\r\n");

    UWORD Imagesize = ((EPD_4IN2_WIDTH % 8 == 0) ? (EPD_4IN2_WIDTH / 8 ) : (EPD_4IN2_WIDTH / 8 + 1)) * EPD_4IN2_HEIGHT;
    if ((BlackImage = (UBYTE *)malloc(Imagesize)) == NULL) {
      printf("Failed to apply for black memory...\r\n");
      while (1);
    } 

    printf("Paint_NewImage\r\n");
    Paint_NewImage(BlackImage, EPD_4IN2_WIDTH, EPD_4IN2_HEIGHT, 0, WHITE);

    my_epd_show_not_connect_wifi();
}

void my_epd_show_not_connect_wifi(void)
{
      Paint_SelectImage(BlackImage);
      Paint_Clear(WHITE); 

      Paint_DrawString_CN(168,5,"年",&Font16CN,BLACK,WHITE);
      Paint_DrawString_CN(226,5,"月",&Font16CN,BLACK,WHITE);
      Paint_DrawString_CN(284,5,"日",&Font16CN,BLACK,WHITE);
      
      Paint_DrawString_CN(5, 60,"姓名", &Font16CN, BLACK, WHITE);
      Paint_DrawString_EN(55, 64, ":", &Font24, WHITE, BLACK);
      Paint_DrawString_CN(5, 90,"班级", &Font16CN, BLACK, WHITE);
      Paint_DrawString_EN(55, 94, ":", &Font20, WHITE, BLACK);
      Paint_DrawString_CN(5, 120,"学号", &Font16CN, BLACK, WHITE);
      Paint_DrawString_EN(55, 124, ":", &Font20, WHITE, BLACK);

      Paint_DrawString_CN(205, 60,"温度", &Font16CN, BLACK, WHITE);
      Paint_DrawString_EN(255, 64, ":", &Font24, WHITE, BLACK);
      Paint_DrawString_CN(205, 90,"湿度", &Font16CN, BLACK, WHITE);
      Paint_DrawString_EN(255, 94, ":", &Font20, WHITE, BLACK);
      Paint_DrawString_CN(205, 120,"天气", &Font16CN, BLACK, WHITE);
      Paint_DrawString_EN(255, 124, ":", &Font20, WHITE, BLACK);  

      Paint_DrawString_CN(5, 170,"通知中心", &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(5, 200,parents_name, &Font16CN, BLACK, WHITE);  
      Paint_DrawString_EN(53, 204, ":", &Font20, WHITE, BLACK);  
      Paint_DrawString_CN(5, 230,teacher_name, &Font16CN, BLACK, WHITE);  
      Paint_DrawString_EN(101, 234, ":", &Font20, WHITE, BLACK);  
      Paint_DrawString_CN(5, 260,"教务处", &Font16CN, BLACK, WHITE);  
      Paint_DrawString_EN(77, 264, ":", &Font20, WHITE, BLACK);  

      EPD_4IN2_Display(BlackImage);
      printf("Drawing---finish\r\n");  
}

void my_epd_show_name_kebiao(void)  //显示名字页面
{
    current_page=1;
    qie_flag=1;
    mode=0; 

    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE); 

    if(Key1_flag==0)
    {
      Key1_flag=1;
      Paint_DrawNum(90,10, year,&Font24,BLACK,WHITE);
      Paint_DrawString_CN(168,5,"年",&Font16CN,BLACK,WHITE);
      Paint_DrawNum(192,10,month,&Font24,BLACK,WHITE);
      Paint_DrawString_CN(226,5,"月",&Font16CN,BLACK,WHITE);
      Paint_DrawNum(250,10,day,&Font24,BLACK,WHITE);
      Paint_DrawString_CN(284,5,"日",&Font16CN,BLACK,WHITE);
      
      Paint_DrawString_CN(5, 60,"姓名", &Font16CN, BLACK, WHITE);
      Paint_DrawString_EN(55, 64, ":", &Font24, WHITE, BLACK);
      Paint_DrawString_CN(75, 60,name, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(5, 90,"班级", &Font16CN, BLACK, WHITE);
      Paint_DrawString_EN(55, 94, ":", &Font20, WHITE, BLACK);
      Paint_DrawString_CN(75, 90,classes, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(5, 120,"学号", &Font16CN, BLACK, WHITE);
      Paint_DrawString_EN(55, 124, ":", &Font20, WHITE, BLACK);
      Paint_DrawString_EN(75, 125,card_number, &Font24, WHITE, BLACK);

      Paint_DrawString_CN(205, 60,"温度", &Font16CN, BLACK, WHITE);
      Paint_DrawString_EN(255, 64, ":", &Font24, WHITE, BLACK);
      Paint_DrawNum(275,62,temperature,&Font24,BLACK,WHITE);
      Paint_DrawString_CN(205, 90,"湿度", &Font16CN, BLACK, WHITE);
      Paint_DrawString_EN(255, 94, ":", &Font20, WHITE, BLACK);
      Paint_DrawNum(275,92,humidity,&Font24,BLACK,WHITE);
      Paint_DrawString_CN(205, 120,"天气", &Font16CN, BLACK, WHITE);
      Paint_DrawString_EN(255, 124, ":", &Font20, WHITE, BLACK);  
      Paint_DrawString_CN(275, 120, weather, &Font16CN, BLACK, WHITE);

      Paint_DrawString_CN(5, 170,"通知中心", &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(5, 200,parents_name, &Font16CN, BLACK, WHITE);  
      Paint_DrawString_EN(53, 204, ":", &Font20, WHITE, BLACK);  
      Paint_DrawString_CN(70, 200,parents_message, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(5, 230,teacher_name, &Font16CN, BLACK, WHITE);  
      Paint_DrawString_EN(101, 234, ":", &Font20, WHITE, BLACK);  
      Paint_DrawString_CN(118, 230,teacher_message, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(5, 260,"教务处", &Font16CN, BLACK, WHITE);  
      Paint_DrawString_EN(77, 264, ":", &Font20, WHITE, BLACK);  
      Paint_DrawString_CN(94, 260,school_message, &Font16CN, BLACK, WHITE);   
    }
    else if(Key1_flag==1)
    {
      Key1_flag=0;

      Paint_DrawString_CN(140, 11,"姓名", &Font16CN, BLACK, WHITE);
      Paint_DrawString_EN(190, 15, ":", &Font24, WHITE, BLACK);
      Paint_DrawString_CN(210, 11,name, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(110, 41,"班级", &Font16CN, BLACK, WHITE);
      Paint_DrawString_EN(160, 45, ":", &Font20, WHITE, BLACK);
      Paint_DrawString_CN(180, 41,classes, &Font16CN, BLACK, WHITE);

      Paint_DrawRectangle(15, 80, 380, 290, BLACK, DOT_PIXEL_1X1, DRAW_FILL_EMPTY);   
      Paint_DrawLine(15, 110, 380, 110, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
      Paint_DrawLine(15, 140, 380, 140, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
      Paint_DrawLine(15, 170, 380, 170, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
      Paint_DrawLine(15, 200, 380, 200, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
      Paint_DrawLine(15, 230, 380, 230, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
      Paint_DrawLine(15, 260, 380, 260, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
      Paint_DrawLine(15, 290, 380, 290, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

      Paint_DrawLine(67, 80, 67, 290, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
      Paint_DrawLine(119, 80, 119, 290, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);   
      Paint_DrawLine(171, 80, 171, 290, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);  
      Paint_DrawLine(223, 80, 223, 290, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
      Paint_DrawLine(275, 80, 275, 290, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);
      Paint_DrawLine(327, 80, 327, 290, BLACK, DOT_PIXEL_1X1, LINE_STYLE_SOLID);

      Paint_DrawString_CN(29, 81,"一", &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(81, 81,"二", &Font16CN, BLACK, WHITE);   
      Paint_DrawString_CN(133, 81,"三", &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(185, 81,"四", &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(237, 81,"五", &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(289, 81,"六", &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(341, 81,"日", &Font16CN, BLACK, WHITE);

      Paint_DrawString_CN(17, 111,class_11, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(17, 141,class_12, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(17, 171,class_13, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(17, 201,class_14, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(17, 231,class_15, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(17, 261,class_16, &Font16CN, BLACK, WHITE);
      
      Paint_DrawString_CN(69, 111,class_21, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(69, 141,class_22, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(69, 171,class_23, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(69, 201,class_24, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(69, 231,class_25, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(69, 261,class_26, &Font16CN, BLACK, WHITE);

      Paint_DrawString_CN(121, 111,class_31, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(121, 141,class_32, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(121, 171,class_33, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(121, 201,class_34, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(121, 231,class_35, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(121, 261,class_36, &Font16CN, BLACK, WHITE);

      Paint_DrawString_CN(173, 111,class_41, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(173, 141,class_42, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(173, 171,class_43, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(173, 201,class_44, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(173, 231,class_45, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(173, 261,class_46, &Font16CN, BLACK, WHITE);

      Paint_DrawString_CN(225, 111,class_51, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(225, 141,class_52, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(225, 171,class_53, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(225, 201,class_54, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(225, 231,class_55, &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(225, 261,class_56, &Font16CN, BLACK, WHITE);
    }

    EPD_4IN2_Display(BlackImage);
    printf("Drawing---finish\r\n");  
}

void my_epd_show_sos(void)
{
    current_page=4;
    Key1_flag=0; //首页切换到学生信息标志，如果不设置， 其他按键切换到第一个页面时，可能是课表也可能是学生信息
    qie_flag=1;

    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE); 

    Paint_DrawString_CN(155, 5,"求救中心", &Font16CN, BLACK, WHITE);

    if(sos_flag==1)
    {
      Paint_DrawString_CN(5, 50,"已发送求救请求", &Font16CN, BLACK, WHITE);
    }
    else
    {
      Paint_DrawString_CN(5, 50,"求救成功", &Font16CN, BLACK, WHITE);
    }


    Paint_DrawString_CN(5, 260,"联系人电话", &Font16CN, BLACK, WHITE);
    Paint_DrawString_EN(127, 264, ":", &Font20, WHITE, BLACK); 
    Paint_DrawString_EN(147, 260,phone_number, &Font24, WHITE, BLACK);

    EPD_4IN2_Display(BlackImage);
    printf("Drawing---finish\r\n");  
}

void my_epd_show_erweima(void) //显示二维码界面
{
    mode=0;
    Key1_flag=0;  
    qie_flag=1;

    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE); 
    //正放
    Paint_DrawBitMap(gImage_show);

    Paint_DrawString_CN(30, 50 ,"二", &Font24CN, BLACK, WHITE);
    Paint_DrawString_CN(30, 92 ,"维", &Font24CN, BLACK, WHITE);
    Paint_DrawString_CN(30, 134,"码", &Font24CN, BLACK, WHITE);
    Paint_DrawString_CN(30, 176,"页", &Font24CN, BLACK, WHITE);
    Paint_DrawString_CN(30, 218,"面", &Font24CN, BLACK, WHITE);

    //倒放
    // Paint_DrawBitMap(gImage_x2);
    // //Paint_SetRotate(ROTATE_180); 
    // Paint_DrawString_CN(30, 50 ,"二", &Font24CN, BLACK, WHITE);
    // Paint_DrawString_CN(30, 92 ,"维", &Font24CN, BLACK, WHITE);
    // Paint_DrawString_CN(30, 134,"码", &Font24CN, BLACK, WHITE);
    // Paint_DrawString_CN(30, 176,"页", &Font24CN, BLACK, WHITE);
    // Paint_DrawString_CN(30, 218,"面", &Font24CN, BLACK, WHITE);
    // //Paint_SetRotate(ROTATE_180); 

    EPD_4IN2_Display(BlackImage); 
    printf("Drawing---finish\r\n"); 
}

void my_epd_show_word(void)
{
    mode=1;
    qie_flag=0;
    Key1_flag=0;

    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE); 

    Paint_DrawString_CN(5, 5,"单词表", &Font24CN, BLACK, WHITE);
    switch(Page)      
    {
      case 0:
          Paint_DrawString_EN(5,56,"abandon",&Font24,WHITE,BLACK);
          Paint_DrawString_CN(134,54,"放弃", &Font16CN, BLACK, WHITE);
          Paint_DrawString_EN(5,90,"organize",&Font24,WHITE,BLACK);
          Paint_DrawString_CN(151, 88,"组织", &Font16CN, BLACK, WHITE);
          Paint_DrawString_EN(5,124,"capable",&Font24,WHITE,BLACK);
          Paint_DrawString_CN(134, 122,"有能力的", &Font16CN, BLACK, WHITE);
          Paint_DrawString_EN(5,158,"performance",&Font24,WHITE,BLACK);
          Paint_DrawString_CN(202, 156,"表演", &Font16CN, BLACK, WHITE);
          Paint_DrawString_EN(5,192,"investment",&Font24,WHITE,BLACK);
          Paint_DrawString_CN(185, 190,"投资", &Font16CN, BLACK, WHITE);
          Paint_DrawString_EN(5,226,"influence",&Font24,WHITE,BLACK);
          Paint_DrawString_CN(168, 224,"影响", &Font16CN, BLACK, WHITE);
          Paint_DrawString_EN(5, 260,"helpless",&Font24,WHITE,BLACK);
          Paint_DrawString_CN(151, 258,"无助的", &Font16CN, BLACK, WHITE);

          if(book.L1_flag==0)
          {
            Paint_DrawCircle(360,30,15,BLACK,DOT_PIXEL_3X3,DRAW_FILL_EMPTY);
          }
          else
          {
            Paint_DrawCircle(360,30,15,BLACK,DOT_PIXEL_3X3,DRAW_FILL_FULL);
          }
          break;
      case 1:
          Paint_DrawString_EN(5,56,"abandon",&Font24,WHITE,BLACK);
          Paint_DrawString_CN(134,54,"放弃", &Font16CN, BLACK, WHITE);
          if(book.L2_flag==0)
          {
            Paint_DrawCircle(360,30,15,BLACK,DOT_PIXEL_3X3,DRAW_FILL_EMPTY);
          }
          else
          {
            Paint_DrawCircle(360,30,15,BLACK,DOT_PIXEL_3X3,DRAW_FILL_FULL);
          }        
          break;
      case 2:
          Paint_DrawString_EN(5,90,"organize",&Font24,WHITE,BLACK);
          Paint_DrawString_CN(151, 88,"组织", &Font16CN, BLACK, WHITE);      
          if(book.L3_flag==0)
          {
            Paint_DrawCircle(360,30,15,BLACK,DOT_PIXEL_3X3,DRAW_FILL_EMPTY);
          }
          else
          {
            Paint_DrawCircle(360,30,15,BLACK,DOT_PIXEL_3X3,DRAW_FILL_FULL);
          }        
          break;    
    }

    Page++;
    Page%=3;

    EPD_4IN2_Display(BlackImage); 
    printf("Drawing---finish\r\n"); 
}


void Button_check(void)
{
  oldpushButton1_State=newpushButton1_State;
  newpushButton1_State=digitalRead(pushButton1);
  oldpushButton2_State=newpushButton2_State;
  newpushButton2_State=digitalRead(pushButton2);
  oldpushButton3_State=newpushButton3_State;
  newpushButton3_State=digitalRead(pushButton3);
  oldpushButton4_State=newpushButton4_State;
  newpushButton4_State=digitalRead(pushButton4);
  if(oldpushButton1_State==0 && newpushButton1_State==1)
  {
    button_flag=1;
  }   
  else if(oldpushButton2_State==0 && newpushButton2_State==1)
  {
    if(long_flag==1)
    {
      long_flag=0;
    }
    else
    {
      button_flag=2;
    }
  }
  else if(oldpushButton3_State==0 && newpushButton3_State==1)
  {
    button_flag=3;
  }
  else if(oldpushButton4_State==0 && newpushButton4_State==1)
  {
    button_flag=4;
  } 
  else if(oldpushButton2_State==0 && newpushButton2_State==0)
  {
    count++;
    if(count>=15)
    {
      count=0;
      Page=0;
      if(mode==1)
      {
        my_epd_show_word();
        long_flag=1;
      }
    }
  } 
  else 
  {
    button_flag=0;
  }
}

void my_epd_show_friend(void)
{
    Key1_flag=0; //首页切换到学生信息标志，如果不设置， 其他按键切换到第一个页面时，可能是课表也可能是学生信息
    qie_flag=1;

    Paint_SelectImage(BlackImage);
    Paint_Clear(WHITE); 

    Paint_DrawString_CN(155, 20,"交友信息", &Font16CN, BLACK, WHITE);
    Paint_DrawString_CN(130, 70,"姓名", &Font16CN, BLACK, WHITE);
    Paint_DrawString_EN(180, 74, ":", &Font24, WHITE, BLACK);
    Paint_DrawString_CN(120, 100,"班级", &Font16CN, BLACK, WHITE);
    Paint_DrawString_EN(170, 104, ":", &Font24, WHITE, BLACK);

    if(friend_flag==1)    //第一张卡信息
    {
      Paint_DrawString_CN(200, 70, "王小红", &Font16CN, BLACK, WHITE); 
      Paint_DrawString_CN(190, 100,"高二三班", &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(50, 160,"你好！我是王小红，来自高二三", &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(5, 190,"班，喜欢打羽毛球、阅读、不断学习", &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(5, 220,"和听音乐。很高兴认识你！", &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(5, 260,"联系人电话", &Font16CN, BLACK, WHITE);
      Paint_DrawString_EN(127, 264, ":", &Font20, WHITE, BLACK); 
      Paint_DrawString_EN(147, 260, "1397566989", &Font24, WHITE, BLACK);
    }
    else if(friend_flag==0) //第二张卡信息
    {
      Paint_DrawString_CN(200, 70, "李晓明", &Font16CN, BLACK, WHITE); 
      Paint_DrawString_CN(190, 100,"高二三班", &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(50, 160,"你好！我是李晓明，来自高二二", &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(5, 190,"班，喜欢打篮球、弹吉他、不断学习", &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(5, 220,"以及尝试新事物，很高兴认识你！", &Font16CN, BLACK, WHITE);
      Paint_DrawString_CN(5, 260,"联系人电话", &Font16CN, BLACK, WHITE);
      Paint_DrawString_EN(127, 264, ":", &Font20, WHITE, BLACK); 
      Paint_DrawString_EN(147, 260, "1397566988", &Font24, WHITE, BLACK);
    }

    EPD_4IN2_Display(BlackImage);
    printf("Drawing---finish\r\n");    
}
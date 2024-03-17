#include "MFRC.h"

MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class

MFRC522::MIFARE_Key key; 

// Init array that will store new NUID 
byte nuidPICC[4];

unsigned long startMillis;  // 用于存储计时开始时间
const unsigned long delayPeriod = 3000;  // 3 秒的计时周期

int clear_flag=0;
bool friend_flag=0; //交友标志

/**
 * Helper routine to dump a byte array as hex values to Serial. 
 */
void printHex(byte *buffer, byte bufferSize) 
{
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * Helper routine to dump a byte array as dec values to Serial.
 */
void printDec(byte *buffer, byte bufferSize) 
{
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

void MFRC_Init(void)
{
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  for (byte i = 0; i < 6; i++) 
  {
    key.keyByte[i] = 0xFF;
  }
  Serial.println(F("This code scan the MIFARE Classsic NUID."));
  Serial.print(F("Using the following key:"));
  printHex(key.keyByte, MFRC522::MF_KEY_SIZE);  
}

void MFRC_workLoop(void)
{
  if(clear_flag==1)
  {
    if (millis() - startMillis >= delayPeriod) 
    {
      digitalWrite(mada,LOW);   //马达震动   
      Serial.println("CLOSE");
      clear_flag=0;
    }
  }
  // Reset the loop if no new card present on the sensor/reader. This saves the entire process when idle.
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;

  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }

  if (rfid.uid.uidByte[0] != nuidPICC[0] || 
    rfid.uid.uidByte[1] != nuidPICC[1] || 
    rfid.uid.uidByte[2] != nuidPICC[2] || 
    rfid.uid.uidByte[3] != nuidPICC[3] ) 
  {
      Serial.println(F("A new card has been detected."));

      // Store NUID into nuidPICC array
      for (byte i = 0; i < 4; i++)
      {
        nuidPICC[i] = rfid.uid.uidByte[i];
      }
    
      Serial.println(F("The NUID tag is:"));
      Serial.print(F("In hex: "));
      printHex(rfid.uid.uidByte, rfid.uid.size);
      Serial.println();
      Serial.print(F("In dec: "));
      printDec(rfid.uid.uidByte, rfid.uid.size);
      Serial.println();
  }
  else 
  {
    Serial.println(F("Card read previously."));
  }

  // Check if UID matches the desired value
  if (rfid.uid.uidByte[0] == 61 &&
      rfid.uid.uidByte[1] == 40 &&
      rfid.uid.uidByte[2] == 38 &&
      rfid.uid.uidByte[3] == 243) 
  {
    Serial.println("OPEN");
    my_epd_show_friend();
    digitalWrite(mada,HIGH);
    clear_flag=1;
    startMillis = millis();  // 记录计时开始时间
  }
  else
  {
    Serial.println("CLOSE");
    clear_flag=1;
    startMillis = millis();  // 记录计时开始时间
  }
  // // 如果计时时间超过了设定的延迟周期，则将 17 和 21 引脚设置为低电平
  // if (millis() - startMillis >= delayPeriod) 
  // {
  //   digitalWrite(17, LOW);   // 设置 17 引脚为低电平
  //   digitalWrite(21, LOW);   // 设置 21 引脚为低电平
  //   Serial.println("CLOSE");
  // }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}


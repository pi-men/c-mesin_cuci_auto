/***********************************************************************************
*This program is a demo of how to display picture and 
*how to use rotate function to display string.
*This demo was made for LCD modules with 8bit or 16bit data port.
*This program requires the the LCDKIWI library.

* File                : display_graphics.ino
* Hardware Environment: Arduino UNO&Mega2560
* Build Environment   : Arduino

*Set the pins to the correct ones for your development shield or breakout board.
*This demo use the BREAKOUT BOARD only and use these 8bit data lines to the LCD,
*pin usage as follow:
*                  LCD_CS  LCD_CD  LCD_WR  LCD_RD  LCD_RST  SD_SS  SD_DI  SD_DO  SD_SCK 
*     Arduino Uno    A3      A2      A1      A0      A4      10     11     12      13                            
*Arduino Mega2560    A3      A2      A1      A0      A4      10     11     12      13                           

*                  LCD_D0  LCD_D1  LCD_D2  LCD_D3  LCD_D4  LCD_D5  LCD_D6  LCD_D7  
*     Arduino Uno    8       9       2       3       4       5       6       7
*Arduino Mega2560    8       9       2       3       4       5       6       7 

*Remember to set the pins to suit your display module!
*
* @attention
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, QD electronic SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE 
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
**********************************************************************************/

#include <LCDWIKI_GUI.h>  //Core graphics library
#include <LCDWIKI_KBV.h>  //Hardware-specific library
#include <SoftwareSerial.h>

SoftwareSerial SerialCom(10, 11);
String data;
String stat;
String time;
int headerPos;
int separatorPos;
int endPos;
int hold;

int waktu = 245;
int detik = 0;
int menit = 0;

//if the IC model is known or the modules is unreadable,you can use this constructed function
//LCDWIKI_KBV mylcd(ST7789V,A3,A2,A1,A0,A4); //model,cs,cd,wr,rd,reset
//if the IC model is not known and the modules is readable,you can use this constructed function
LCDWIKI_KBV mylcd(240, 320, A3, A2, A1, A0, A4);  //width,height,cs,cd,wr,rd,reset

//define some colour values
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

//display some strings
//Halaman awal
void homepage() {
  mylcd.Set_Text_Mode(0);
  mylcd.Set_Text_Back_colour(BLUE);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String(" Mesin Cuci Otomatis", 0, 0);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("Tekan", 80, 100);
  mylcd.Print_String("Untuk Memulai!", 10, 150);
}

//Halaman pengisian awal
void pengisianawal() {
  mylcd.Set_Text_Mode(0);
  mylcd.Set_Text_Back_colour(BLUE);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String(" Mesin Cuci Otomatis", 0, 0);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String("Proses Pengisian Air", 5, 60);
  mylcd.Print_String("Tekan ketika air", 30, 150);
  mylcd.Print_String("sudah cukup", 60, 170);
}

void pencucian() {
  mylcd.Set_Text_Mode(0);
  mylcd.Set_Text_Back_colour(BLUE);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String(" Mesin Cuci Otomatis", 0, 0);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(4);
  mylcd.Print_String("Proses", 50, 30);
  mylcd.Print_String("Mencuci", 35, 70);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(8);
  if (menit < 10) {
    mylcd.Print_String("0" + String(menit), 12, 150);
  } else {
    mylcd.Print_String(String(menit), 12, 150);
  }
  mylcd.Print_String(":", 100, 150);
  if (detik < 10) {
    mylcd.Print_String("0" + String(detik), 140, 150);
  } else {
    mylcd.Print_String(String(detik), 140, 150);
  }
}

void pengurasan1() {
  mylcd.Set_Text_Mode(0);
  mylcd.Set_Text_Back_colour(BLUE);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String(" Mesin Cuci Otomatis", 0, 0);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("Proses", 60, 30);
  mylcd.Print_String("Pengurasan 1", 20, 70);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(8);
  if (menit < 10) {
    mylcd.Print_String("0" + String(menit), 12, 150);
  } else {
    mylcd.Print_String(String(menit), 12, 150);
  }
  mylcd.Print_String(":", 100, 150);
  if (detik < 10) {
    mylcd.Print_String("0" + String(detik), 140, 150);
  } else {
    mylcd.Print_String(String(detik), 140, 150);
  }
}

void pengisian2() {
  mylcd.Set_Text_Mode(0);
  mylcd.Set_Text_Back_colour(BLUE);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String(" Mesin Cuci Otomatis", 0, 0);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("Proses", 60, 30);
  mylcd.Print_String("Pengisian 2", 20, 70);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(8);
  if (menit < 10) {
    mylcd.Print_String("0" + String(menit), 12, 150);
  } else {
    mylcd.Print_String(String(menit), 12, 150);
  }
  mylcd.Print_String(":", 100, 150);
  if (detik < 10) {
    mylcd.Print_String("0" + String(detik), 140, 150);
  } else {
    mylcd.Print_String(String(detik), 140, 150);
  }
}

void bilas1() {
  mylcd.Set_Text_Mode(0);
  mylcd.Set_Text_Back_colour(BLUE);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String(" Mesin Cuci Otomatis", 0, 0);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(4);
  mylcd.Print_String("Proses", 50, 30);
  mylcd.Print_String("Bilas 1", 35, 70);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(8);
  if (menit < 10) {
    mylcd.Print_String("0" + String(menit), 12, 150);
  } else {
    mylcd.Print_String(String(menit), 12, 150);
  }
  mylcd.Print_String(":", 100, 150);
  if (detik < 10) {
    mylcd.Print_String("0" + String(detik), 140, 150);
  } else {
    mylcd.Print_String(String(detik), 140, 150);
  }
}

void pengurasan2() {
  mylcd.Set_Text_Mode(0);
  mylcd.Set_Text_Back_colour(BLUE);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String(" Mesin Cuci Otomatis", 0, 0);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("Proses", 60, 30);
  mylcd.Print_String("Pengurasan 2", 20, 70);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(8);
  if (menit < 10) {
    mylcd.Print_String("0" + String(menit), 12, 150);
  } else {
    mylcd.Print_String(String(menit), 12, 150);
  }
  mylcd.Print_String(":", 100, 150);
  if (detik < 10) {
    mylcd.Print_String("0" + String(detik), 140, 150);
  } else {
    mylcd.Print_String(String(detik), 140, 150);
  }
}

void pengisian3() {
  mylcd.Set_Text_Mode(0);
  mylcd.Set_Text_Back_colour(BLUE);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String(" Mesin Cuci Otomatis", 0, 0);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("Proses", 60, 30);
  mylcd.Print_String("Pengisian 3", 20, 70);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(8);
  if (menit < 10) {
    mylcd.Print_String("0" + String(menit), 12, 150);
  } else {
    mylcd.Print_String(String(menit), 12, 150);
  }
  mylcd.Print_String(":", 100, 150);
  if (detik < 10) {
    mylcd.Print_String("0" + String(detik), 140, 150);
  } else {
    mylcd.Print_String(String(detik), 140, 150);
  }
}

void bilas2() {
  mylcd.Set_Text_Mode(0);
  mylcd.Set_Text_Back_colour(BLUE);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String(" Mesin Cuci Otomatis", 0, 0);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(4);
  mylcd.Print_String("Proses", 50, 30);
  mylcd.Print_String("Bilas 2", 35, 70);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(8);
  if (menit < 10) {
    mylcd.Print_String("0" + String(menit), 12, 150);
  } else {
    mylcd.Print_String(String(menit), 12, 150);
  }
  mylcd.Print_String(":", 100, 150);
  if (detik < 10) {
    mylcd.Print_String("0" + String(detik), 140, 150);
  } else {
    mylcd.Print_String(String(detik), 140, 150);
  }
}

void pengurasan3() {
  mylcd.Set_Text_Mode(0);
  mylcd.Set_Text_Back_colour(BLUE);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String(" Mesin Cuci Otomatis", 0, 0);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(3);
  mylcd.Print_String("Proses", 60, 30);
  mylcd.Print_String("Pengurasan 3", 20, 70);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(8);
  if (menit < 10) {
    mylcd.Print_String("0" + String(menit), 12, 150);
  } else {
    mylcd.Print_String(String(menit), 12, 150);
  }
  mylcd.Print_String(":", 100, 150);
  if (detik < 10) {
    mylcd.Print_String("0" + String(detik), 140, 150);
  } else {
    mylcd.Print_String(String(detik), 140, 150);
  }
}

void selesai() {
  mylcd.Set_Text_Mode(0);
  mylcd.Set_Text_Back_colour(BLUE);
  mylcd.Set_Text_colour(WHITE);
  mylcd.Set_Text_Size(2);
  mylcd.Print_String(" Mesin Cuci Otomatis", 0, 0);

  mylcd.Set_Text_Back_colour(WHITE);
  mylcd.Set_Text_colour(BLACK);
  mylcd.Set_Text_Size(4);
  mylcd.Print_String("Pencucian", 15, 100);
  mylcd.Print_String("Selesai", 35, 140);
}

void setup() {
  Serial.begin(9600);
  SerialCom.begin(9600);
  mylcd.Init_LCD();
  Serial.println(mylcd.Read_ID(), HEX);
  mylcd.Fill_Screen(WHITE);
  //homepage();
}

void loop() {
  //Read data value from main controller using uart
  if (SerialCom.available()) {
    //Read incoming data untill \n
    data = SerialCom.readStringUntil('\n');

    //Print the data to Serial Monitor
    Serial.println("Received: " + data);

    //Index the location of marking
    headerPos = data.indexOf('@');
    separatorPos = data.indexOf(';');
    endPos = data.indexOf('#');

    //Detection if the first data is stat?
    if (data.substring(headerPos + 1, headerPos + 5) == "stat") {
      //if Oke, store the stat data to stat variable
      stat = data.substring(headerPos + 6, separatorPos);
    }

    //Serial.println(data.substring(separatorPos + 1, separatorPos + 5));
    //Detection if the second data is time?
    if (data.substring(separatorPos + 1, separatorPos + 5) == "time") {
      //If oke, Store the time data to time variable
      time = data.substring(separatorPos + 6, endPos);
    }
    Serial.println("Time = " + time);
    waktu = time.toInt();
    menit = waktu / 60;
    detik = waktu % 60;
  }

  //Display menu based on stat data
  if (stat == "0") {
    homepage();
  } else if (stat == "1") {
    if (hold == 0) {
      mylcd.Fill_Screen(WHITE);
    }
    hold = 1;
    pengisianawal();
  } else if (stat == "2") {
    if (hold == 1) {
      mylcd.Fill_Screen(WHITE);
    }
    hold = 2;
    pencucian();
  } else if (stat == "3") {
    if (hold == 2) {
      mylcd.Fill_Screen(WHITE);
    }
    hold = 3;
    pengurasan1();
  } else if (stat == "4") {
    if (hold == 3) {
      mylcd.Fill_Screen(WHITE);
    }
    hold = 4;
    pengisian2();
  } else if (stat == "5") {
    if (hold == 4) {
      mylcd.Fill_Screen(WHITE);
    }
    hold = 5;
    bilas1();
  } else if (stat == "6") {
    if (hold == 5) {
      mylcd.Fill_Screen(WHITE);
    }
    hold = 6;
    pengurasan2();
  } else if (stat == "7") {
    if (hold == 6) {
      mylcd.Fill_Screen(WHITE);
    }
    hold = 7;
    pengisian3();
  } else if (stat == "8") {
    if (hold == 7) {
      mylcd.Fill_Screen(WHITE);
    }
    hold = 8;
    bilas2();
  } else if (stat == "9") {
    if (hold == 8) {
      mylcd.Fill_Screen(WHITE);
    }
    hold = 9;
    pengurasan3();
  } else if (stat == "10") {
    if (hold == 9) {
      mylcd.Fill_Screen(WHITE);
    }
    hold = 10;
    selesai();
  }
}
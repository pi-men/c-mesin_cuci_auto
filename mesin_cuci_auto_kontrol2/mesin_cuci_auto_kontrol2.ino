#include <SoftwareSerial.h>
#include <espnow.h>
#include <ESP8266WiFi.h>
#define SW D5
#define DT D6
#define CLK D7

// REPLACE WITH YOUR ESP RECEIVER'S MAC ADDRESS
uint8_t broadcastAddress1[] = { 0xE8, 0xDB, 0x84, 0xAE, 0xB5, 0x35 };
uint8_t broadcastAddress2[] = { 0x24, 0x6F, 0x28, 0x24, 0xD6, 0x50 };

typedef struct data_struct {
  int cmd;
  int stat;
  int time;
} data_struct;

data_struct data;

// Callback when data is sent
void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {
  char macStr[18];
  //Serial.print("Packet to:");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  //Serial.print(macStr);
  //Serial.print(" send status: ");
  // if (sendStatus == 0) {
  //   Serial.println("Delivery success");
  // } else {
  //   Serial.println("Delivery fail");
  // }
}

SoftwareSerial SerialCom(D1, D2);
int a = 1;
int delay1 = 1;

int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;

unsigned long prevMillis = 0;
const long interval = 1000;
int detik = 0;
int menit = 0;
int waktu = 0;
//Default detik
int waktuCuci = 180;
int waktuBilas = 180;
int waktuKuras = 300;
int waktuPengisian = 0;
int holdmenit = 0;
long stat = 0;
long cmd = 0;
int button;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  SerialCom.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();

  // Init ESP-NOW
  if (esp_now_init() != 0) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER);

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
  esp_now_register_send_cb(OnDataSent);

  // Register peer
  esp_now_add_peer(broadcastAddress1, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  esp_now_add_peer(broadcastAddress2, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);
  pinMode(CLK, INPUT);
  pinMode(DT, INPUT);
  pinMode(SW, INPUT_PULLUP);

  lastStateCLK = digitalRead(CLK);
}

void loop() {
  // put your main code here, to run repeatedly:
  unsigned long currentMillis = millis();

  currentStateCLK = digitalRead(CLK);
  if (currentStateCLK != lastStateCLK && currentStateCLK == 1) {
    if (digitalRead(DT) != currentStateCLK) {
      waktu++;
      currentDir = "CW";
    } else {
      waktu--;
      currentDir = "CCW";
    }
    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);
  }

  //Remember last CLK State
  lastStateCLK = currentStateCLK;

  //Read the button state
  int btnState = digitalRead(SW);

  //If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    //if 50ms have passed since last LOW pulse, it means that the
    //button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button Pressed!");
      button = 1;  // Kondisi tombol pertama kali ditekan
    }

    //Remember last button press event
    lastButtonPress = millis();
  }

  //Algorithm of Washing

  //Step 1 - tombol memulai Pengisian awal
  //====================================
  if (button == 1 && stat == 0) {
    stat = 1;
    button = 0;
    cmd = 1;
    //Serial.println("Proses pengisian Air");
  }
  //======================================

  //Step 2 - Air Penuh, Mulai mencuci
  if (button == 1 && stat == 1) {
    stat = 2;
    button = 0;
    waktu = waktuCuci;
    cmd = 0;
    Serial.println(waktuPengisian);
  }
  //------------------------------------

  //Timer Pewaktu

  if (currentMillis - prevMillis >= interval) {
    prevMillis = currentMillis;

    //Step 1: timer pengisian awal
    //=====================================
    if (stat == 1) {
      waktu++;
      menit = waktu / 60;
      detik = waktu % 60;
      waktuPengisian = waktu;
      Serial.println("Waktu:" + String(menit) + ":" + String(detik));
    }
    //======================================

    //Step 2: timer proses mencuci
    //=====================================
    if (stat == 2) {
      if (waktu == 0) {
        Serial.println("Timer Selesai");
        stat = 3;
        cmd = 2;
        //Store waktukurang data to waktu
        waktu = waktuKuras;
      } else {
      }

      waktu--;
      menit = waktu / 60;
      detik = waktu % 60;
      Serial.println("Waktu:" + String(menit) + ":" + String(detik));
    }
    //======================================

    //Step 3: timer proses pengurasan
    //=====================================
    if (stat == 3) {
      if (waktu == 0) {
        Serial.println("Timer Selesai");
        stat = 4;
        cmd = 1;
        //Store waktuPengisian data to holdmenit
        waktu = waktuPengisian;
      } else {
      }
      waktu--;
      menit = waktu / 60;
      detik = waktu % 60;
      Serial.println("Waktu:" + String(menit) + ":" + String(detik));
    }
    //======================================

    //Step 4: timer pengisian untuk bilas 1
    //=====================================
    if (stat == 4) {
      if (waktu== 0) {
        Serial.println("Timer Selesai");
        stat = 5;
        cmd = 0;
        //Store waktuBilas data to holdmenit
        waktu = waktuBilas;
      } else {
      }
      waktu--;
      menit = waktu / 60;
      detik = waktu % 60;
      Serial.println("Waktu:" + String(menit) + ":" + String(detik));
    }
    //======================================

    //Step 5: timer proses bilas 1
    //=====================================
    if (stat == 5) {
      if (waktu == 0) {
        Serial.println("Timer Selesai");
        stat = 6;
        cmd = 2;
        //Store waktuPengisian data to holdmenit
        waktu = waktuKuras;
      } else {
      }
      waktu--;
      menit = waktu / 60;
      detik = waktu % 60;
      Serial.println("Waktu:" + String(menit) + ":" + String(detik));
    }
    //======================================

    //Step 6: timer pengurasan 2
    //=====================================
    if (stat == 6) {
      if (waktu == 0) {
        Serial.println("Timer Selesai");
        stat = 7;
        cmd = 1;
        //Store waktuPengisian data to holdmenit
        holdmenit = waktuPengisian;
      } else {
      }
      waktu--;
      menit = waktu / 60;
      detik = waktu % 60;
      Serial.println("Waktu:" + String(menit) + ":" + String(detik));
    }
    //======================================

    //Step 7: timer pengisian untuk bilas 2
    //=====================================
    if (stat == 7) {
      if (waktu == 0) {
        Serial.println("Timer Selesai");
        stat = 8;
        cmd = 0;
        //Store waktuBilas data to holdmenit
        waktu = waktuBilas;
      } else {
      }
      waktu--;
      menit = waktu / 60;
      detik = waktu % 60;
      Serial.println("Waktu:" + String(menit) + ":" + String(detik));
    }
    //======================================

    //Step 8: timer bilas 2
    //=====================================
    if (stat == 8) {
      if (waktu == 0) {
        Serial.println("Timer Selesai");
        stat = 9;
        cmd = 2;
        //Store waktuPengisian data to holdmenit
        waktu = waktuKuras;
      } else {
      }
      waktu--;
      menit = waktu / 60;
      detik = waktu % 60;
      Serial.println("Waktu:" + String(menit) + ":" + String(detik));
    }
    //======================================

    //Step 9: timer pengurasan 3
    //=====================================
    if (stat == 9) {
      if (waktu == 0) {
        Serial.println("Timer Selesai");
        stat = 10;
        cmd = 0;
      } else {
      }
      waktu--;
      menit = waktu / 60;
      detik = waktu % 60;
      Serial.println("Waktu:" + String(menit) + ":" + String(detik));
    }
    //======================================

    // Set values to send
    data.cmd = cmd;
    data.stat = stat;
    data.time = waktu;

    // Send message via ESP-NOW
    esp_now_send(0, (uint8_t *)&data, sizeof(data));
  }

  delay1++;
  if (delay1 >= 100000) {
    delay1 = 0;
    SerialCom.println("@stat=" + String(stat) + ";time=" + String(waktu) + "#");
  }
}

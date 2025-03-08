//Main Board : ESP32
//Program for washing machine

#include <esp_now.h>
#include <WiFi.h>
#define spinCW 18
#define spinCCW 21
#define drain 19

unsigned long interval = 5000;
unsigned long currentTime = 0;
unsigned long prevTime = 0;

int count;
int countOff;
int condition = 1;

//variable to check if nothing coming data after minute
unsigned long interval2 = 1000;
unsigned long currentTime2 = 0;
unsigned long prevTime2 = 0;
unsigned long varIn;
unsigned long varCheck;

typedef struct data_struct {
  int cmd;
  int stat;
  int time;
} data_struct;

data_struct data;


void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&data, incomingData, sizeof(data));
  Serial.print("Bytes received: ");
  Serial.println(len);
  Serial.print("cmd: ");
  Serial.println(data.cmd);
  Serial.print("stat: ");
  Serial.println(data.stat);
  Serial.print("time: ");
  Serial.println(data.time);
  Serial.println();

  //Varible check
  varIn++;
}

void setup() {
  Serial.begin(9600);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
  // Register for a callback function that will be called when data is received
  esp_now_register_recv_cb(esp_now_recv_cb_t(OnDataRecv));
  pinMode(spinCW, OUTPUT);
  pinMode(spinCCW, OUTPUT);
  pinMode(drain, OUTPUT);
  digitalWrite(spinCW, HIGH);
  digitalWrite(spinCCW, LOW);
  digitalWrite(drain, HIGH);
  condition = 1;
}

void loop() {
  if (data.stat == 2 || data.stat == 5 || data.stat == 8) {
    currentTime = millis();
    if (currentTime - prevTime >= interval) {
      prevTime = currentTime;
      count++;
      if (count == 6 && condition == 1) {
        condition = 0;
        count = 0;
      } else {
      }

      if (count == 2 && condition == 0) {
        condition = 2;
        count = 0;
        countOff++;
      } else {
      }

      if (count == 6 && condition == 2) {
        condition = 0;
        count = 0;
      } else {
      }

      if (countOff == 2) {
        condition = 1;
        countOff = 0;
      } else {
      }
    }

    if (condition == 1) {
      //Serial.println("Putar kanan");
      digitalWrite(spinCCW, LOW);
      digitalWrite(spinCW, LOW);  // Active Low
    } else if (condition == 2) {
      //Serial.println("putar kiri");
      digitalWrite(spinCCW, HIGH);  //Active High
      digitalWrite(spinCW, HIGH);
    } else if (condition == 0) {
      //Serial.println("Berhenti");
      digitalWrite(spinCCW, LOW);
      digitalWrite(spinCW, HIGH);
    }
  } else {
    digitalWrite(spinCW, HIGH);
    digitalWrite(spinCCW, LOW);
  }

  if (data.cmd == 2) {
    digitalWrite(drain, LOW);
  } else {
    digitalWrite(drain, HIGH);
  }

  currentTime2 = millis();
  if (currentTime2 - prevTime2 >= interval2) {
    prevTime2 = currentTime2;
    if (varIn == varCheck) {
      digitalWrite(spinCW, HIGH);
      digitalWrite(spinCCW, LOW);
      data.stat = 0;
      //Serial.println("Data tidak masuk");

    } else {
      varCheck = varIn;
      //Serial.println("Data Masuk");
    }
    //varIn = 0;
  }
}
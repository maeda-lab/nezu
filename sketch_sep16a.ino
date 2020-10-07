#include <ESP32Servo.h>
#include <SD.h>
#include <SPIFFS.h>
#include <SPI.h>
enum { sd_sck = 18, sd_miso = 19, sd_mosi = 17, sd_ss = 5 };
const double VOLT = 3.3;
const int ANALOG_MAX = 4096;

const char* file_name = "/data.csv";
File file;
// 計測する回数
int MEASUREMENT_TIME = 100;
int cnt = 0;

void setup()
{
  SPI.end();
  SPI.begin(sd_sck, sd_miso, sd_mosi, sd_ss);
  Serial.println("setup start");
  pinMode(16, OUTPUT);
  digitalWrite(16, 1);
  Serial.begin(115200);
  if(!SD.begin(sd_ss, SPI)){
    Serial.println("Card Mount Failed");
    return;
  }else {
    Serial.println("Card Mount Successed");
    file = SD.open(file_name, FILE_APPEND);
    file.print("hoge");
    file.close();
  }
}

void loop() {
  if(cnt++ < MEASUREMENT_TIME) {
    float right_voltage = ((long)analogRead(34) * VOLT * 1000) / ANALOG_MAX;
    float left_voltage = ((long)analogRead(32) * VOLT * 1000) / ANALOG_MAX;
    Serial.print("right: ");
    Serial.print(right_voltage);
    Serial.print(" mV, left: ");
    Serial.print(left_voltage);
    Serial.println(" mV");
    write_file(right_voltage, left_voltage);
    delay(100);
  }
}

void write_file(float right_voltage, float left_voltage) {
  file = SD.open(file_name, FILE_APPEND);
  file.print(right_voltage);
  file.print(", ");
  file.print(left_voltage);
  file.println("");
}

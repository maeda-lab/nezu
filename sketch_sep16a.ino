#include <ESP32Servo.h>
#include <SD.h>
#include <SPIFFS.h>

const double VOLT = 3.3;
const int ANALOG_MAX = 4096;

File file;

Servo myservo;

int pos = 0;

int cnt = 0;

void setup() {
  Serial.begin( 9600 );     // シリアル通信を初期化する。通信速度は9600bps

  Serial.println( "Hello Arduino!" );   // 最初に1回だけメッセージを表示する

  file = SD.open("/data.csv", FILE_WRITE);
  if(!file) {
    Serial.println("file error");
  }
}

void loop() {
//  ファイルの書き込み
  if(cnt++ < 1000){
    uint8_t data[] = "hoge¥n";
    file.write(data, sizeof(data) / sizeof(data[0]));
    while(file.available()){
      Serial.print((char)file.read());
      delay(50);
    }
  }
  int reading = analogRead(34);
  float voltage = ((long)reading * VOLT * 1000) / ANALOG_MAX;
  Serial.print(voltage);
  Serial.println(" mV, ");
//  delay(15);
//  サーボモータを動かす部分
//  for(pos = 0; pos < 180; pos += 1) {
//    myservo.write(pos);
//    delay(15);
//  }
//  for(pos = 180; pos > 0; pos -=1 ) {
//    myservo.write(pos);
//    delay(15);
//  }

  
}

#include <ESP32Servo.h>
#include <SD.h>
#include <SPIFFS.h>
#include <SPI.h>

const double VOLT = 3.3;
const int ANALOG_MAX = 4096;

File fo;

Servo myservo;

int pos = 0;

int cnt = 0;

int cs_SD = 5;
const char* f_name ="/test1.txt";
int bangou = 0;
File myFile;
enum { sd_sck=18, sd_miso=19, sd_mosi = 17, sd_ss=5};

void setup() {
  SPI.end();
  SPI.begin(sd_sck, sd_miso, sd_mosi, sd_ss);
  Serial.begin( 9600 );     // シリアル通信を初期化する。通信速度は9600bps

  Serial.println( "Hello Arduino!" );   // 最初に1回だけメッセージを表示する

  SD.begin(sd_ss, SPI);
  Serial.println("SD_conect...");
  delay(10);
     
  if (!SD.begin(cs_SD, SPI, 1000000)){
    Serial.println("Card failed");
  }else { 
    Serial.println("card ok!");
    myFile = SD.open(f_name,FILE_APPEND);
    bangou++;
    if(myFile){
       myFile.println(""); 
       myFile.print(bangou);
       myFile.print(" : test kakikomi OK!");
       Serial.print("");
       Serial.print(bangou); 
       Serial.print(" : Card kakikomi ok");  
       myFile.close();
    }else{
      Serial.print(" : Card kakikomi shippai");   
    }
    
    delay(1000);
  }
}

void loop() {
  
//  ファイルの書き込み
//  if(cnt++ < 1000){
//    uint8_t data[] = "hoge¥n";
//    file.write(data, sizeof(data) / sizeof(data[0]));
//    while(file.available()){
////      Serial.print((char)file.read());
//      delay(50);
//    }
//  }
//  int reading = analogRead(34);
//  float voltage = ((long)reading * VOLT * 1000) / ANALOG_MAX;
//  Serial.print(voltage);
//  Serial.println(" mV, ");
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
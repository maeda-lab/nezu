#include <ESP32Servo.h>
#include <SD.h>
#include <SPIFFS.h>
#include <SPI.h>
#include <time.h>
#include <string>
enum { sd_sck = 18, sd_miso = 19, sd_mosi = 23, sd_ss = 5 };
const double VOLT = 3.3;
const int ANALOG_MAX = 4096;

const char* file_name = "/data.csv";
File file;
// 計測する回数
int MEASUREMENT_TIME = 300;
int cnt = 0;
bool isFinished = false;

Servo servo;

int test_led_pins[5]  = {12, 14, 27};
int led_count = 3;
int test_proc[20][2] = {
    {1, 80},
    {0, 40},
    {2, 70},
    {0, 30},
    {1, 10},
    {2, 100}
  };
int test_proc_count = 6;
int led_step_count = 0;
int current_proc = 0;

hw_timer_t* timer = NULL;

void setup()
{
  // シリアル通信初期化
  Serial.println("setup start");
  pinMode(16, OUTPUT);
  digitalWrite(16, 1);
  Serial.begin(115200);
  // SDカード読み込み初期化
  SPI.end();
  SPI.begin(sd_sck, sd_miso, sd_mosi, sd_ss);
  if(!SD.begin(sd_ss, SPI, 1000000)){
    Serial.println("Card Mount Failed");
    return;
  }else {
    Serial.println("Card Mount Successed");
    file = SD.open(file_name, FILE_APPEND);
    file.println("");
    file.println("start");
    file.println("left,right,lighting led pin");
    file.close();
  }
  // サーボモータ初期化
  servo.attach(15);
  // 実験用LED初期化
  for(int i = 0; i < led_count; i++) {
    pinMode(test_led_pins[i], OUTPUT);
  }
  timer = timerBegin(0, 80, true); // 80/80=1MHzでカウントアップ
}

void loop() {
  if(current_proc < test_proc_count) {
    if(led_step_count >= test_proc[current_proc][1]) {
      digitalWrite(test_led_pins[test_proc[current_proc][0]], LOW);
      led_step_count = 0;
      current_proc++;
      digitalWrite(test_led_pins[test_proc[current_proc][0]], HIGH);
    }
    if(led_step_count == 0) {
      digitalWrite(test_led_pins[test_proc[current_proc][0]], HIGH);
    }
    led_step_count++;
  }
  
  if(cnt++ < MEASUREMENT_TIME && !isFinished) {
    float right_voltage = ((long)analogRead(34) * VOLT * 1000) / ANALOG_MAX;
    float left_voltage = ((long)analogRead(32) * VOLT * 1000) / ANALOG_MAX;
//    uint64_t num = timerRead(timer);
//    uint32_t low = num % 0xFFFFFFFF; 
//    uint32_t high = (num >> 32) % 0xFFFFFFFF;
//  
//   Serial.print(low);
//   Serial.println(high); 

    Serial.print("right: ");
    Serial.print(right_voltage);
    Serial.print(" mV, left: ");
    Serial.print(left_voltage);
    Serial.print(" mV");
    servo.write(calc_angle(right_voltage, left_voltage));
    Serial.print(" angle: ");
    Serial.print(calc_angle(right_voltage, left_voltage));
    Serial.print(" lighting led pin: ");
    Serial.println(test_proc[current_proc][0]);
    write_file(right_voltage, left_voltage, test_proc[current_proc][0], timerRead(timer));
  }
  if (Serial.available()) {
    isFinished = true;
  }
}

int calc_angle(float right_voltage, float left_voltage) {
//  TODO: 計算式を変える
  double gain = 180.0 / 3300;
  return gain * (right_voltage + left_voltage) / 2;
}

void write_file(float right_voltage, float left_voltage, int lighting_led_pin, uint64_t timer_count) {
  file = SD.open(file_name, FILE_APPEND);
  if(file) {
    file.print(right_voltage);
    file.print(", ");
    file.print(left_voltage);
    file.print(", ");
    file.print(lighting_led_pin);
    file.print(",");
    uint32_t low = timer_count % 0xFFFFFFFF; 
    uint32_t high = (timer_count >> 32) % 0xFFFFFFFF;
    file.print(high);
    file.print(low);
    file.println("");

  } else {
    Serial.println("file error");
  }
  
  delay(20);
  file.close();
}

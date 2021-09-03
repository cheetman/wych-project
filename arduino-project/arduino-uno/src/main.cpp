#include <Arduino.h>

void setup() {
  Serial.begin(115200); //串口波特率配置
}

void loop() {
  // put your main code here, to run repeatedly:
  Serial.println("测试");
  delay(2000);
}
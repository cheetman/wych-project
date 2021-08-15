#include <Arduino.h>
#include <U8x8lib.h>
// #include <dht11.h>
#include "protothreads.h"
#include "pt-sem.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif


U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display
static struct pt_sem sem_LED;
pt ptBlink;
pt ptOut;
pt ptIn;

  



int blinkThread(struct pt* pt) {
  PT_BEGIN(pt);

  // Loop forever
  for(;;) {

    PT_SEM_WAIT(pt,&sem_LED); //LED有在用吗？

    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    time_t now = time(0);
    tm *ltm = localtime(&now);
    Serial.print(ltm->tm_sec);
    Serial.print(" ");
    Serial.print(millis ());
    Serial.println(" 灯亮");
    u8x8.print("灯亮");		
    PT_SLEEP(pt, 1000);
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    now = time(0);
    ltm = localtime(&now);
    Serial.print(ltm->tm_sec);
    Serial.print(" ");
    Serial.print(millis ());
    Serial.println(" 灯关");
    u8x8.print("灯关");		
    PT_SLEEP(pt, 1000);
    
    PT_SEM_SIGNAL(pt,&sem_LED);//用完了。
    PT_YIELD(pt); //看看别人要用么？

  }

  PT_END(pt);
}

int outThread(struct pt* pt) {
  PT_BEGIN(pt);

  // Loop forever
  for(;;) {
    time_t now = time(0);
    tm *ltm = localtime(&now);

    Serial.print(ltm->tm_sec);
    Serial.print(" ");
    Serial.print(millis ());
    Serial.print(" Hello world!\r\n");
    PT_SLEEP(pt,30000);
	  PT_YIELD(pt);
  }

  PT_END(pt);
}

String inString = "";
int inThread(struct pt* pt) {
  PT_BEGIN(pt);

  // Loop forever
  for(;;) {
    PT_WAIT_UNTIL(pt, Serial.available() > 0);
    int inChar = Serial.read();
    if (isalnum(inChar))
      inString += (char)inChar; 
    if (inChar == ' '||inChar=='\n') {
      // time_t now = time(0);
      // tm *ltm = localtime(&now);
      // Serial.print(ltm->tm_sec);
      // Serial.print(" ");
      // Serial.print(millis ());
      // Serial.print(" ");
      // Serial.println(inString);
      
      PT_SEM_WAIT(pt,&sem_LED);//我要用LED啊！
      inString = ""; 
      //抢到使用权了，虐5次
      digitalWrite(LED_BUILTIN,HIGH);
      Serial.print("闪烁");
      PT_SLEEP(pt,100);
      digitalWrite(LED_BUILTIN,LOW);
      Serial.print("闪烁");
      PT_SLEEP(pt,100);
      digitalWrite(LED_BUILTIN,HIGH);
      Serial.print("闪烁");
      PT_SLEEP(pt,100);
      digitalWrite(LED_BUILTIN,LOW);
      Serial.print("闪烁");
      PT_SLEEP(pt,100);
      digitalWrite(LED_BUILTIN,HIGH);
      Serial.print("闪烁");
      PT_SLEEP(pt,100);
      digitalWrite(LED_BUILTIN,LOW);
      Serial.print("闪烁");
      PT_SLEEP(pt,100);
      digitalWrite(LED_BUILTIN,HIGH);
      Serial.print("闪烁");
      PT_SLEEP(pt,100);
      digitalWrite(LED_BUILTIN,LOW);
      Serial.print("闪烁");
      PT_SLEEP(pt,100);
      digitalWrite(LED_BUILTIN,HIGH);
      Serial.print("闪烁");
      PT_SLEEP(pt,100);
      digitalWrite(LED_BUILTIN,LOW);
      Serial.print("闪烁");
      PT_SLEEP(pt,100);
      
      PT_SEM_SIGNAL(pt,&sem_LED); //归还LED使用权了
      
	    PT_YIELD(pt);
    }
  }

  PT_END(pt);
}




void setup() {
  Serial.begin(115200);//串口波特率配置
  u8x8.begin();

  PT_SEM_INIT(&sem_LED,1);

  PT_INIT(&ptBlink);
  PT_INIT(&ptOut);
  PT_INIT(&ptIn);
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
  
  PT_SCHEDULE(blinkThread(&ptBlink));
  PT_SCHEDULE(outThread(&ptOut));
  PT_SCHEDULE(inThread(&ptIn));

}
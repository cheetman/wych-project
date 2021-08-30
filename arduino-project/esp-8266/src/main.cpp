#include <Arduino.h>
// #include <U8x8lib.h>
#include <U8g2lib.h>
#include "protothreads.h"

#include "pt-sem.h"
#include "DHT.h"


#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include "LittleFS.h"



ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'
ESP8266WebServer esp8266_server(80);    // 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）      


#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#define DHTPIN 0
#define DHTTYPE DHT11

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display
// U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display
static struct pt_sem sem_LED;
pt ptBlink;
// pt ptOut;
pt ptIn;
pt ptLed8x8;
pt ptDht11;
DHT dht(DHTPIN, DHTTYPE);

char* Humidity = new char[40];  
char* Temperature = new char[40];  

void pre(void)
{

  
  u8g2.setFont(u8g2_font_unifont_t_chinese1);  // use chinese2 for all the glyphs of "你好世界"
  u8g2.setFontDirection(0);

}


int Dht11Thread(struct pt* pt) {
  PT_BEGIN(pt);
  // Loop forever
  for(;;) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t) ) {
       Serial.println(F("Failed to read from DHT sensor!"));
       strcpy(Humidity ,"Failed to read");
       strcpy(Temperature ,"from DHT sensor!");

    }else{
      Serial.print(F("Humidity: "));
      Serial.print(h);
      Serial.print(F("%  Temperature: "));
      Serial.print(t);
      Serial.println(F("°C "));

      char Humidityc[40] ;  
      sprintf(Humidityc, "Hum:%.1f%%",h);  
      strcpy(Humidity ,Humidityc);

      char Temperaturec[40] ;  
      sprintf(Temperaturec, "Tem:%.1fC",t);  
      strcpy(Temperature ,Temperaturec);
      
    }
    PT_SLEEP(pt,2000);
    PT_YIELD(pt); //看看别人要用么？
  }

  PT_END(pt);
}

int Led8x8Thread(struct pt* pt) {
  PT_BEGIN(pt);
  // Loop forever
  for(;;) {

    pre();
    // u8x8.drawString(0, 2, Humidity);
    // u8x8.drawString(0, 3, Temperature); 
    u8g2.clearBuffer();
    u8g2.setCursor(0, 15);

    u8g2.print(Humidity);
    u8g2.setCursor(0, 35);
    u8g2.print(Temperature);	
    
    u8g2.sendBuffer();
    Serial.println(Humidity);
    Serial.println(Temperature);
    PT_SLEEP(pt,2000);

    PT_YIELD(pt); //看看别人要用么？

  }

  PT_END(pt);
}

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
    PT_SLEEP(pt, 1000);
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    now = time(0);
    ltm = localtime(&now);
    Serial.print(ltm->tm_sec);
    Serial.print(" ");
    Serial.print(millis ());
    Serial.println(" 灯关");
    PT_SLEEP(pt, 1000);
    
    PT_SEM_SIGNAL(pt,&sem_LED);//用完了。
    PT_YIELD(pt); //看看别人要用么？

  }

  PT_END(pt);
}

// int outThread(struct pt* pt) {
//   PT_BEGIN(pt);

//   // Loop forever
//   for(;;) {
//     time_t now = time(0);
//     tm *ltm = localtime(&now);

//     Serial.print(ltm->tm_sec);
//     Serial.print(" ");
//     Serial.print(millis ());
//     Serial.print(" Hello world!\r\n");
//     PT_SLEEP(pt,30000);
// 	  PT_YIELD(pt);
//   }

//   PT_END(pt);
// }

String inString = "";
int inThread(struct pt* pt) {
  PT_BEGIN(pt);

  // Loop forever
  for(;;) {
    PT_WAIT_UNTIL(pt, Serial.available() > 0);
    int inChar = Serial.read();
    if (isalnum(inChar)){
      inString += (char)inChar; 
      Serial.print(inString);}
    if (inChar == ' '||inChar=='\n') {
      Serial.print("发送");
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
  PT_SEM_INIT(&sem_LED,1);
  PT_INIT(&ptBlink);
  // PT_INIT(&ptOut);
  PT_INIT(&ptIn);
  PT_INIT(&ptLed8x8);
  PT_INIT(&ptDht11);
  pinMode(LED_BUILTIN, OUTPUT);
  // u8x8.begin();
  dht.begin();
  u8g2.begin();
  u8g2.enableUTF8Print();

  wifiMulti.addAP("BFDA_Office", ""); // 将需要连接的一系列WiFi ID和密码输入这里
  Serial.println("Connecting ...");// 则尝试使用此处存储的密码进行连接。

  int i = 0;  
  while (wifiMulti.run() != WL_CONNECTED) { // 尝试进行wifi连接。
    delay(1000);
    Serial.print(i++); Serial.print('.');
  }

  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID());              // 通过串口监视器输出连接的WiFi名称
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());           // 通过串口监视器输出ESP8266-NodeMCU的IP

  struct FSInfo x;
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  LittleFS.info(x);
  Serial.println("全部的大小");
  Serial.println(x.totalBytes);
  Serial.println("已经使用的");
  Serial.println(x.usedBytes);
}

void loop() {
  

  PT_SCHEDULE(blinkThread(&ptBlink));
  // PT_SCHEDULE(outThread(&ptOut));
  PT_SCHEDULE(inThread(&ptIn));
  PT_SCHEDULE(Led8x8Thread(&ptLed8x8));
  PT_SCHEDULE(Dht11Thread(&ptDht11));

}
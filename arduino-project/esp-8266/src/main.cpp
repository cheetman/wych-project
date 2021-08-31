#include <Arduino.h>
// #include <U8x8lib.h>
#include <U8g2lib.h>
#include <U8g2wqy.h>
#include "protothreads.h"
#include "pt-sem.h"
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266WebServer.h>
#include "LittleFS.h"

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
// #define DHTPIN 0 // DHT传感器使用D3引脚(GPIO0)
#define DHTPIN D5 // DHT传感器使用D3引脚(GPIO0)
#define DHTTYPE DHT11

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // All Boards without Reset of the Display
// U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display
DHT dht(DHTPIN, DHTTYPE);
ESP8266WiFiMulti wifiMulti;     // 建立ESP8266WiFiMulti对象,对象名称是 'wifiMulti'
ESP8266WebServer esp8266_server(80);    // 建立网络服务器对象，该对象用于响应HTTP请求。监听端口（80）     

pt ptBlink;
// pt ptOut;
pt ptIn;
pt ptLed8x8;
pt ptDht11;
pt ptFlashBtn;

static struct pt_sem sem_LED;

StreamString stream;
String WIFI_SSID = "";
String WIFI_PASSWORD = "";
String WIFI_IP = "";
char* Humidity = new char[20];  
char* Temperature = new char[20];  
 

void pre(void)
{
  // u8g2.setFont(u8g2_font_unifont_t_chinese3);  // use chinese2 for all the glyphs of "你好世界"
  u8g2.setFont(u8g2_font_wqy12_t_gb2312a);
  u8g2.setFontDirection(0);
  
}


int Dht11Thread(struct pt* pt) {
  PT_BEGIN(pt);
  // Loop forever
  for(;;) {
    float h = dht.readHumidity();
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t) ) {
       Serial.println(F("温度传感器连接异常!"));
       strcpy(Humidity ,"传感器异常!");
       strcpy(Temperature ,"传感器异常!");

    }else{
      char Humidityc[40] ;  
      sprintf(Humidityc, "%.1f%%",h);  
      strcpy(Humidity ,Humidityc);

      char Temperaturec[40] ;  
      sprintf(Temperaturec, "%.1fC",t);  
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

    stream.clear();
    stream << "SSID: "  << WIFI_SSID;
    u8g2.clearBuffer();
    u8g2.setCursor(0, 10);
    u8g2.print(stream.c_str());

    stream.clear();
    stream << "IP: "  << WIFI_IP;
    u8g2.setCursor(0, 23);
    u8g2.print(stream.c_str());

    stream.clear();
    stream << "湿度:"  << Humidity;
    u8g2.setCursor(0, 36);
    u8g2.print(stream.c_str());

    stream.clear();
    stream << "温度:"  << Temperature;
    u8g2.setCursor(0, 49);
    u8g2.print(stream.c_str());	

    u8g2.sendBuffer();
    // Serial.println(Humidity);
    // Serial.println(Temperature);
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
      // 测试程序
      if((char)inChar == 't'){

        File file = LittleFS.open("/test.txt", "r");
        if(!file){
          Serial.println("Failed to open file for reading");
        }else{
          Serial.print("文件内容:");
          while(file.available()){
            Serial.write(file.read());
          }
          Serial.println("");
          file.close();
        }

        Dir dir = LittleFS.openDir("/");
        while (dir.next()) {
            Serial.printf("文件名：%s\r\n",dir.fileName().c_str());
            File f = dir.openFile("r");
            Serial.printf("文件大小：%d字节\r\n",f.size());
        }
      }


    inString += (char)inChar; 
    }
    if (inChar == ' '||inChar=='\n') {
      
      PT_SEM_WAIT(pt,&sem_LED);//我要用LED！
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


int FlashBtnThread(struct pt* pt) {
  PT_BEGIN(pt);
  // Loop forever
  for(;;) {
      
      PT_SEM_WAIT(pt,&sem_LED);//我要用LED！
      //抢到使用权了，虐5次
      int value = digitalRead(D3);
      Serial.print(value);
      PT_SLEEP(pt,100);
	    PT_YIELD(pt);
    
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
  PT_INIT(&ptFlashBtn);
  pinMode(LED_BUILTIN, OUTPUT);
  // u8x8.begin();
  dht.begin();
  u8g2.begin();
  u8g2.enableUTF8Print();

  wifiMulti.addAP("BFDA_Office","");
  wifiMulti.addAP("dfsdf","");

  pre();

  String message = "正在连接WIFI,请稍等..";
  Serial.println("");// 则尝试使用此处存储的密码进行连接。
  Serial.println(message);// 则尝试使用此处存储的密码进行连接。
  u8g2.clearBuffer();
  u8g2.setCursor(0, 15);
  u8g2.print(message);
  u8g2.sendBuffer();

  int i = 0;  
  while (wifiMulti.run() != WL_CONNECTED) { // 尝试进行wifi连接。
    delay(1000);
    Serial.print(i++); Serial.print('.');
  }

  Serial.println('\n');
  Serial.print("Connected to ");
  Serial.println(WiFi.SSID()); 
  WIFI_SSID = WiFi.SSID();
  Serial.print("IP address:");
  Serial.println(WiFi.localIP());           // 通过串口监视器输出ESP8266-NodeMCU的IP
  WIFI_IP = WiFi.localIP().toString();
  
  struct FSInfo x;
  if(!LittleFS.begin()){
    Serial.println("An Error has occurred while mounting LittleFS");
    return;
  }
  LittleFS.info(x);

  Serial.printf("闪存总容量:%d字节\r\n",x.totalBytes);
  Serial.printf("已使用:%d字节\r\n",x.usedBytes);
}

void loop() {
  PT_SCHEDULE(blinkThread(&ptBlink));
  // PT_SCHEDULE(outThread(&ptOut));
  PT_SCHEDULE(inThread(&ptIn));
  PT_SCHEDULE(Led8x8Thread(&ptLed8x8));
  PT_SCHEDULE(Dht11Thread(&ptDht11));
  PT_SCHEDULE(FlashBtnThread(&ptFlashBtn));

}
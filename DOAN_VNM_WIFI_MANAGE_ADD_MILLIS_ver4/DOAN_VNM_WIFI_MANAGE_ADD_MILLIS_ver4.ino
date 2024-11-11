#include <WiFi.h>
#include "time.h"
#include "sntp.h"
#include <WiFiManager.h>
#include "modalLib.h"
#include <ArduinoJson.h>
#include <FirebaseClient.h>
#include "OneButton.h"
#include <string.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h> 
#include <Adafruit_SSD1306.h>
#include "DHT.h"
#include <Robojax_L298N_DC_motor.h>

#define DHTTYPE DHT22  

#define SCREEN_WIDTH 128 // 
#define SCREEN_HEIGHT 64 // Độ phân giải của màn hình OLED

#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);



const unsigned char wifi_router [] = {
0x00, 0x00, 0x00, 0x00, 0xFF, 0x00, 0x03, 0xC3, 0xC0, 0x06, 0x00, 0x60, 0xCC, 0xFF, 0x33, 0x41,
0x81, 0x82, 0x43, 0x18, 0xC2, 0x60, 0x7E, 0x06, 0x20, 0xC3, 0x04, 0x30, 0x18, 0x0C, 0x10, 0x3C,
0x08, 0x18, 0x00, 0x18, 0x08, 0x00, 0x10, 0x3F, 0xFF, 0xFC, 0xFF, 0xFF, 0xFF, 0x80, 0x00, 0x01,
0x80, 0x02, 0x49, 0x80, 0x03, 0x49, 0x80, 0x02, 0x49, 0x80, 0x00, 0x01, 0xFF, 0xFF, 0xFF, 0x7F,
0xFF, 0xFE, 0x10, 0x00, 0x08, 0x00, 0x00, 0x00, 
};// cấu hình chữ lên màn hình OLED


const unsigned char wifi_connected [] = {
0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 0xC0,
0x00, 0x00, 0x1F, 0xF0, 0x00, 0x0F, 0xBF, 0xF8, 0x00, 0x7F, 0xBF, 0xF8, 0x01, 0xFF, 0x7F, 0xDC,
0x07, 0xFF, 0x7F, 0xBC, 0x1F, 0xF0, 0x77, 0x7C, 0x3F, 0x80, 0x7A, 0xFC, 0x3E, 0x0F, 0x7D, 0xFC,
0x3C, 0x7F, 0xBF, 0xF8, 0x00, 0xFF, 0xBF, 0xF8, 0x03, 0xFF, 0xDF, 0xF0, 0x03, 0xF0, 0x07, 0xC0,
0x03, 0xC0, 0x00, 0x00, 0x00, 0x0F, 0xF0, 0x00, 0x00, 0x1F, 0xF8, 0x00, 0x00, 0x3F, 0xFC, 0x00,
0x00, 0x3E, 0x7C, 0x00, 0x00, 0x00, 0x10, 0x00, 0x00, 0x01, 0x80, 0x00, 0x00, 0x07, 0xE0, 0x00,
0x00, 0x07, 0xE0, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x07, 0xE0, 0x00, 0x00, 0x07, 0xE0, 0x00,
0x00, 0x03, 0xC0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};// cấu hình chữ lên màn hình OLED

const char* ntpServer1 = "pool.ntp.org";// thư viện lấy thời gian 
const char* ntpServer2 = "time.nist.gov";// thư viện lấy thời gian 
const long  gmtOffset_sec = 25200;// giờ châu Á
const int   daylightOffset_sec = 0;// set có lệch ngày hay k?

#define WIFI_SSID "TUANWIFI69"
#define WIFI_PASSWORD "11111111"

// Khóa API_KEY từ bảng điều khiển của Firebase
#define API_KEY "AIzaSyAv69NX_aeKcU0nipezrw0ZXPuZZRlERPw"

// User Email and password that already registerd or added in your project.
#define USER_EMAIL "tuando02@gmail.com"
#define USER_PASSWORD "123456"
#define DATABASE_URL "https://vungocminhdatn-default-rtdb.firebaseio.com/"

void asyncCB(AsyncResult &aResult); // Thư viện callback đọc và ghi dữ liệu từ Realtime Database

void printResult(AsyncResult &aResult);

DefaultNetwork network; // Khởi tạo với tham số boolean để bật/tắt kết nối mạng

UserAuth user_auth(API_KEY, USER_EMAIL, USER_PASSWORD); // Đăng nhập Firebase

FirebaseApp app;

#if defined(ESP32) || defined(ESP8266) || defined(ARDUINO_RASPBERRY_PI_PICO_W)
#include <WiFiClientSecure.h>
WiFiClientSecure ssl_client1, ssl_client2;


#elif defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_UNOWIFIR4) || defined(ARDUINO_GIGA) || defined(ARDUINO_PORTENTA_C33) || defined(ARDUINO_NANO_RP2040_CONNECT)
#include <WiFiSSLClient.h>
WiFiSSLClient ssl_client1, ssl_client2;
#endif

// In case the keyword AsyncClient using in this example was ambigous and used by other library, you can change
// it with other name with keyword "using" or use the class name AsyncClientClass directly.

using AsyncClient = AsyncClientClass;

AsyncClient aClient(ssl_client1, getNetwork(network));
AsyncClient aClient2(ssl_client2, getNetwork(network));

RealtimeDatabase Database;

AsyncResult aResult_no_callback;

unsigned long ms = 0;

bool henGioBom = false;
bool henGioDen = false;
bool henGioMaiChe = false;
bool henGioPhunSuong = false;

bool bomAuto;
bool denAuto;
bool maiCheAuto;
bool phunSuongAuto;
// biến lưu lại trạng thái đèn để khi tắt auto sẽ trở lại trạng thái này
bool bomState;
bool denState;
bool maiCheState;
bool phunSuongState; 

MyTime timeBomOn;
MyTime timeBomOff;

MyTime timeDenOn;
MyTime timeDenOff;

MyTime timeMaiCheOn;
MyTime timeMaiCheOff;

MyTime timePhunSuongOn;
MyTime timePhunSuongOff;

MyTime realTime;


#define DEN_PIN 26
#define MAI_CHE_PIN 14


#define LDR_PIN 35
#define DHT22_PIN 32
#define SM_PIN 33 // do am dat
#define RAIN_PIN 34

DHT dht(DHT22_PIN, DHTTYPE);

#define BOM_BTN 27
#define DEN_BTN 13
#define PHUN_SUONG_BTN 17
#define MAI_CHE_BTN 4


// motor 1 settings phun suong
#define CHA 0
#define ENA 19 // this pin must be PWM enabled pin if Arduino board is used
#define IN1 23
#define IN2 5

// motor 2 settings bom
#define IN3 15
#define IN4 16
#define ENB 18// this pin must be PWM enabled pin if Arduino board is used
#define CHB 1

const int CCW = 2; // do not change
const int CW  = 1; // do not change

#define motor1 1 // do not change bom
#define motor2 2 // do not change

Robojax_L298N_DC_motor robot(IN1, IN2, ENA, CHA,  IN3, IN4, ENB, CHB);

OneButton bomBTN(BOM_BTN, true);
OneButton denBTN(DEN_BTN, true);
OneButton phunSuongBTN(PHUN_SUONG_BTN, true);
OneButton maiCheBTN(MAI_CHE_BTN, true);

// lưu trữ dữ liệu đọc từ cảm biến
bool rainValue; // mua 1 ko mua 0
int ldrValue; // trả về giá trị cảm biến ánh sang 
int soilMoistureValue; // giá trị độ ẩm đất
int humidValue; // độ ẩm môi trường
int tempValue;

int nguongDoAmDat;
int nguongDoAmMT;
int tocDoBom;
int tocDoPhun;
int doNhay;

bool initData = false;
unsigned long ms2 = 0;
unsigned long ms3 = 0;
unsigned long ms4 = 0;


void toggleBom() {
  if (!bomAuto) {
        if (bomState) {
          bomState = false;
          robot.rotate(motor1, 0, CW);
          // Database.set<bool>(aClient, "/device/bom_tuoi_cay/nguon", false, asyncCB);
        } else {
          bomState = true;
          if (tocDoBom == 1) {
            robot.rotate(motor1, 80, CW);
          } else if (tocDoBom == 2) {
            robot.rotate(motor1, 90, CW);
          } else {
            robot.rotate(motor1, 100, CW);
          }
          // Database.set<bool>(aClient, "/device/bom_tuoi_cay/nguon", true, asyncCB);
        }
      }
}

void toggleDen() {
  if (!denAuto) {
        if (denState) {
          denState = false;
          digitalWrite(DEN_PIN, LOW);
          // Database.set<bool>(aClient2, "/device/den_quang_hop/nguon", false, aResult_no_callback);
          // Database.set<bool>(aClient2, "/device/den_quang_hop/nguon", false, asyncCB);
        } else {
          denState = true;
          digitalWrite(DEN_PIN, HIGH);
          // Database.set<bool>(aClient2, "/device/den_quang_hop/nguon", true, aResult_no_callback);
          // Database.set<bool>(aClient2, "/device/den_quang_hop/nguon", true, asyncCB);
        }
      }
}

void togglePhunSuong() {
  if (!phunSuongAuto) {
        if (phunSuongState) {
          phunSuongState = false;
          robot.rotate(motor2, 0, CW);
          // Database.set<bool>(aClient, "/device/phun_suong/nguon", false, asyncCB);
        } else {
          phunSuongState = true;
          if (tocDoPhun == 1) {
            robot.rotate(motor2, 80, CW);
          } else if (tocDoPhun == 2) {
            robot.rotate(motor2, 90, CW);
          } else {
            robot.rotate(motor2, 100, CW);
          }
          // Database.set<bool>(aClient, "/device/phun_suong/nguon", true, asyncCB);
        }
      }
}

void toggleMaiChe() {
  if (!maiCheAuto) {
        if (maiCheState) {
          maiCheState = false;
          digitalWrite(MAI_CHE_PIN, LOW);
          // Database.set<bool>(aClient, "/device/mai_che/nguon", false, asyncCB);
        } else {
          maiCheState = true;
          digitalWrite(MAI_CHE_PIN, HIGH);
          // Database.set<bool>(aClient, "/device/mai_che/nguon", true, asyncCB);
        }
      }
}

void setup()
{
    Serial.begin(115200);
    dht.begin();
    if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
      Serial.println(F("SSD1306 allocation failed"));
      for(;;); // Don't proceed, loop forever
    }

    pinMode(DEN_PIN, OUTPUT);
    pinMode(MAI_CHE_PIN, OUTPUT);

    pinMode(RAIN_PIN, INPUT);

    digitalWrite(DEN_PIN, LOW);
    digitalWrite(MAI_CHE_PIN, LOW);    

    bomBTN.attachClick(toggleBom);
    denBTN.attachClick(toggleDen);
    phunSuongBTN.attachClick(togglePhunSuong);
    maiCheBTN.attachClick(toggleMaiChe);
    
    sntp_set_time_sync_notification_cb( timeavailable );
    sntp_servermode_dhcp(1);
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer1, ntpServer2);


    wifiConnect();
    

    Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);

    Serial.println("Initializing app...");

    #if defined(ESP32) || defined(ESP8266) || defined(PICO_RP2040)
        ssl_client1.setInsecure();
        ssl_client2.setInsecure();
    #if defined(ESP8266)
    ssl_client1.setBufferSizes(4096, 1024);
    ssl_client2.setBufferSizes(4096, 1024);

    #endif
    #endif

    app.setCallback(asyncCB);

    // The async client used for auth task should not use for stream task
    initializeApp(aClient2, app, getAuth(user_auth));

    // Waits for app to be authenticated.
    // For asynchronous operation, this blocking wait can be ignored by calling app.loop() in loop().
    ms = millis();
    while (app.isInitialized() && !app.ready() && millis() - ms < 120 * 1000)
        ;

    app.getApp<RealtimeDatabase>(Database);

    Database.url(DATABASE_URL);

    Database.get(aClient, "/device", asyncCB, true /* SSE mode (HTTP Streaming) */);

}

void loop()
{
    if (millis() - ms2 > 2000) { // cứ 4s lại lấy thời gian thực
      ms2 = millis();
      getTime();
      getSensorValue();
    }


    bomBTN.tick();
    denBTN.tick();
    phunSuongBTN.tick();
    maiCheBTN.tick();

    app.loop();
    Database.loop();
    
    if (millis() - ms4 > 1000) {
      ms4 = millis();
      if (initData) { // để chắc chắn các giá trị trên firebase được khởi tạo

        displayOLED(realTime.hour, realTime.min, tempValue, humidValue, 
        soilMoistureValue, rainValue, bomState, maiCheState, denState, phunSuongState);


        if (bomAuto) {
          if (soilMoistureValue < nguongDoAmDat) {  // THÊM CHỨC NĂNG CHỈNH TỐC ĐỘ BƠM THEO ĐỘ ẨM ĐẤT 
              if (!bomState) { // nếu bơm đang tắt thì mới bật 
                  bomState = true;
                  if (tocDoBom == 1) {
                    robot.rotate(motor1, 80, CW);
                  } else if (tocDoBom == 2) {
                    robot.rotate(motor1, 90, CW);
                  } else {
                    robot.rotate(motor1, 100, CW);
                  }
                  // Database.set<bool>(aClient, "/device/bom_tuoi_cay/nguon", true, asyncCB);
              }

          } else {
            if (bomState) { // nếu bơm đang bật thì mới tắt
                  bomState = false;
                  robot.rotate(motor1, 0, CW);
                  // Database.set<bool>(aClient, "/device/bom_tuoi_cay/nguon", false, asyncCB);
              }
          }
        }

        if (denAuto) {
          int nguong_do_nhay;
          if (doNhay == 1) { 
            nguong_do_nhay = 2000;
          } 

          if (doNhay == 2) {  
            nguong_do_nhay = 3000;
          } 

          if (doNhay == 3) {  
            nguong_do_nhay = 4000;
          } 

          if (ldrValue > nguong_do_nhay) { 
              if (!denState) { // t 
                  denState = true;
                  digitalWrite(DEN_PIN, HIGH);
                  // Database.set<bool>(aClient, "/device/den_quang_hop/nguon", true, asyncCB);
              } 
          } else {
              if (denState) { // t 
                  denState = false;
                  digitalWrite(DEN_PIN, LOW);
                  // Database.set<bool>(aClient, "/device/den_quang_hop/nguon", false, asyncCB);
              } 
          }
        }

        if (maiCheAuto) {
            if (!rainValue) {  // có mưa
              if (!maiCheState) { // t 
                  maiCheState = true;
                  digitalWrite(MAI_CHE_PIN, HIGH);
                  // Database.set<bool>(aClient, "/device/mai_che/nguon", true, asyncCB);
              } // đóng mái che 
            } else {
              if (maiCheState) { // t 
                  maiCheState = false;
                  digitalWrite(MAI_CHE_PIN, LOW);
                  // Database.set<bool>(aClient, "/device/mai_che/nguon", false, asyncCB);
              } 
            } // mở mái che
        }

        if (phunSuongAuto) {
            if (humidValue < nguongDoAmMT) {  // THÊM CHỨC NĂNG CHỈNH TỐC ĐỘ BƠM THEO ĐỘ ẨM ĐẤT 
              if (!phunSuongState) { // t 
                  phunSuongState = true;
                  if (tocDoPhun == 1) {
                    robot.rotate(motor2, 80, CW);
                  } else if (tocDoPhun == 2) {
                    robot.rotate(motor2, 90, CW);
                  } else {
                    robot.rotate(motor2, 100, CW);
                  }
                  // Database.set<bool>(aClient, "/device/phun_suong/nguon", true, asyncCB);
              }
            } else {
              if (phunSuongState) { // t 
                  phunSuongState = false;
                  robot.rotate(motor2, 0, CW);
                  // Database.set<bool>(aClient, "/device/phun_suong/nguon", false, asyncCB);
              }
            }
        }

        if (henGioBom) // nếu đang hẹn giờ 
        {
          if (realTime.hour == timeBomOn.hour && realTime.min == timeBomOn.min) {
            Serial.println("dung gio bat bom !");
            if (!bomState) { // nếu đang tắt thì mới bật còn bật rồi thì ko cần :))
              bomState = true;
              // Database.set<bool>(aClient2, "/device/bom_tuoi_cay/nguon", true, asyncCB);
              if (tocDoBom == 1) {
                robot.rotate(motor1, 80, CW);
              } else if (tocDoBom == 2) {
                robot.rotate(motor1, 90, CW);
              } else {
                robot.rotate(motor1, 100, CW);
              }
              // Database.set<bool>(aClient, "/device/bom_tuoi_cay/nguon", true, asyncCB);
            }
          } 
          if (realTime.hour == timeBomOff.hour && realTime.min == timeBomOff.min) {
            if (bomState) { 
              bomState = false;
              // Database.set<bool>(aClient2, "/device/bom_tuoi_cay/nguon", false, asyncCB);
              robot.rotate(motor1, 0, CW);
              // Database.set<bool>(aClient, "/device/bom_tuoi_cay/nguon", false, asyncCB);
            }
          } 
        }
        if (henGioDen) // nếu đang hẹn giờ
        {
          if (realTime.hour == timeDenOn.hour && realTime.min == timeDenOn.min) {
            if (!denState) { 
                  denState = true;
                  // Database.set<bool>(aClient, "/device/den_quang_hop/nguon", true, asyncCB);
                  digitalWrite(DEN_PIN, HIGH);
              } 
          } 
          if (realTime.hour == timeDenOff.hour && realTime.min == timeDenOff.min) {
            if (denState) { // t 
                  denState = false;
                  // Database.set<bool>(aClient, "/device/den_quang_hop/nguon", false, asyncCB);
                  digitalWrite(DEN_PIN, LOW);
              } 
          } 
        }
        if (henGioMaiChe) // nếu đang hẹn giờ
        {
          if (realTime.hour == timeMaiCheOn.hour && realTime.min == timeMaiCheOn.min) {
            if (!maiCheState) { // t 
                  maiCheState = true;
                  // Database.set<bool>(aClient, "/device/mai_che/nguon", true, asyncCB);
                  digitalWrite(MAI_CHE_PIN, HIGH);
              } 
          } 
          if (realTime.hour == timeMaiCheOff.hour && realTime.min == timeMaiCheOff.min) {
            if (maiCheState) { // t 
                  maiCheState = false;
                  // Database.set<bool>(aClient, "/device/mai_che/nguon", false, asyncCB);
                  digitalWrite(MAI_CHE_PIN, LOW);
              } 
          } 
        }
        if (henGioPhunSuong) // nếu đang hẹn giờ
        {
          if (realTime.hour == timePhunSuongOn.hour && realTime.min == timePhunSuongOn.min) {
            if (!phunSuongState) { // t 
                  phunSuongState = true;
                  // Database.set<bool>(aClient, "/device/phun_suong/nguon", true, asyncCB);
                  if (tocDoPhun == 1) {
                    robot.rotate(motor2, 80, CW);
                  } else if (tocDoPhun == 2) {
                    robot.rotate(motor2, 90, CW);
                  } else {
                    robot.rotate(motor2, 100, CW);
                  }
              }
          } 
          if (realTime.hour == timePhunSuongOff.hour && realTime.min == timePhunSuongOff.min) {
            if (phunSuongState) { // t 
                  phunSuongState = false;
                  // Database.set<bool>(aClient, "/device/phun_suong/nguon", false, asyncCB);
                  robot.rotate(motor2, 0, CW);
              }
          } 
        }
    } else {
      display.clearDisplay();
      display.setTextSize(1);             // Normal 1:1 pixel scale
      display.setTextColor(WHITE);        // Draw white text
      display.setCursor(10, 12);             // Start at top-left corner
      display.print(F("Loading data..."));
      display.display();
    }
  }   
}

void wifiConnect() {
  display.clearDisplay();
  display.setTextSize(1);             // Normal 1:1 pixel scale
  display.setTextColor(WHITE);        // Draw white text
  display.setCursor(10, 12);             // Start at top-left corner
  display.print(F("Configuring Wifi... \n"));
  display.drawBitmap( 90, 28, wifi_router, 24, 24, WHITE);
  display.display();
  WiFiManager wm;
  // wm.resetSettings();
  bool res;
  res = wm.autoConnect("VuonTuDong");

  if(!res) {
      Serial.println("Failed to connect");
      // ESP.restart();
  } 
  else {
      //if you get here you have connected to the WiFi    
      display.clearDisplay();
      display.drawBitmap( 48, 0, wifi_connected, 32, 32, WHITE);
      display.display();
      Serial.println(wm.getWiFiSSID());
      Serial.println(wm.getConfigPortalSSID());
      delay(500);
      display.clearDisplay();
      display.setTextSize(1);             // Normal 1:1 pixel scale
      display.setTextColor(WHITE);        // Draw white text
      display.setCursor(10, 12);             // Start at top-left corner
      display.print(F("Loading data..."));
      display.display();
  }
}

void getTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("No time available (yet)");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
  realTime.hour = timeinfo.tm_hour;
  realTime.min = timeinfo.tm_min;
  // displayTime(realTime.hour, realTime.min);
}

void displayTime(int hour, int min) {
  if (hour < 10) {
    display.setCursor(6, 0);
  } else {
    display.setCursor(0, 0);
  }
  
  display.print(hour);
  display.setCursor(11, 0);
  display.print(":");
  display.setCursor(16, 0);
  display.print(min);
  display.display();
}

void timeavailable(struct timeval *t)
{
  Serial.println("Got time adjustment from NTP!");
  getTime();
}

void asyncCB(AsyncResult &aResult)
{
    printResult(aResult);
}

void printResult(AsyncResult &aResult)
{
    if (aResult.available())
    {
        RealtimeDatabaseResult &RTDB = aResult.to<RealtimeDatabaseResult>();
        Serial.println("hehe");
        if (RTDB.isStream())
        {
            Serial.println("----------------------------");
            Firebase.printf("task: %s\n", aResult.uid().c_str());
            Firebase.printf("event: %s\n", RTDB.event().c_str());
            Firebase.printf("path: %s\n", RTDB.dataPath().c_str());
            Firebase.printf("data: %s\n", RTDB.to<const char *>());
            Firebase.printf("type: %d\n", RTDB.type());
            String path = RTDB.dataPath();

            // path = "" là đường dẫn firebase chính
            if (!initData) {
              if (path == "/") { 
                if (strcmp(RTDB.to<const char *>(), "null") != 0) { // khác null mới lấy dữ liệu
                JsonDocument doc;
                DeserializationError error = deserializeJson(doc, RTDB.to<const char *>());

                // ----------- BƠM ---------------------
                JsonObject bom_json = doc["bom_tuoi_cay"];
                bomState = bom_json["nguon"];
                bomAuto = bom_json["auto"];
                nguongDoAmDat = bom_json["nguong_do_am_dat"];
                tocDoBom = bom_json["toc_do_bom"];


                // if (tocDoBom == 1) {
                //   analogWrite(1, 200);
                // } else if (tocDoBom == 2) {
                //   analogWrite(1, 220);
                // } else {
                //   analogWrite(1, 255);
                // }
                
                // CHẾ ĐỘ AUTO
                if (bom_json["auto"]) {  
                  if (soilMoistureValue < bom_json["nguong_do_am_dat"]) {  // THÊM CHỨC NĂNG CHỈNH TỐC ĐỘ BƠM THEO ĐỘ ẨM ĐẤT 
                    if (tocDoBom == 1) {
                      robot.rotate(motor1, 80, CW);
                    } else if (tocDoBom == 2) {
                      robot.rotate(motor1, 90, CW);
                    } else {
                      robot.rotate(motor1, 100, CW);
                    }
                  } else {
                    robot.rotate(motor1, 0, CW);
                  }
                  
                } else {  // CHẾ ĐỘ THỦ CÔNG
                    if (bom_json["nguon"]) { // bom_json["nguon"] = true => bật
                    
                      if (tocDoBom == 1) {
                        robot.rotate(motor1, 80, CW);
                      } else if (tocDoBom == 2) {
                        robot.rotate(motor1, 90, CW);
                      } else {
                        robot.rotate(motor1, 100, CW);
                      }
                    } else {
                      robot.rotate(motor1, 0, CW);
                    }
                }

                // CHẾ ĐỘ HẸN GIỜ
                henGioBom = bom_json["hen_gio"]; // GÁN TRẠNG THÁI ĐỂ SAU SỬ DỤNG 
                for (JsonPair thoi_gian_item : bom_json["thoi_gian"].as<JsonObject>()) {
                  const char* thoi_gian_item_key = thoi_gian_item.key().c_str(); // "bat_bom", ...
                  if (strcmp(thoi_gian_item_key, "bat_bom") == 0) {
                    timeBomOn.init(thoi_gian_item.value()["hour"], thoi_gian_item.value()["min"]);
                  } else {
                    timeBomOff.init(thoi_gian_item.value()["hour"], thoi_gian_item.value()["min"]);
                  }
                } 

                
                // ----------------- ĐÈN ------------------------
                JsonObject den_json = doc["den_quang_hop"];
                denState = den_json["nguon"];
                denAuto = den_json["auto"];
                doNhay = den_json["do_nhay"];
                if (den_json["auto"]) {  
                  int nguong_do_nhay;
                  if (den_json["do_nhay"] == 1) { 
                    nguong_do_nhay = 600;
                  } 

                  if (den_json["do_nhay"] == 2) {  
                    nguong_do_nhay = 800;
                  } 

                  if (den_json["do_nhay"] == 3) {  
                    nguong_do_nhay = 1000;
                  } 

                  if (ldrValue > nguong_do_nhay) { 
                      digitalWrite(DEN_PIN, HIGH);
                  } else {
                      digitalWrite(DEN_PIN, LOW);
                  }
                  
                } else {  // CHẾ ĐỘ THỦ CÔNG
                    if (den_json["nguon"]) { // den_json["nguon"] = true => bật
                      digitalWrite(DEN_PIN, HIGH);
                    } else digitalWrite(DEN_PIN, LOW);
                }


                // CHẾ ĐỘ HẸN GIỜ
                henGioDen = den_json["hen_gio"]; // GÁN TRẠNG THÁI ĐỂ SAU SỬ DỤNG 
                for (JsonPair thoi_gian_item : den_json["thoi_gian"].as<JsonObject>()) {
                  const char* thoi_gian_item_key = thoi_gian_item.key().c_str(); // "bat_den", ...
                  if (strcmp(thoi_gian_item_key, "bat_den") == 0) {
                    timeDenOn.init(thoi_gian_item.value()["hour"], thoi_gian_item.value()["min"]);
                  } else {
                    timeDenOff.init(thoi_gian_item.value()["hour"], thoi_gian_item.value()["min"]);
                  }
                } 


                // ------------------ MÁI CHE -------------------
                JsonObject mai_che_json = doc["mai_che"];
                maiCheState = mai_che_json["nguon"];
                maiCheAuto = mai_che_json["auto"];
                if (mai_che_json["auto"]) {  
                  if (rainValue) {  // có mưa
                    digitalWrite(MAI_CHE_PIN, HIGH); // đóng mái che 
                  } else digitalWrite(MAI_CHE_PIN, LOW); // mở mái che
                  
                } else {  // CHẾ ĐỘ THỦ CÔNG
                    if (mai_che_json["nguon"]) { // mai_che_json["nguon"] = true => bật
                      digitalWrite(MAI_CHE_PIN, HIGH);
                    } else digitalWrite(MAI_CHE_PIN, LOW);
                }

                // CHẾ ĐỘ HẸN GIỜ
                henGioMaiChe = mai_che_json["hen_gio"]; // GÁN TRẠNG THÁI ĐỂ SAU SỬ DỤNG 
                for (JsonPair thoi_gian_item : mai_che_json["thoi_gian"].as<JsonObject>()) {
                  const char* thoi_gian_item_key = thoi_gian_item.key().c_str(); // "bat_bom", ...
                  if (strcmp(thoi_gian_item_key, "dong_mai_che") == 0) {  
                    timeMaiCheOn.init(thoi_gian_item.value()["hour"], thoi_gian_item.value()["min"]);  // ON LÀ ĐÓNG MÁI CHE
                  } else {
                    timeMaiCheOff.init(thoi_gian_item.value()["hour"], thoi_gian_item.value()["min"]);
                  }
                } 


                // ------------------ PHUN SƯƠNG ------------------------
                JsonObject phun_suong_json = doc["phun_suong"];
                phunSuongState = phun_suong_json["nguon"];
                phunSuongAuto = phun_suong_json["auto"];
                nguongDoAmMT = phun_suong_json["nguong_do_am_mt"];
                tocDoPhun = phun_suong_json["toc_do_phun"];

                // if (tocDoPhun == 1) {
                //   analogWrite(3, 200);
                // } else if (tocDoPhun == 2) {
                //   analogWrite(3, 220);
                // } else {
                //   analogWrite(3, 255);
                // }

                // CHẾ ĐỘ AUTO
                if (phun_suong_json["auto"]) {  
                  if (humidValue < phun_suong_json["nguong_do_am_mt"]) {  // THÊM CHỨC NĂNG CHỈNH TỐC ĐỘ BƠM THEO ĐỘ ẨM ĐẤT 
                    if (tocDoPhun == 1) {
                      robot.rotate(motor2, 80, CW);
                    } else if (tocDoPhun == 2) {
                      robot.rotate(motor2, 90, CW);
                    } else {
                      robot.rotate(motor2, 100, CW);
                    }
                  } else robot.rotate(motor2, 0, CW);
                  
                } else {  // CHẾ ĐỘ THỦ CÔNG
                    if (phun_suong_json["nguon"]) { // phun_suong_json["nguon"] = true => bật
                      if (tocDoPhun == 1) {
                        robot.rotate(motor2, 80, CW);
                      } else if (tocDoPhun == 2) {
                        robot.rotate(motor2, 90, CW);
                      } else {
                        robot.rotate(motor2, 100, CW);
                      }
                    } else robot.rotate(motor2, 0, CW);
                }

                // CHẾ ĐỘ HẸN GIỜ
                henGioPhunSuong = phun_suong_json["hen_gio"]; // GÁN TRẠNG THÁI ĐỂ SAU SỬ DỤNG 
                for (JsonPair thoi_gian_item : phun_suong_json["thoi_gian"].as<JsonObject>()) {
                  const char* thoi_gian_item_key = thoi_gian_item.key().c_str(); // "bat_bom", ...
                  if (strcmp(thoi_gian_item_key, "bat_phun") == 0) {
                    timePhunSuongOn.init(thoi_gian_item.value()["hour"], thoi_gian_item.value()["min"]);
                  } else {
                    timePhunSuongOff.init(thoi_gian_item.value()["hour"], thoi_gian_item.value()["min"]);
                  }
                } 

                initData = true;
                // Serial.println("Init dữ liệu thành công !");
              }
              
            } 
            }
            

            else {

              // BƠM 
              if (path == "/bom_tuoi_cay/auto") {
                Serial.println("Bơm tưới cây auto thay đổi !");
                if (RTDB.to<bool>()) {
                  bomAuto = true;
                  Serial.println("Bật auto !");
                } else {
                  bomAuto = false;
                  Serial.println("Tắt auto !");
                  if (bomState) {  // CẬP NHẬT LẠI TRANG THÁI CŨ TRƯỚC AUTO
                    if (tocDoBom == 1) {
                      robot.rotate(motor1, 80, CW);
                    } else if (tocDoBom == 2) {
                      robot.rotate(motor1, 90, CW);
                    } else {
                      robot.rotate(motor1, 100, CW);
                    }
                    
                  } else {
                    robot.rotate(motor1, 0, CW);
                  }
                  // cap nhat lai trang thai
                }
              }
              if (path == "/bom_tuoi_cay/nguon") {
                // Serial.println("Bơm tưới cây auto thay đổi !");
                if (RTDB.to<bool>()) {
                  bomState = true;
                  Serial.println("bat bom");
                  if (tocDoBom == 1) {
                      robot.rotate(motor1, 80, CW);
                    } else if (tocDoBom == 2) {
                      robot.rotate(motor1, 90, CW);
                    } else {
                      robot.rotate(motor1, 100, CW);
                    }
                } else {
                  bomState = false;
                  robot.rotate(motor1, 0, CW);
                  delay(200);
                  Serial.println("tat bom");
                }
              }
              if (path == "/bom_tuoi_cay/hen_gio") {
                RTDB.to<bool>() ? henGioBom = true :  henGioBom = false;
                
              }

              if (path == "/bom_tuoi_cay/nguong_do_am_dat") {
                nguongDoAmDat = RTDB.to<int>();
              } 

              if (path == "/bom_tuoi_cay/thoi_gian/bat_bom/hour") {
                timeBomOn.hour = RTDB.to<int>();
              }

              if (path == "/bom_tuoi_cay/thoi_gian/bat_bom/min") {
                timeBomOn.min = RTDB.to<int>();
              }

              if (path == "/bom_tuoi_cay/thoi_gian/tat_bom/hour") {
                timeBomOff.hour = RTDB.to<int>();
              }

              if (path == "/bom_tuoi_cay/thoi_gian/tat_bom/min") {
                timeBomOff.min = RTDB.to<int>();
              }

             

              if (path == "/bom_tuoi_cay/toc_do_bom") {
                tocDoBom = RTDB.to<int>();

                if (bomState) {
                  if (tocDoBom == 1) {
                        robot.rotate(motor1, 80, CW);
                      } else if (tocDoBom == 2) {
                        robot.rotate(motor1, 90, CW);
                      } else {
                        robot.rotate(motor1, 100, CW);
                      }
                }
              }


              // DEN
              if (path == "/den_quang_hop/auto") {
                if (RTDB.to<bool>()) {
                  denAuto = true;
                } else {
                  denAuto = false;
                  if (denState) {  // CẬP NHẬT LẠI TRANG THÁI CŨ TRƯỚC AUTO
                    digitalWrite(DEN_PIN, HIGH);
                  } else {
                    digitalWrite(DEN_PIN, LOW);
                  }
                  // cap nhat lai trang thai
                }
              }
              if (path == "/den_quang_hop/nguon") {
                // Serial.println("Bơm tưới cây auto thay đổi !");
                if (RTDB.to<bool>()) {
                  denState = true;
                  digitalWrite(DEN_PIN, HIGH);
                } else {
                  denState = false;
                  digitalWrite(DEN_PIN, LOW);
                }
              }
              if (path == "/den_quang_hop/hen_gio") {
                RTDB.to<bool>() ? henGioDen = true :  henGioDen = false;
              }

              if (path == "/den_quang_hop/thoi_gian/bat_den/hour") {
                timeDenOn.hour = RTDB.to<int>();
              }

              if (path == "/den_quang_hop/thoi_gian/bat_den/min") {
                timeDenOn.min = RTDB.to<int>();
              }

              if (path == "/den_quang_hop/thoi_gian/tat_den/hour") {
                timeDenOff.hour = RTDB.to<int>();
              }

              if (path == "/den_quang_hop/thoi_gian/tat_den/min") {
                timeDenOff.min = RTDB.to<int>();
              }

              if (path == "/den_quang_hop/do_nhay") {
                doNhay = RTDB.to<int>();
              }
              // MÁI CHE
              if (path == "/mai_che/auto" ) {
                Serial.println("Đúng đường dẫn mái che !");
                if (RTDB.to<bool>()) {
                  maiCheAuto = true;
                } else {
                  maiCheAuto = false;
                  if (maiCheState) {  
                    Serial.println("Mái che bật");
                    digitalWrite(MAI_CHE_PIN, HIGH);
                  } else {
                    digitalWrite(MAI_CHE_PIN, LOW);
                    Serial.println("Mái che tắt");
                  }
                  // cap nhat lai trang thai
                }
              }


              if (path == "/mai_che/nguon") {
                Serial.println("Nguồn mái che thay đổi !");
                if (RTDB.to<bool>()) {
                  maiCheState = true;
                  digitalWrite(MAI_CHE_PIN, HIGH);
                } else {
                  maiCheState = false;
                  digitalWrite(MAI_CHE_PIN, LOW);
                }
              }
              if (path == "/mai_che/hen_gio") {
                RTDB.to<bool>() ? henGioMaiChe = true :  henGioMaiChe = false;
              }

              if (path == "/mai_che/thoi_gian/dong_mai_che/hour") {
                timeMaiCheOn.hour = RTDB.to<int>();
              }

              if (path == "/mai_che/thoi_gian/dong_mai_che/min") {
                timeMaiCheOn.min = RTDB.to<int>();
              }

              if (path == "/mai_che/thoi_gian/mo_mai_che/hour") {
                timeMaiCheOff.hour = RTDB.to<int>();
              }

              if (path == "/mai_che/thoi_gian/mo_mai_che/min") {
                timeMaiCheOff.min = RTDB.to<int>();
              }


              // PHUN SUONG
              if (path == "/phun_suong/auto") {
                if (RTDB.to<bool>()) {
                  phunSuongAuto = true;
                } else {
                  phunSuongAuto = false;
                  if (phunSuongState) {  // CẬP NHẬT LẠI TRANG THÁI CŨ TRƯỚC AUTO
                    if (tocDoPhun == 1) {
                      robot.rotate(motor2, 80, CCW);
                    } else if (tocDoPhun == 2) {
                      robot.rotate(motor2, 90, CCW);
                    } else {
                      robot.rotate(motor2, 100, CCW);
                    }

                  } else {
                    robot.rotate(motor2, 0, CCW);
                  }
                  // cap nhat lai trang thai
                }
              }
              if (path == "/phun_suong/nguon") {
                // Serial.println("Bơm tưới cây auto thay đổi !");
                if (RTDB.to<bool>()) {
                  phunSuongState = true;
                  if (tocDoPhun == 1) {
                      robot.rotate(motor2, 80, CCW);
                    } else if (tocDoPhun == 2) {
                      robot.rotate(motor2, 90, CCW);
                    } else {
                      robot.rotate(motor2, 100, CCW);
                    }
                } else {
                  phunSuongState = false;
                  robot.rotate(motor2, 0, CCW);
                }
              }
              if (path == "/phun_suong/hen_gio") {
                RTDB.to<bool>() ? henGioPhunSuong = true :  henGioPhunSuong = false;
              }

              if (path == "/phun_suong/nguong_do_am_mt") {
                Serial.println("Thay đổi ngưỡng độ ẩm");
                nguongDoAmMT = RTDB.to<int>();
              } 

              if (path == "/phun_suong/thoi_gian/bat_phun/hour") {
                timePhunSuongOn.hour = RTDB.to<int>();
              }

              if (path == "/phun_suong/thoi_gian/bat_phun/min") {
                timePhunSuongOn.min = RTDB.to<int>();
              }

              if (path == "/phun_suong/thoi_gian/tat_phun/hour") {
                timePhunSuongOff.hour = RTDB.to<int>();
              }

              if (path == "/phun_suong/thoi_gian/tat_phun/min") {
                timePhunSuongOff.min = RTDB.to<int>();
              }

              if (path == "/phun_suong/toc_do_phun") {
                tocDoPhun = RTDB.to<int>();
                if (phunSuongState) {
                  if (tocDoPhun == 1) {
                        robot.rotate(motor2, 80, CCW);
                      } else if (tocDoPhun == 2) {
                        robot.rotate(motor2, 90, CCW);
                      } else {
                        robot.rotate(motor2, 100, CCW);
                      }
                }
              }

            }
        }
        // else
        // {
        //     Serial.println("----------------------------");
        //     Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
        // }
    }
}

void getSensorValue() 
{
  int h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  int t = dht.readTemperature();

  if (isnan(h) || isnan(t)) {
    Serial.println(F("Failed to read from DHT sensor!"));
    return;
  }

  int analogMoisture = analogRead(SM_PIN);
  soilMoistureValue = ( 100 - ( (analogMoisture/4095.00) * 100 ) );// do am dat
  humidValue = h; // do am moi truong
  tempValue = t; 
  Serial.print("ldrValue:");
  ldrValue = analogRead(LDR_PIN);
  Serial.println(ldrValue);
  rainValue = digitalRead(RAIN_PIN);
  Serial.print("rainValue:");
  Serial.println(rainValue);

  Database.set<bool>(aClient2, "/sensor/cam_bien_mua", rainValue, asyncCB);
  Database.set<int>(aClient2, "/sensor/do_am_dat", soilMoistureValue, asyncCB);
  Database.set<int>(aClient2, "/sensor/do_am_moi_truong", humidValue, asyncCB);
  Database.set<int>(aClient2, "/sensor/nhiet_do", tempValue, asyncCB);
}

void displayOLED(int hour, int min, int tempValue, int humidValue, 
    int soilMoistureValue, bool rainValue, bool bomState,
    bool maiCheState, bool denState,  bool phunSuongState) {

  display.clearDisplay();
  if (hour < 10) {
    display.setCursor(0, 0);
    display.print(0);
    display.setCursor(6, 0);
    display.print(hour);
  } else {
    display.setCursor(0, 0);
    display.print(hour);
  }
  display.setCursor(11, 0);
  display.print(":");
  if (min < 10) {
    display.setCursor(16, 0);
    display.print(0);
    display.setCursor(22, 0);
    display.print(min);
  } else {
    display.setCursor(16, 0);
    display.print(min);
  }


  display.setCursor(0, 10);             // Start at top-left corner
  display.print("Temp:");
  display.setCursor(31, 10); 
  display.print(tempValue); 
  display.drawCircle(46, 12, 2, WHITE);
  display.setCursor(50, 10); 
  display.print("C"); 
  display.setCursor(66, 10);             // Start at top-left corner
  display.print("Humid:");
  display.setCursor(102, 10);             // Start at top-left corner
  display.print(humidValue);
  display.setCursor(116, 10);             // Start at top-left corner
  display.print("%");
  display.setCursor(0, 20);             // Start at top-left corner
  display.print("Soil Moisture:");
  display.setCursor(84, 20);             // Start at top-left corner
  display.print(soilMoistureValue);
  display.setCursor(98, 20); 
  display.print("%");
  display.setCursor(0, 30);             // Start at top-left corner
  display.print("Weather:");
  display.setCursor(48, 30);  
  if (!rainValue) // có mưa 
  {
    display.print("Rain"); 
  } else {
    display.print("No rain"); 
  }

  display.setCursor(0, 42);  
  display.print("Watering:");

  display.setCursor(75, 42);  
  display.print("Cover:");

  display.setCursor(18, 54);  
  display.print("Light:");

  display.setCursor(75, 54);  
  display.print("Spray:");

  display.setCursor(53, 42);             // Start at top-left corner
  bomState ? display.print("ON") : display.print("OFF");
  display.setCursor(110, 42);             // Start at top-left corner
  maiCheState ? display.print("ON") : display.print("OFF");
  display.setCursor(53, 54);             // Start at top-left corner
  denState ? display.print("ON") : display.print("OFF");
  display.setCursor(110, 54);             // Start at top-left corner
  phunSuongState ? display.print("ON") : display.print("OFF");

  display.display();
}

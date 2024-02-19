//Khai báo thư viện 
#include<ESP8266WiFi.h> 
#include<FirebaseESP8266.h>
#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>
//Thiết lập firebase
#define FIREBASE_HOST "doan-2d278-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "mS8wG2HF1hyI7k9u83gZ8TYvGESVNmNhySnhxRu0"
//Thiết lập WiFI
#define WIFI_SSID "Tan Dung"
#define WIFI_PASSWORD "0907930056"
//Định nghĩa các chân giao tiếp với ESP8266
#define trig   2 //D4
#define echo  14  //D5
#define pumpPin  0 //D3
#define oneWireBus  12 //D6 
//Khai báo các biến
boolean pumpState = 0;
String  mode_text;
long duration;
float distance;
float temperatureC;
FirebaseData firebaseData;
String path = "/";
FirebaseJson json;
OneWire oneWire(oneWireBus);
DallasTemperature sensors(&oneWire);

void setup()
{
  //Thiết lập các chân giao tiếp
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(pumpPin, OUTPUT);
  digitalWrite(pumpPin,0);
  Serial.begin(9600);
  sensors.begin();
  //Kiểm tra kết nối WIFI
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(500);
    }
  //Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  //Firebase.reconnectWiFi(true);
  /*if (!Firebase.beginStream(firebaseData,path))
  {
    Serial.println("REASON: " + firebaseData.errorReason());
    Serial.println();
    }*/
  Serial.println("Connect with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  }
  void loop()
  {
    //Đo và lấy dữ liệu từ HCSR04
    digitalWrite(trig,LOW);
    delayMicroseconds(2);
    digitalWrite(trig, HIGH);
    delayMicroseconds(10);
    digitalWrite(trig, LOW);
    duration = pulseIn(echo,HIGH);
    distance = 0.034*duration/2;
    Serial.print("KHOANG CACH:");
    Serial.print(distance);
    Serial.print("CM");
    Serial.println("");
    //Đo và lấy dữ liệu từ DS18B20
    sensors.requestTemperatures(); 
    temperatureC = sensors.getTempCByIndex(0);
    Serial.print(temperatureC);
    Serial.print("ºC");
    Serial.println("");
    //Gửi dữ liệu lên Firebase
    //Firebase.setFloat(firebaseData, path + "/mayBom/status",distance);
    //Firebase.setFloat(firebaseData, path + "/mayBom/temp",temperatureC);

    //Lấy dữ liệu từ Firebase
    /*if(Firebase.getBool(firebaseData, path + "/mayBom/mayBom")) 
      {
        pumpState = firebaseData.boolData();
        Serial.print("pumpstate:");
        Serial.print(pumpState);
        Serial.println("");
      }
      
    if(Firebase.getString(firebaseData, path + "/mayBom/mode"))
      {
        mode_text = firebaseData.stringData();
        Serial.print("mode:");
        Serial.print(mode_text);
        Serial.println("");
      }
      */
     //Chế độ tự động
     if(mode_text == "tuDong")
     {
      if ( distance > 4 ) 
      digitalWrite(pumpPin, HIGH);
      else 
      digitalWrite(pumpPin, LOW);
     }
     
     //Chế độ thủ công
     else if(mode_text == "thuCong")
     {
      if(pumpState == 1)
        digitalWrite(pumpPin, HIGH);
       else
        digitalWrite(pumpPin, LOW);
     }
    }
  

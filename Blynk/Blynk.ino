//Khai báo biến cấu hình blynk
#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID           "TMPL6bNbF5-3Z"
#define BLYNK_TEMPLATE_NAME         "IOTSIC"
#define BLYNK_AUTH_TOKEN            "N1bvQQEmolnesfKtaoBjqH22HAsLAtTP"

//Khai báo thư viện
#include <WiFi.h>
#include <dhtESP32-rmt.h>
#include <BlynkSimpleEsp32.h>
#include <ESP32Servo.h>

//Hiển thị oled
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Cấu hình oled
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDRESS 0x3C
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

//Cấu hình servo
int APin = 18;
ESP32PWM pwm;
int freq = 50;
  
//Khai báo biến phục vụ kết nối wifi và blynk 
char auth[] = "N1bvQQEmolnesfKtaoBjqH22HAsLAtTP";
char ssid[] = "PING⁹⁹⁹⁺";       // Thay bằng tên Wi-Fi thật của bạn
char pass[] = "123456789@"; // Thay bằng mật khẩu Wi-Fi

//Khai báo biến
int sensorPin = 34; 
float temperature = 0.0;
int sensorValue=0;
float humidity = 0.0;

void hienthi(int temp,int humi,int value)
{
     display.clearDisplay();
     display.setTextSize(1);
     display.setTextColor(SSD1306_WHITE);
     display.setCursor(0, 0);
     display.printf("Temp:%doC Humi:%dpt", temp,humi);
     display.setCursor(0, 20);
     display.printf("Gass:%d", value);
     display.display();
}
void setup()
{
  Serial.begin(115200);
  delay(1000);
  
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nConnected to WiFi");

  Blynk.config(auth,"blynk-server.com",8080);
  Blynk.connect();
  pinMode(23, OUTPUT);
  pinMode(1, OUTPUT);
  pinMode(3, OUTPUT);
  //pinMode(34, INPUT);
    // Khởi tạo OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS)) {
    Serial.println(F("Không tìm thấy màn hình OLED"));
    while (true); // Dừng chương trình
  }
  //Khởi tạo servo
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);
  pwm.attachPin(APin, freq, 10);
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Hello world!!");
  display.display();
  delay(1000);
  display.clearDisplay();
}

void loop()
{
  Blynk.run();
  uint8_t error = read_dht(temperature, humidity, 19, DHT11);
  static unsigned long lastMsg = 0;
  sensorValue = analogRead(sensorPin);
  if (millis() - lastMsg > 1000) {
    lastMsg = millis(); 
    Blynk.virtualWrite(V3, sensorValue);
    if (!error) {
      Serial.print("Temp: "); Serial.print(temperature);
      Serial.print(" Hum: "); Serial.println(humidity);
      Blynk.virtualWrite(V0, temperature); 
      Blynk.virtualWrite(V1, humidity);
      hienthi(temperature,humidity,sensorValue);
    } else {
      Serial.println("Error reading DHT sensor");
    }
  }

  if (temperature > 40||sensorValue>1000) 
  {
    digitalWrite(23, 0);
    digitalWrite(1, 1);
    digitalWrite(3, 1);
    pwm.writeScaled(0.12);
  }
  else 
  {
     digitalWrite(23, 1);
     digitalWrite(1, 0);
     digitalWrite(3, 0);
     pwm.writeScaled(0.075);
  }
}

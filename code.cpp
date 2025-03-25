#include <EEPROM.h>
#include <Servo.h>
#include <LiquidCrystal.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// Configurar modulo wifi
const char* ssid = "YOUR_SSID";  
const char* password = "YOUR_PASSWORD"; 

//servidor
const char* serverURL = "http://your-server.com/api";  

#define APP_DELAY 100
#define MULTIPLIER 1
#define FEED_DELTA 1

#define TIME_RATE 1000

#define MIN_TEMPERATURE 15
#define MIN_LIGHT 200
#define MAX_LIGHT 1000

#define TEMPERATURE A1
#define LIGHT A0
#define LAMP 11
#define MOTOR 10
#define SWITCH 9
#define INCREASE 13
#define DECREASE 8
#define POWER A2

Servo servo;
LiquidCrystal lcd(7, 6, 5, 4, 3, 2);

byte degree[]{ 0x06, 0x09, 0x09, 0x06, 0x00, 0x00, 0x00, 0x00 };

int location;
String interface;

int brightness = 0;
int temperature = 0;
int power = 128;
int value = 0;
unsigned feedRate = 3;
bool hasEaten = 0;
int last = 0;

WiFiClient client;

void setup() { 
  pinMode(SWITCH, INPUT);
  
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.createChar(0, degree);
  
  servo.attach(12); 
  servo.write(0);
  
  // Conectar à rede Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado à rede Wi-Fi");
  
  Serial.println("Use command 'show' and locations (0, 1, 2) to view the data");
}

void loop() {
  interface = Serial.readString();
 
  auto time = (millis() / TIME_RATE) - last;
  power = analogRead(POWER);
  EEPROM.update(0, power);
  
  brightness = map(analogRead(LIGHT), 6, 675, 0, 256);
  EEPROM.update(1, brightness);
  
  temperature = (((analogRead(TEMPERATURE) * 5.0) / 1024.0) - 0.5) * 100;
  EEPROM.update(2, temperature);
  auto map_temp = map(temperature, -40, 125, 0, 256);
  
  if (interface.startsWith("show")) {
    location = interface.substring(5,6).toInt();
    if (location == 0) {
      Serial.print("Power of motor: ");
      Serial.println(EEPROM.read(location));
    } else if (location == 1) {
      Serial.print("Brightness: ");
      Serial.println(EEPROM.read(location));
    } else if (location == 2){
      Serial.print("Temperature: ");
      Serial.println(EEPROM.read(location));
    } else {
      Serial.println("No data");
    }
  }
  
  analogWrite(LAMP, brightness > map_temp ? brightness : map_temp);
  analogWrite(MOTOR, power);

  if (time == feedRate) {
    servo.write(90);
    last += time;
    delay(500);
  } else {
    servo.write(0);
  }
  
  // Enviar dados para o servidor via Wi-Fi
  sendDataToServer();

  lcd.clear();
  lcd.setCursor(0, 0);
  
  if (digitalRead(SWITCH)) {    
    lcd.print("STOP WATCH");
    lcd.setCursor(0, 1);
    lcd.print(feedRate);
    lcd.print("s");
    
    if (!digitalRead(INCREASE)) feedRate += MULTIPLIER;
    if (!digitalRead(DECREASE)) feedRate -= MULTIPLIER;
    
    return;
  }
  
  lcd.print(temperature);
  lcd.write(byte(0));
  lcd.print("C ");
  
  lcd.print(time);
  lcd.print(" ");
  lcd.print(map(power, 0, 1022, 0, 100));
  lcd.print("%");
  
  lcd.setCursor(0, 1);
  lcd.print("Light: ");
  lcd.print(map(brightness, 0, 255, 0, 100));
  lcd.print("%");
  
  delay(APP_DELAY);
}

void sendDataToServer() {
  if(WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    
    // Criar o payload 
    String payload = String("{\"temperature\":") + temperature + 
                     String(", \"brightness\":") + brightness +
                     String(", \"power\":") + power +
                     String(", \"feedRate\":") + feedRate + "}";
                     
    http.begin(client, serverURL);
    http.addHeader("Content-Type", "application/json");  // Definir conteúdo como JSON

    int httpResponseCode = http.POST(payload);  // Enviar dados via POST

    if (httpResponseCode > 0) {
      Serial.print("Resposta do servidor: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Erro ao enviar dados: ");
      Serial.println(httpResponseCode);
    }

    http.end();  // Fechar a conexão
  } else {
    Serial.println("Erro: Não está conectado à rede Wi-Fi");
  }
}

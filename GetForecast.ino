#include <Adafruit_NeoPixel.h>

#include <ArduinoJson.h>
#include <ESP8266WiFi.h>

// RGBLEDに出力するピン番号
#define RGBLED_OUTPIN 12
// Arduinoにぶら下がっているRGBLEDの個数
#define NUMRGBLED 1

Adafruit_NeoPixel RGBLED = Adafruit_NeoPixel(NUMRGBLED, RGBLED_OUTPIN, NEO_RGB + NEO_KHZ400);

//const char* ssid     = "Buffalo-G-4FA4";
//const char* password = "8h834syh7m8nw";
const char* ssid     = "takashi の iPhone";
const char* password = "takashi0926";

const char* host = "api.openweathermap.org";

int d = 1;
int brightness = 0;

String weather = "";

String ParseJson(String line){
  String parsedJson = "";

  if(line.indexOf("description")!= -1){
//    Serial.print("choosedline:");
//    Serial.println(line);

    line.remove(line.indexOf("}]")+2,line.length()-line.indexOf("}]")-3);
//    Serial.print("catline:");
//    Serial.println(line);

    int lineLength = line.length()+1;
    char* lineChar = new char[lineLength];
    
    line.toCharArray(lineChar,lineLength);
//    Serial.print("lineChar:");
//    Serial.println(lineChar);
    
    StaticJsonBuffer<500> jsonBuffer;
    JsonObject& root = jsonBuffer.parseObject(lineChar);
    
    if (!root.success()) {
      Serial.println("parseObject() failed");
      return parsedJson;
    }

    String parsedJson = root["weather"][0]["main"];
    Serial.print("weather:");
    Serial.println(parsedJson);
    return parsedJson;
  } 
  else {
    return parsedJson;
  }
}

void getForecastData(){
  Serial.println();
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(host);
  
  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
    Serial.println("connection failed");
    weather = "";
    return;
  }
  
  // We now create a URI for the request
  String url = "/data/2.5/weather?q=Tokyo,jp&APPID=53ae162fe37e0ce562d4e6761a50b4be";
  
  Serial.print("Requesting URL: ");
  Serial.println(url);
  
  // This will send the request to the server
  client.print(String("GET ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  unsigned long timeout = millis();
  while (client.available() == 0) {
    if (millis() - timeout > 5000) {
      Serial.println(">>> Client Timeout !");
      client.stop();
      weather = "";
      return;
    }
  }
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available()){
    String line = client.readStringUntil('\r');
    weather = ParseJson(line);
  }
  
  Serial.println("closing connection");
  return;
}

void showLED(){
  if(brightness >= 255){
    d = -1;
    }else if(brightness <= 0){
      d = 1;
    }
  brightness += d;
  Serial.println(brightness);

}


void setup() {
  RGBLED.begin() ;
  //富樫はこの２行いじればいい
  RGBLED.setBrightness(255);
  //左からLEDのLEDの番号,R,G,B
  RGBLED.setPixelColor(0,100,100,100) ; 
  RGBLED.show() ;
  Serial.begin(115200);
  delay(10);

  // We start by connecting to a WiFi network

  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  delay(10);
//  getForecastData();
//  showLED();
}


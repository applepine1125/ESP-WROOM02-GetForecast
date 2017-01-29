#include <Adafruit_NeoPixel.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <FS.h>

// pin number for output RGBLED
#define RGBLED_OUTPIN 12
// number of RGBLED
#define NUMRGBLED 3
// buffer size for read html
#define BUFFER_SIZE 16384

uint8_t buf[BUFFER_SIZE];

Adafruit_NeoPixel RGBLED = Adafruit_NeoPixel(NUMRGBLED, RGBLED_OUTPIN, NEO_RGB + NEO_KHZ400);

bool isThunder = false;

const char* path_root   = "/index.html";
const char* ssid     = ""; // id
const char* password = ""; // pass
ESP8266WebServer server(80);

//Flashing LED
void showLEDBrightness() {
  int d = 1;
  int brightnessTopLED = 0;// top position LED (LED0)
  int brightnessMiddleLED = 0;// middle position LED (LED1)
  int brightnessBottomLED = 0;// bottom position LED (LED2)

  brightnessTopLED += d;
  brightnessMiddleLED = brightnessTopLED;
  // switch light and shade
  // ===============================255を好きな値に変えて点滅の最大明るさ変更
  if (brightnessTopLED >= 255 || brightnessTopLED <= 0) {
    d = -1 * d;
  }

  //  Serial.println(brightness);

  // random flashing like thunder
  if (isThunder == true) {
    //==============================擬似乱数生成．random(Max),0~Maxの間で整数の
    //==============================乱数生成．maxの値かif文の値変えて雷の頻度健康
    int i = random(300);
    if (i < 100) {
      brightnessBottomLED = 100;
    } else {
      brightnessBottomLED = 0;
    }
  } else {
    //light and shade
    brightnessBottomLED = brightnessTopLED;
  }

}

//read index.html func
boolean readHTML() {
  File htmlFile = SPIFFS.open(path_root, "r");
  if (!htmlFile) {
    Serial.println("Failed to open index.html");
    return false;
  }
  size_t size = htmlFile.size();
  if (size >= BUFFER_SIZE) {
    Serial.print("File Size Error:");
    Serial.println((int)size);
  } else {
    Serial.print("File Size OK:");
    Serial.println((int)size);
  }
  htmlFile.read(buf, size);
  htmlFile.close();
  return true;
}

// call back func
void Root() {
  //  RGBLED.setPixelColor(0,255,255,255) ;
  //  RGBLED.setPixelColor(1,255,255,255) ;
  //  RGBLED.setPixelColor(2,255,255,255) ;
  //  isThunder = false;
  server.send(200, "text/html", (char *)buf);
};

void ClearSky() {
  RGBLED.setPixelColor(0, 255, 63, 47) ;
  RGBLED.setPixelColor(1, 255, 63, 47) ;
  RGBLED.setPixelColor(2, 255, 63, 47) ;
  RGBLED.show();
  isThunder = false;
  Serial.println("change clear sky");
  server.send(200, "text/html", (char *)buf);
};

void FewClouds() {
  RGBLED.setPixelColor(0, 255, 255, 255) ;
  RGBLED.setPixelColor(1, 255, 255, 255) ;
  RGBLED.setPixelColor(2, 255, 63, 47) ;
  isThunder = false;
  Serial.println("change few clouds");
  server.send(200, "text/html", (char *)buf);
};

void ScatteredClouds() {
  RGBLED.setPixelColor(0, 255, 255, 255) ;
  RGBLED.setPixelColor(1, 255, 255, 255) ;
  RGBLED.setPixelColor(2, 255, 255, 255) ;
  isThunder = false;
  Serial.println("change scattered clouds");
  server.send(200, "text/html", (char *)buf);
};

void BrokenClouds() {
  RGBLED.setPixelColor(0, 80, 80, 80) ;
  RGBLED.setPixelColor(1, 80, 80, 80) ;
  RGBLED.setPixelColor(2, 80, 80, 80) ;
  isThunder = false;
  Serial.println("change Broken clouds");
};

void ShowerRain() {
  RGBLED.setPixelColor(0, 80, 80, 80) ;
  RGBLED.setPixelColor(1, 80, 80, 80) ;
  RGBLED.setPixelColor(2, 0, 0, 255) ;
  isThunder = false;
  Serial.println("change shower rain");
};

void Rain() {
  RGBLED.setPixelColor(0, 80, 80, 80) ;
  RGBLED.setPixelColor(1, 0, 0, 255) ;
  RGBLED.setPixelColor(2, 0, 0, 255) ;
  isThunder = false;
  Serial.println("change rain");
};

void ThunderStorm() {
  RGBLED.setPixelColor(0, 0, 0, 255) ;
  RGBLED.setPixelColor(1, 0, 0, 255) ;
  RGBLED.setPixelColor(2, 255, 255, 0) ;
  isThunder = true;
  Serial.println("change thunder storm");
};

void Snow() {
  RGBLED.setPixelColor(0, 100, 100, 100) ;
  RGBLED.setPixelColor(1, 100, 100, 100) ;
  RGBLED.setPixelColor(2, 100, 100, 100) ;
  isThunder = false;
  Serial.println("change snow");
};

void Mist() {
  RGBLED.setPixelColor(0, 100, 100, 100) ;
  RGBLED.setPixelColor(1, 100, 100, 100) ;
  RGBLED.setPixelColor(2, 100, 100, 100) ;
  isThunder = false;
  Serial.println("change mist");
};


void setup() {
  RGBLED.begin() ;
  RGBLED.setBrightness(255);
  //setPixelColor(pinnumber,R,G,B)
  RGBLED.setPixelColor(0, 100, 100, 100) ;
  RGBLED.setPixelColor(1, 100, 100, 100) ;
  RGBLED.setPixelColor(2, 100, 100, 100) ;
  RGBLED.show() ;

  // serial protocol begin
  Serial.begin(115200);
  delay(10);

  // file system begin
  SPIFFS.begin();
  if (!readHTML()) {
    Serial.println("Read HTML error!!");
  }

  // start by connecting to a WiFi network
  Serial.println();
  Serial.println();
  //  Serial.print("Configuring access point...");
  //  WiFi.mode(WIFI_AP);
  //  WiFi.softAP(ssid,password);
  //  IPAddress myIP = WiFi.softAPIP();
  //  Serial.print("AP IP address: ");
  //  Serial.println(myIP);
  WiFi.begin(ssid, password);
  WiFi.mode(WIFI_STA);
  Serial.println("Connecting to Access Point");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  //mDNS begin
  if (!MDNS.begin("wifi-tng")) {
  Serial.println("Error setting up MDNS responder!");
    while (1) {
      delay(1000);
    }
  }

  Serial.println("MDNS responder started");

  //rooting
  server.on("/", Root);
  server.on("/clearsky/", ClearSky);
  server.on("/fewclouds/", FewClouds);
  server.on("/scatteredclouds/", ScatteredClouds);
  server.on("/brokenclouds/", BrokenClouds);
  server.on("/showerrain/", ShowerRain);
  server.on("/rain/", Rain);
  server.on("/thunderstorm/", ThunderStorm);
  server.on("/snow/", Snow);
  server.on("/mist/", Mist);
  server.begin();
  Serial.println("server started.");

  MDNS.addService("http", "tcp", 80);

  RGBLED.show();
}


void loop() {
  server.handleClient();
  showLEDBrightness();
  delay(10);
}

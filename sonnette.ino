#include <SPI.h>
#include <Wire.h>
//#include <Adafruit_GFX.h>

//#include <DHT.h>
#include <ESP8266WiFi.h>
//#include <Adafruit_SSD1306.h>
#include <ESP8266WebServer.h>

const char* ssid = "Livebox-B7B0";
const char* password = "pTXMsVp3CGZiQj6tJy";
#define RELAIS 15  // vers le relais
#define INT D3  // vers l'interupteur
ESP8266WebServer server(80);

void onsonne() {
  digitalWrite(RELAIS, HIGH);
  Serial.println("sa sonne");
  delay(2000);
  digitalWrite(RELAIS, LOW);
}

void connexion() {
  int cpt=0;
  while ((WiFi.status() != WL_CONNECTED) && (cpt < 5)) {
    Serial.println();
    // Connect to WiFi network
    Serial.print("Connecting to ");
    Serial.println(ssid);
    WiFi.begin(ssid, password);
    int t_wifi = millis();
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
      if (millis() - t_wifi > 20000) {break;}
    }
    cpt++;
  }
  if (WiFi.status() != WL_CONNECTED) {
    ESP.reset();
  }
  Serial.println("WiFi connected");
}

void alu() {
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
  digitalWrite(LED_BUILTIN, HIGH);
  Serial.println("alu");
}

String info(){
  String reponse = String("info : \n<br />")
  + "\n<br />Vcc :" + ESP.getVcc()
  + "\n<br />Free Heap :" + ESP.getFreeHeap()
  + "\n<br />Frequence :" + ESP.getCpuFreqMHz()
  + "\n<br />taille du code :" + ESP.getSketchSize()
  //+ "\n<br />signature du ode :" + ESP.getSketchMD5()
  + "\n<br />cycle : " + ESP.getCycleCount() +"\n";
  return reponse;
}

String iptoString (IPAddress ip)
{
  String adr = String("")
  + ip[0]
  + "."
  + ip[1]
  + "."
  + ip[2]
  + "."
  + ip[3];
  return adr;
}

void setup() {
  Serial.begin(115200);
  
  pinMode(RELAIS, OUTPUT);
  digitalWrite(RELAIS,LOW);
  pinMode(INT, OUTPUT);
  digitalWrite(INT,HIGH);

  // server wifi
  String ip = "0";
  connexion();
  // Print the IP address
  Serial.print("URL : ");
  Serial.print("http://");
  ip = iptoString(WiFi.localIP());
  Serial.print(ip);
  Serial.println("/");

  server.on("/", [](){
    Serial.println("get /");
    server.send(200, "text/html", info());
  });
  
  server.on("/sonne", [](){
    Serial.println("get /sonne");
    onsonne();
    server.send(200, "text/html", "sa sonne");
  });
  server.begin();
  Serial.println("Server started");
  
  alu();
}

void loop() {
  server.handleClient();
  delay(100);
}

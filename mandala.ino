#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <CustomStepper.h>

const char* ssid = "lsdmovel";
const char* password = "77188b9f";
byte mac[6];
const int led = 2;

int nivel = -1;
int nivel_antes = 0;
int pulo = 180;

CustomStepper stepper(14, 12, 13, 15);
ESP8266WebServer server(80);

int calcula_pulo(int atual, int objetivo) {
  if(atual > objetivo){
    return (atual-objetivo)*pulo;
  } else if (atual < objetivo){
    return (objetivo - atual) * pulo;
  }
  return 0;
}

byte calcula_rotate(int atual, int objetivo) {
  if(atual > objetivo){
    return CCW;
  } else if (atual < objetivo){
    return CW;
  }
  return CW;
}

void handleRoot() {
  server.send(200, "text/plain", "Vai pegar nunca!");
  digitalWrite(led, 1);
  delay(1000);
  digitalWrite(led, 0);
}

void handleNotFound() {
  digitalWrite(led, 1);
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += server.uri();
  message += "\nMethod: ";
  message += (server.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += server.args();
  message += "\n";
  for (uint8_t i=0; i < server.args(); i++) {
    message += " " + server.argName(i) + ": " + server.arg(i) + "\n";
  }
  server.send(404, "text/plain", message);
  digitalWrite(led, 0);
}

long stringToLong(String s)
{
   char arr[12];
   s.toCharArray(arr, sizeof(arr));
   return atol(arr);
}

void handleGo() {
  digitalWrite(led, 1);
  delay(1000);
  digitalWrite(led, 0);
  
  String message = "Number of args received:";
  message += server.args();
  message += "\n";

  for (int i = 0; i < server.args(); i++) {
    message += "[" + (String)i + "] ";
    message += server.argName(i) + ": ";
    message += server.arg(i) + "\n";
  }

  if (server.arg("step") == "") {
    message += "Step not found";
  } else {
    message += "Step ";
    message += server.arg("step") + "\n";
    
    long stepa = stringToLong(server.arg("step"));
    if (stepa >= 0 && stepa <= 6) {
      nivel = stepa;
    }
  }
  
  server.send(200, "text/plain", message);
}

void setup(void) {
  pinMode(led, OUTPUT);
  digitalWrite(led, 0);
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Start");

  // Wait for connection
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  Serial.print("mac: ");
  Serial.println(WiFi.macAddress());

  if (MDNS.begin("esp8266")) {
    Serial.println("MDNS responder started");
  }

  server.on("/", handleRoot);
  server.on("/go", handleGo);
  server.onNotFound(handleNotFound);
  server.begin();
  Serial.println("HTTP server started");

  stepper.setRPM(12);
  stepper.setSPR(4075.7728395);

  stepper.setDirection(CCW);
  stepper.rotateDegrees(1);
  Serial.println("Stepper running");
}

void loop(void) {
  if (stepper.isDone()) {
    delay(1000);
    if (nivel != -1) {
      Serial.println(nivel);
      Serial.println(nivel_antes);
      Serial.println(calcula_pulo(nivel_antes,nivel));
      stepper.setDirection(calcula_rotate(nivel_antes, nivel));
      stepper.rotateDegrees(calcula_pulo(nivel_antes, nivel));
      nivel_antes = nivel;
      nivel = -1;
      
    }
    server.handleClient();
  }
  stepper.run();
}

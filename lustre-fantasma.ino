#include <WiFi.h>
#include <aREST.h>
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#define LEDC_CHANNEL_0         0
#define LEDC_CHANNEL_1         1
#define LEDC_CHANNEL_2         2
#define LEDC_CHANNEL_3         3
#define LEDC_CHANNEL_4         4
#define LEDC_CHANNEL_5         5
#define LEDC_CHANNEL_6         6
#define LEDC_CHANNEL_7         7
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#define RELAY_FAN_PIN          23
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#define LEDC_TIMER_8_BIT       8
#define LEDC_BASE_FREQ         5000
#define NEED_UPDATE_STATUS     60000 // once a minute
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
#define LED_PIN_R_1            15
#define LED_PIN_G_1            2
#define LED_PIN_B_1            4
#define LED_PIN_R_2            16
#define LED_PIN_G_2            17
#define LED_PIN_B_2            5
#define LED_PIN_R_3            18
#define LED_PIN_G_3            19
#define LED_PIN_B_3            21
#define LED_PIN_R_4            13
#define LED_PIN_G_4            12
#define LED_PIN_B_4            14
#define LED_PIN_R_5            13
#define LED_PIN_G_5            12
#define LED_PIN_B_5            14
#define LED_PIN_W_1            14
#define LED_PIN_W_2            14
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
int rgbstatus_r1 = 0;
int rgbstatus_g1 = 0;
int rgbstatus_b1 = 0;
int rgbstatus_r2 = 0;
int rgbstatus_g2 = 0;
int rgbstatus_b2 = 0;
int white1 = 0;
int white2 = 0;
bool fanState = false;
String ssid = "lustre";
String password = "fantasma";
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
aREST rest = aREST();
WiFiServer server(80);
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
int explosion(String command) {
  digitalWrite(RELAY_FAN_PIN,HIGH);
  delay(1000);
  digitalWrite(RELAY_FAN_PIN,LOW);
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
int ledRGB1Control(String command) {
  String rgb       = command;
  
  // Get rid of '#' and convert it to integer
  long number = (int) strtol( &rgb[0], NULL, 16);
  
  // Split them up into r, g, b values
  int r = number >> 16;
  int g = number >> 8 & 0xFF;
  int b = number & 0xFF;

  rgbstatus_r1 = r;
  rgbstatus_g1 = g;
  rgbstatus_b1 = b;

  ledcWrite(LEDC_CHANNEL_0, rgbstatus_r1);
  ledcWrite(LEDC_CHANNEL_1, rgbstatus_g1);
  ledcWrite(LEDC_CHANNEL_2, rgbstatus_b1);
  
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
int ledRGB2Control(String command) {
  String rgb       = command;
  
  // Get rid of '#' and convert it to integer
  long number = (int) strtol( &rgb[0], NULL, 16);
  
  // Split them up into r, g, b values
  int r = number >> 16;
  int g = number >> 8 & 0xFF;
  int b = number & 0xFF;

  rgbstatus_r2 = r;
  rgbstatus_g2 = g;
  rgbstatus_b2 = b;

  ledcWrite(LEDC_CHANNEL_3, rgbstatus_r2);
  ledcWrite(LEDC_CHANNEL_4, rgbstatus_g2);
  ledcWrite(LEDC_CHANNEL_5, rgbstatus_b2);
  
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
int teste(String command) {
  Serial.println("########################################");
  Serial.println("TESTE A");
  Serial.println(command);  
  Serial.println("########################################");
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void setup() {
  String initialSetup = "";
  Serial.begin(115200);
  delay(10);
  // Setup timer and attach timer to a led pin
  ledcSetup(LEDC_CHANNEL_0, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT);
  ledcSetup(LEDC_CHANNEL_1, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT);
  ledcSetup(LEDC_CHANNEL_2, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT);
  ledcSetup(LEDC_CHANNEL_3, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT);
  ledcSetup(LEDC_CHANNEL_4, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT);
  ledcSetup(LEDC_CHANNEL_5, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT);
  ledcSetup(LEDC_CHANNEL_6, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT);
  ledcSetup(LEDC_CHANNEL_7, LEDC_BASE_FREQ, LEDC_TIMER_8_BIT);
  ledcAttachPin(LED_PIN_R_1, LEDC_CHANNEL_0);
  ledcAttachPin(LED_PIN_G_1, LEDC_CHANNEL_1);
  ledcAttachPin(LED_PIN_B_1, LEDC_CHANNEL_2);
  ledcAttachPin(LED_PIN_R_2, LEDC_CHANNEL_3);
  ledcAttachPin(LED_PIN_G_2, LEDC_CHANNEL_4);
  ledcAttachPin(LED_PIN_B_2, LEDC_CHANNEL_5);
  ledcAttachPin(LED_PIN_R_3, LEDC_CHANNEL_0);
  ledcAttachPin(LED_PIN_G_3, LEDC_CHANNEL_1);
  ledcAttachPin(LED_PIN_B_3, LEDC_CHANNEL_2);
  ledcAttachPin(LED_PIN_R_4, LEDC_CHANNEL_3);
  ledcAttachPin(LED_PIN_G_4, LEDC_CHANNEL_4);
  ledcAttachPin(LED_PIN_B_4, LEDC_CHANNEL_5);
  ledcWrite(LEDC_CHANNEL_0, rgbstatus_r1);
  ledcWrite(LEDC_CHANNEL_1, rgbstatus_g1);
  ledcWrite(LEDC_CHANNEL_2, rgbstatus_b1);
  ledcWrite(LEDC_CHANNEL_3, rgbstatus_r2);
  ledcWrite(LEDC_CHANNEL_4, rgbstatus_g2);
  ledcWrite(LEDC_CHANNEL_5, rgbstatus_b2);
  ledcWrite(LEDC_CHANNEL_6, white1);
  ledcWrite(LEDC_CHANNEL_7, white2);
  pinMode(RELAY_FAN_PIN, OUTPUT);
  digitalWrite(RELAY_FAN_PIN, LOW);
  
  WiFi.softAP("lustre", "fantasma");
  
  Serial.println("");
  Serial.print("IP address: ");
  Serial.println(WiFi.softAPIP());
 
  rest.function("rgb1",ledRGB1Control);
  rest.function("rgb2",ledRGB2Control);
  rest.function("white1",teste);
  rest.function("white2",teste);
  rest.function("explosion",explosion);
 
  server.begin();

}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////
void loop() {
  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while(!client.available()){
    delay(1);
  }
  rest.handle(client);
}
///////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////

#include <WiFiManager.h>
#include <WebSocketsServer.h>
#include <WiFiUdp.h>
#include <EEPROM.h>
#include "conf.h"
#include "epd.h"
#include "js.h"
#include "html.h"

long beginTime;
long udpTime;
unsigned int port = 4280;
bool shouldSaveConfig, sleepMode, wsConnected;
char EDPinit[] = "init";
char EDPnext[] = "next";
char EDPshow[] = "show";
char EDPclear[] = "clear";
char EDPreset[] = "reset";
std::vector<const char *> menu = {"custom", "wifi", "sep", "info", "restart"};

WebSocketsServer webSocket = WebSocketsServer(port);
WiFiUDP Udp;
EPD e;
WiFiManager wm;

void tick(int ts = 0)
{
  int s = digitalRead(BUILTIN_LED);
  for (int i = 0; i < ts; i++)
  {
    digitalWrite(BUILTIN_LED, HIGH);
    delay(25);
    digitalWrite(BUILTIN_LED, LOW);
    delay(50);
  }
  if (digitalRead(BUILTIN_LED) != s)
    digitalWrite(BUILTIN_LED, s);
}

void saveConfigCallback()
{
  shouldSaveConfig = true;
}

struct Settings
{
  char id[20] = "GKK";
  unsigned int key = 8888;
  unsigned int edp = D_EPD;
} sett;

class IntParameter : public WiFiManagerParameter
{
public:
  IntParameter(const char *id, const char *placeholder, long value, const uint8_t length = 10)
      : WiFiManagerParameter("")
  {
    init(id, placeholder, String(value).c_str(), length, "", WFM_LABEL_BEFORE);
  }
  long getValue()
  {
    return String(WiFiManagerParameter::getValue()).toInt();
  }
};

void webSocketEvent(uint8_t num, WStype_t type, uint8_t *payload, size_t length)
{
  if (millis() - beginTime > 10000 || sleepMode)
  {
    switch (type)
    {
    case WStype_DISCONNECTED:
      wsConnected = false;
      digitalWrite(BUILTIN_LED, HIGH);
      break;
    case WStype_CONNECTED:
      wsConnected = true;
      digitalWrite(BUILTIN_LED, LOW);
      break;
    case WStype_TEXT:
      Serial.printf("[%u] get cmd: %s\n", num, payload);
      if (strcmp(EDPinit, (const char *)payload) == 0)
      {
        if (!sleepMode)
        {
          beginTime = millis();
          sleepMode = true;
          e.init();
          webSocket.sendTXT(num, "ok");
        }
      }
      else if (strcmp(EDPnext, (const char *)payload) == 0)
      {
        if (sleepMode)
        {
          e.next();
          webSocket.sendTXT(num, "ok");
        }
      }
      else if (strcmp(EDPshow, (const char *)payload) == 0)
      {
        if (sleepMode)
        {
          beginTime = millis();
          sleepMode = false;
          e.refresh();
          e.sleep();
          webSocket.sendTXT(num, "ok");
        }
      }
      else if (strcmp(EDPclear, (const char *)payload) == 0)
      {
        if (!sleepMode)
        {
          beginTime = millis();
          e.clear();
          webSocket.sendTXT(num, "ok");
        }
      }
      else if (strcmp(EDPreset, (const char *)payload) == 0)
      {
        WiFi.persistent(true);
        WiFi.disconnect(true);
        WiFi.persistent(false);
        webSocket.sendTXT(num, "ok");
        tick(5);
        ESP.restart();
      }
      break;
    case WStype_BIN:
      Serial.printf("[%u] get data: %u ", num, length);
      if (sleepMode)
      {
        for (int i = 0; i < length; i++)
        {
          e.DATA(*payload);
          payload++;
          if (i % 20000 == 0)
            ESP.wdtFeed();
        }
        webSocket.sendTXT(num, "ok");
      }
      Serial.println();
      break;
    }
  }
}

bool validateHttpHeader(String headerName, String headerValue)
{
  if (headerName.equalsIgnoreCase("sKey"))
  {
    return headerValue.toInt() == sett.key;
  }
  return true;
}
void EPD_Init()
{
  digitalWrite(BUILTIN_LED, HIGH);
  e.setSize(((int)wm.server->arg(0)[0] - 'a') + (((int)wm.server->arg(0)[1] - 'a') << 4));
  Serial.println("get cmd: init");
  e.init();
  wm.server->send(200, "text/plain", "Init ok\n");
}

void EPD_Load()
{
  String p = wm.server->arg(0);
  if (p.endsWith("LOAD"))
  {
    int i = 0;
    int DataLength = p.length() - 8;
    Serial.print("get data:");
    Serial.print(DataLength / 2);
    while (i < DataLength)
    {
      e.load(((int)p[i] - 'a') + (((int)p[i + 1] - 'a') << 4));
      i += 2;
    }
  }
  Serial.println();
  wm.server->send(200, "text/plain", "Load ok\n");
}

void EPD_Next()
{
  Serial.println("get cmd: next");
  e.next();
  wm.server->send(200, "text/plain", "Next ok\n");
}

void EPD_Show()
{
  Serial.println("get cmd: show");
  e.refresh();
  e.sleep();
  digitalWrite(BUILTIN_LED, LOW);
  wm.server->send(200, "text/plain", "Show ok\n");
}

void bindServerCallback()
{
  wm.server->on("/pic", handleRoot); // this is now crashing esp32 for some reason
  wm.server->on("/styles.css", sendCSS);
  wm.server->on("/processingA.js", sendJS_A);
  wm.server->on("/processingB.js", sendJS_B);
  wm.server->on("/processingC.js", sendJS_C);
  wm.server->on("/processingD.js", sendJS_D);
  wm.server->on("/LOAD", EPD_Load);
  wm.server->on("/EPD", EPD_Init);
  wm.server->on("/NEXT", EPD_Next);
  wm.server->on("/SHOW", EPD_Show);
  // wm.server->on("/info",handleRoute); // you can override wm!
}

void setup()
{

  Serial.begin(115200);
  EEPROM.begin(512);
  Serial.println("");

  EEPROM.get(100, sett);
  Serial.print("sessionId: ");
  Serial.println(sett.key);
  Serial.print("Product Id: ");
  Serial.println(sett.id);
  Serial.print("Size: ");
  Serial.println(sett.edp);
  e.begin();
  digitalWrite(BUILTIN_LED, LOW);

  // wifi initialization
  wm.setAPStaticIPConfig(IPAddress(1, 1, 1, 1), IPAddress(1, 1, 1, 1), IPAddress(255, 255, 255, 0));
  wm.setSaveConfigCallback(saveConfigCallback);
  wm.setWebServerCallback(bindServerCallback);
  WiFiManagerParameter productId("id", "productId", sett.id, 20);
  IntParameter sessionId("key", "ws sessionKey", sett.key);
  IntParameter Esize("size", "29, 42, 75, 102", sett.edp);
  wm.addParameter(&productId);
  wm.addParameter(&sessionId);
  wm.addParameter(&Esize);
  wm.setCustomMenuHTML("<form action='/pic' method='get'><button>传图</button></form><br/>\n");
  wm.setMenu(menu);
  //  wm.setDarkMode(true);
  // 配网
  wm.autoConnect("ESP_GKK");
  // 解析
  strncpy(sett.id, productId.getValue(), 20);
  sett.edp = Esize.getValue();
  sett.key = sessionId.getValue();
  if (shouldSaveConfig)
  {
    // 保存
    EEPROM.put(100, sett);
    EEPROM.commit();
  }
  tick(5);
  // udp msg
  Udp.begin(4288);
  e.setSize(sett.edp);
  Serial.println("socket begin");
  const char *headerkeys[] = {"sKey"};
  size_t headerKeyCount = sizeof(headerkeys) / sizeof(char *);
  webSocket.onValidateHttpHeader(validateHttpHeader, headerkeys, headerKeyCount);
  webSocket.begin();
  webSocket.onEvent(webSocketEvent);
}

void loop()
{
  webSocket.loop();
  if (sleepMode && millis() - beginTime > 10000)
  {
    tick(4);
    Serial.println("auto sleep");
    sleepMode = false;
    e.sleep();
  }
  if (!wsConnected && millis() - udpTime > 5000)
  {
    udpTime = millis();
    udpSend();
  }
}
void udpSend()
{
  Udp.beginPacket("255.255.255.255", 4288);
  Udp.print(sett.id);
  Udp.endPacket();
}

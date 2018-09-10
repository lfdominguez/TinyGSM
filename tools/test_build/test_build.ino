/**************************************************************
 *
 *  DO NOT USE THIS - this is just a compilation test!
 *
 **************************************************************/
// #define TINY_GSM_MODEM_SIM800  // Select for a SIM800, SIM900, or variant thereof
// #define TINY_GSM_MODEM_A6  // Select for a AI-Thinker A6 or A7 chip
// #define TINY_GSM_MODEM_M590  // Select for a Neoway M590
// #define TINY_GSM_MODEM_UBLOX  // Select for most u-blox cellular modems
// #define TINY_GSM_MODEM_ESP8266  // Select for an ESP8266 using the DEFAULT AT COMMAND FIRMWARE
// #define TINY_GSM_MODEM_XBEE  // Select for Digi brand WiFi or Cellular XBee's

#include <TinyGsmClient.h>

TinyGsm modem(Serial);
TinyGsmClient client(modem);

#if defined(TINY_GSM_MODEM_HAS_SSL)
  TinyGsmClientSecure client_secure(modem);
#endif

char server[] = "somewhere";
char resource[] = "something";

void setup() {
  Serial.begin(115200);
  delay(3000);
  modem.restart();
}

void loop() {

  // Test the start/restart functions
  modem.restart();
  modem.begin();
  modem.testAT();
  modem.factoryDefault();

  // Test the SIM card functions
  #if defined(TINY_GSM_MODEM_HAS_GPRS)
  modem.getSimCCID();
  modem.getIMEI();
  modem.getSimStatus();
  modem.getRegistrationStatus();
  modem.getOperator();
  #endif


  // Test the Networking functions
  modem.getSignalQuality();
  modem.localIP();

  #if defined(TINY_GSM_MODEM_HAS_GPRS)
    modem.waitForNetwork();
    modem.gprsConnect("YourAPN", "", "");
  #endif
  #if defined(TINY_GSM_MODEM_HAS_WIFI)
    modem.networkConnect("YourSSID", "YourPWD");
    modem.waitForNetwork();
  #endif

  client.connect(server, 80);

  // Make a HTTP GET request:
  client.print(String("GET ") + resource + " HTTP/1.0\r\n");
  client.print(String("Host: ") + server + "\r\n");
  client.print("Connection: close\r\n\r\n");

  unsigned long timeout = millis();
  while (client.connected() && millis() - timeout < 10000L) {
    while (client.available()) {
      client.read();
      timeout = millis();
    }
  }

  client.stop();

  #if defined(TINY_GSM_MODEM_HAS_GPRS)
    modem.gprsDisconnect();
  #endif
  #if defined(TINY_GSM_MODEM_HAS_WIFI)
    modem.networkDisconnect();
  #endif
}

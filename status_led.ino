#include <ESP8266WiFi.h>

int statusLed_i = -1;
int statusLed_t = 0;

const int LED_PATTERN_WIFI_CONNECTING_LN = 2;
const int LED_PATTERN_WIFI_CONNECTING[LED_PATTERN_WIFI_CONNECTING_LN] = { 500, 500 };

const int LED_PATTERN_READY_LN = 2;
const int LED_PATTERN_READY[LED_PATTERN_WIFI_CONNECTING_LN] = { 100, 2900 };

void setupStatusLed() {
  pinMode(STATUS_LED_PIN, OUTPUT);
  digitalWrite(STATUS_LED_PIN, STATUS_LED_OFF);
}

void loopStatusLed() {
  if (WiFi.status() != WL_CONNECTED) {
    ledPatternLoop(
      STATUS_LED_PIN,
      statusLed_i, statusLed_t,
      LED_PATTERN_WIFI_CONNECTING,
      LED_PATTERN_WIFI_CONNECTING_LN,
      STATUS_LED_ON, STATUS_LED_OFF
    );
  } else if (relayOffAfterTimestamp > 0) {
    // Means working
    digitalWrite(STATUS_LED_PIN, STATUS_LED_ON);
  } else {
    ledPatternLoop(
      STATUS_LED_PIN,
      statusLed_i, statusLed_t,
      LED_PATTERN_READY,
      LED_PATTERN_READY_LN,
      STATUS_LED_ON, STATUS_LED_OFF
    );
  }
}

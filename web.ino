#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EspMQTTClient.h>

extern ESP8266WebServer server(80);

#if ENABLE_MQTT
EspMQTTClient mqttClient(
  MQTT_BROKER_IP,
  MQTT_BROKER_PORT,
  MQTT_USERNAME,
  MQTT_PASSWORD,
  MQTT_CLIENT_NAME
);
#endif

extern int lastMqttWifiStatus = WiFi.status();
bool lastMqttDoorOpened = false;
int mqttStatusLastPublishedAtTimestamp = -1;

void setupWeb() {
  Serial.print("Connecting to \"");
  Serial.print(WIFI_SSID);
  Serial.print("\" ...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("WiFi connected. ");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();

  #if ENABLE_MQTT
  mqttClient.enableHTTPWebUpdater();
  mqttClient.enableDebuggingMessages();
  #endif

  server.on("/", HTTP_GET, []() {
    String content = "";
    content += "<!doctype html>\n";
    content += "<html>\n";
    content += "<head>\n";
    content += "  <meta charset=\"UTF-8\">\n";
    content += "  <meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">\n";
    content += "  <title>electric_lock_control_esp8266</title>\n";
    content += "  </head>\n";
    content += "  <body>\n";
    content += "  <h1>Welcome to electric_lock_control_esp8266</h1>\n";

    // if (doorOpened) {
    //   content += "  <p>Door is opened.</p>\n";
    // } else {
    //   content += "  <p>Door is closed.</p>\n";
    // }

    content += "  <p><form action=\"/status\" method=\"get\">\n";
    content += "    API key: <input type=\"text\" name=\"api_key\" value=\"\">\n";
    content += "    <input type=\"submit\" value=\"Get status\">\n";
    content += "  </form></p>\n";

    content += "  <p><form action=\"/open\" method=\"post\">\n";
    content += "    API key: <input type=\"text\" name=\"api_key\" value=\"\">\n";
    content += "    Relay on duration: <input type=\"number\" name=\"relay_on_duration\" min=\"1\" max=\"5000\" value=\"\" placeholder=\"(optional)\">\n";
    content += "    <input type=\"submit\" value=\"Open\">\n";
    content += "  </form></p>\n";

    content += "  </body>\n";
    content += "</html>\n";

    server.sendHeader("Connection", "close");
    server.send(200, "text/html", content);
  });

  server.on("/open", HTTP_POST, []() {
    String apiKey = "";
    int nextRelayOnDuration = RELAY_ON_DURATION;

    for (int i = 0; i <= server.args(); i++) {
      if (server.argName(i) == "api_key") {
        apiKey = server.arg(i);
      }

      if (server.argName(i) == "relay_on_duration") {
        nextRelayOnDuration = server.arg(i).toInt();
      }
    }

    if (apiKey.isEmpty()) {
      return server.send(401, "text/plain", "HTTP 401: api_key is required.");
    }

    if (apiKey != API_KEY) {
      Serial.print("Got invalid API key: ");
      Serial.println(apiKey);
      return server.send(403, "text/plain", "HTTP 403: invalid api_key.");
    }

    Serial.print("Executing open from web API, relayOnDuration: ");
    Serial.println(nextRelayOnDuration);

    relayOnDuration = nextRelayOnDuration;
    relayOn = true;

    String content;
    content = "opened";
    server.sendHeader("Location", "/");
    server.sendHeader("Connection", "close");
    server.send(301, "text/html", content);
  });

  server.on("/status", HTTP_GET, []() {
    String apiKey = "";

    for (int i = 0; i <= server.args(); i++) {
      if (server.argName(i) == "api_key") {
        apiKey = server.arg(i);
      }
    }

    if (apiKey.isEmpty()) {
      return server.send(401, "text/plain", "HTTP 401: api_key is required.");
    }

    if (apiKey != API_KEY) {
      Serial.print("Got invalid API key: ");
      Serial.println(apiKey);
      return server.send(403, "text/plain", "HTTP 403: invalid api_key.");
    }

    String content;
    if (doorOpened) {
      content = "opened";
    } else {
      content = "closed";
    }
    server.sendHeader("Connection", "close");
    server.send(200, "text/html", content);
  });

  server.begin();
}

#if ENABLE_MQTT
void onConnectionEstablished() {
  mqttClient.subscribe(MQTT_COMMAND_TOPIC, [] (const String &payload) {
    if (payload.equals(MQTT_OPEN_COMMAND)) {
      Serial.println("Executing open from MQTT");
      relayOn = true;
    } else if (payload.equals("LOCK")) {
      // Door can only be locked manually, we do this to clear the "Locking..." status on some UI implementations (e.g. Apple HomeKit)
      mqttClient.publish(MQTT_STATE_TOPIC, "LOCKED");
      // Restore the correct state after 300 ms
      mqttStatusLastPublishedAtTimestamp = millis() - MQTT_PUBLISH_STATE_EVERY + 300;
    }
  });
}

void mqttPublishStatus() {
  mqttClient.publish(MQTT_STATE_TOPIC, doorOpened ? MQTT_DOOR_OPENED_STATE : MQTT_DOOR_CLOSED_STATE);
  mqttStatusLastPublishedAtTimestamp = millis();
}
#endif

void loopWeb() {
  server.handleClient();

  #if ENABLE_MQTT
  mqttClient.loop();

  if (lastMqttDoorOpened != doorOpened) mqttPublishStatus();
  if (WiFi.status() == WL_CONNECTED && lastMqttWifiStatus != WiFi.status()) mqttPublishStatus();
  if (millis() - mqttStatusLastPublishedAtTimestamp > MQTT_PUBLISH_STATE_EVERY) mqttPublishStatus();

  lastMqttDoorOpened = doorOpened;
  lastMqttWifiStatus = WiFi.status();
  #endif
}

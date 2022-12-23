// Serial
#define SERIAL_BAUD 9600

// WiFi config
#define WIFI_SSID "IoT WiFi"
#define WIFI_PASSWORD "wifi_password"

// Web API
#define API_KEY "api-key"

// Relay to control the electronic lock
#define RELAY_PIN 5 // D1
#define RELAY_ON HIGH
#define RELAY_OFF LOW
#define RELAY_ON_DURATION 500

// Door open status
#define DOOR_OPEN_LIGHT_SENSOR_INPUT_PIN 14 // D5
#define DOOR_OPEN_LIGHT_SENSOR_THRESHOLD 900
#define DOOR_OPEN_LIGHT_SENSOR_STABILIZER_PARAMETER 12

// Status LED
#define STATUS_LED_PIN LED_BUILTIN
#define STATUS_LED_ON LOW
#define STATUS_LED_OFF HIGH

// MQTT
#define ENABLE_MQTT false
#define MQTT_BROKER_IP "192.168.88.2"
#define MQTT_BROKER_PORT 51883
#define MQTT_USERNAME "mqtt-username"
#define MQTT_PASSWORD "mqtt-password"
#define MQTT_CLIENT_NAME "electric_lock_control_esp8266"
#define MQTT_COMMAND_TOPIC "electric_lock_control_esp8266/command"
#define MQTT_STATE_TOPIC "electric_lock_control_esp8266/state"
#define MQTT_PUBLISH_STATE_EVERY (1000 * 60 * 5)
#define MQTT_OPEN_COMMAND "UNLOCK"
#define MQTT_DOOR_OPENED_STATE "UNLOCKED"
#define MQTT_DOOR_CLOSED_STATE "LOCKED"

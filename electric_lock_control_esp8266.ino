/**
 * electric_lock_control_esp8266
 *
 * See 00_config.ino for configurations.
 */

void setup() {
  setupStatusLed();
  setupSerial();
  setupWeb();
  setupRelayControl();
  setupDoorOpenStatus();
}

void loop() {
  loopStatusLed();
  loopSerial();
  loopWeb();
  loopRelayControl();
  loopDoorOpenStatus();
}

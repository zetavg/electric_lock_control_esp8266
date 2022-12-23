enum relay_state_t {
  ON,
  OFF,
};

extern relay_state_t relay_state = OFF;

extern bool relayOn = false;
extern int relayOnDuration = RELAY_ON_DURATION;
extern int relayOffAfterTimestamp = -1;

void setupRelayControl() {
  pinMode(RELAY_PIN, OUTPUT);
  digitalWrite(RELAY_PIN, RELAY_OFF);
}

void loopRelayControl() {
  if (relayOn) {
    if (relayOnDuration > 10000) relayOnDuration = 10000;
    if (relayOnDuration < 1) relayOnDuration = 1;
    relayOffAfterTimestamp = millis() + relayOnDuration;
    digitalWrite(RELAY_PIN, RELAY_ON);
    relayOn = false;
    relayOnDuration = RELAY_ON_DURATION;
  }

  if (relayOffAfterTimestamp > 0 && millis() >= relayOffAfterTimestamp) {
    digitalWrite(RELAY_PIN, RELAY_OFF);
    relayOffAfterTimestamp = -1;
  }
}

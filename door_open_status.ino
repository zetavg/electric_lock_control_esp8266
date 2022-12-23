extern bool doorOpened = false;
int openedCount = 0;
int closedCount = 0;

void setupDoorOpenStatus() {
  pinMode(DOOR_OPEN_LIGHT_SENSOR_INPUT_PIN, INPUT);
}

void loopDoorOpenStatus() {
  int doorOpenStatusPinValue = analogRead(DOOR_OPEN_LIGHT_SENSOR_INPUT_PIN);

  if (doorOpenStatusPinValue > DOOR_OPEN_LIGHT_SENSOR_THRESHOLD) {
    closedCount = 0;
    if (openedCount < DOOR_OPEN_LIGHT_SENSOR_STABILIZER_PARAMETER) {
      openedCount++;
      return;
    }

    if (!doorOpened) {
      Serial.println("Door changed to opened");
    }
    doorOpened = true;
  } else {
    openedCount = 0;
    if (closedCount < DOOR_OPEN_LIGHT_SENSOR_STABILIZER_PARAMETER) {
      closedCount++;
      return;
    }

    if (doorOpened) {
      Serial.println("Door changed to closed");
    }
    doorOpened = false;
  }
}

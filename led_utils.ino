void ledPatternLoop(int pin, int &i, int &t, const int patternArr[], const int patternLength, int on = HIGH, int off = LOW) {
  if (i < 0 || i > patternLength) {
    i = 0;
    t = millis() + patternArr[i];
  }
  if (millis() >= t) {
    i++;
    if (i >= patternLength) i = 0;
    t = millis() + patternArr[i];
  }
  if (i % 2 == 0) {
    digitalWrite(pin, on);
  } else {
    digitalWrite(pin, off);
  }
}

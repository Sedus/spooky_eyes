const int leftEyePin = 9;
const int rightEyePin = 10;

int currentEffect = 0;
long startEffect = 0;

const int minEffectTime = 30000; // Minimum duration of effect
const int maxEffectTime = 60000; // Maximum duration of effect
const int effects = 3; // amount of active effects;

// Blinking
long blinkTime = 0;
int nextBlinkTime = 0;
const int minBlinkTime = 10000; // Blink not before
const int maxBlinkTime = 15000; // Blink before

// Flickering
const int maxFlickeringTime = 10000; // Maximum ms flickering may occur

void setup() {
  Serial.begin(115200);
  openEyes(30, 5, true);
  nextBlinkTime = random(minBlinkTime, maxBlinkTime);
}

void loop() {
  if (currentEffect == 1 && (millis() - blinkTime) >= nextBlinkTime) {
    doBlink();
    nextBlinkTime = random(minBlinkTime, maxBlinkTime);
  }

  if (currentEffect == 2) {
    if (millis() - startEffect <= maxFlickeringTime) {
      flicker(false);
    } else {
      // Stop flickering
      openEyes(30, 5, true);
    }
  }

  // wait some time before a new effect can occur
  if (millis() - startEffect > random(minEffectTime, maxEffectTime)) {
    int effect = random(0, effects);
    if (effect != currentEffect) {
      Serial.println("Old effect: " + (String)currentEffect + " New effect: " + (String)effect);
      switch (effect) {
        case 0: closeEyes(30, 5, true); break;
        case 1: openEyes(30, 5, true); break;
        case 2: flicker(true); break;
      }
      currentEffect = effect;
    }
  }
}

void closeEyes(int ms, int steps, boolean start) {
  if (start) {
    Serial.println("Close eyes");
    startEffect = millis();
  }
  currentEffect = 0;
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= steps) {
    analogWrite(leftEyePin, fadeValue);
    analogWrite(rightEyePin, fadeValue);
    delay(ms);
  }
}

void openEyes(int ms, int steps, boolean start) {
  if (start) {
    Serial.println("Open eyes");
    startEffect = millis();
    blinkTime = 0;
  }
  currentEffect = 1;
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += steps) {
    analogWrite(leftEyePin, fadeValue);
    analogWrite(rightEyePin, fadeValue);
    delay(ms);
  }
  analogWrite(leftEyePin, 255);
  analogWrite(rightEyePin, 255);
}

void flicker(boolean start) {
  if (start) {
    Serial.println("Flicker eyes");
    startEffect = millis();
    currentEffect = 2;
  }
  if (currentEffect == 0) {
    openEyes(30, 5, false);
  }
  int value = random(0, 200);
  analogWrite(leftEyePin, value);
  analogWrite(rightEyePin, value);
  delay(50);
}

// Part of openEyes. Blink every 2 seconds
void doBlink() {
  Serial.println("Blink after " + (String)(millis() - blinkTime) + " ms");
  blinkTime = millis();
  if (currentEffect == 1) {
    closeEyes(5, 20, false);
    openEyes(5, 20, false);
  }
}

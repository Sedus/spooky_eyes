const int leftEyePin = 9;
const int rightEyePin = 10;

long startTimeEffect = 0;

const int minEffectTime = 1 * 60000; // Minimum duration of effect
const int maxEffectTime = 5 * 60000; // Maximum duration of effect

// Eyes status
const String OPEN = "OPEN";
const String CLOSED = "CLOSED";
const String FLICKER = "FLICKER";
String status = CLOSED;

// Blinking
const int minBlinkTime = 10000; // Blink not before
const int maxBlinkTime = 15000; // Blink before
long lastBlinkTime = millis();
long nextBlinkTime = random(minBlinkTime, maxBlinkTime);

// Flickering
const int MAX_FLICK_TIME = 20000; // Maximum ms flickering may occur

// Weighted effects (sum must be 100)
// _NR must correspond to the index in the array!!!
const int STARING_WEIGHT = 50;
const int STARING_NR = 0;

const int SLEEPING_WEIGHT = 35;
const int SLEEPING_NR = 1;

const int FLICKERING_WEIGHT = 15;
const int FLICKERING_NR = 2;

const int EFFECTS[] = {STARING_WEIGHT, SLEEPING_WEIGHT, FLICKERING_WEIGHT};
const int EFFECT_COUNT = 3; // length of array
int activeEffect;

void setup() {
  Serial.begin(115200);
  randomSeed(analogRead(0));
//  delay(5000);
  stare();
}

void loop() {
  if (activeEffect == STARING_NR && (millis() - lastBlinkTime) >= nextBlinkTime) {
    doBlink(1);
    nextBlinkTime = random(minBlinkTime, maxBlinkTime);
  }

  // wait some time before a new effect can occur
  if (millis() - startTimeEffect > random(minEffectTime, maxEffectTime)) {
    int newEffect = getEffect();
    startTimeEffect = millis();
    switch (newEffect) {
      case STARING_NR: stare(); break;
      case SLEEPING_NR: sleep(); break;
      case FLICKERING_NR: flicker(MAX_FLICK_TIME); break;
    }
  }
}

int getEffect() {
  int effect = random(0, 100);
  Serial.println("New random: " + (String)effect);
  int x = 0;
  for (int i = 0; i < EFFECT_COUNT; i++) {
    x += EFFECTS[i];
    if (x >= effect) {
      return i;
    }
  }
}

void closeEyes(int ms, int steps) {
  Serial.println("Closing eyes");
  for (int fadeValue = 255 ; fadeValue >= 0; fadeValue -= steps) {
    analogWrite(leftEyePin, fadeValue);
    analogWrite(rightEyePin, fadeValue);
    delay(ms);
  }
  status = CLOSED;
}

void openEyes(int ms, int steps) {
  Serial.println("Opening eyes");
  for (int fadeValue = 0 ; fadeValue <= 255; fadeValue += steps) {
    analogWrite(leftEyePin, fadeValue);
    analogWrite(rightEyePin, fadeValue);
    delay(ms);
  }
  analogWrite(leftEyePin, 255);
  analogWrite(rightEyePin, 255);
  status = OPEN;
}

/*
   STARING: effect

   Eyes are open and blink once in a while
*/
void stare() {
  if (status == CLOSED) {
    // Wakeup dramatically
    for (int i = 0; i < 5; i++) {
      openEyes(3*i, 50);
      delay(30*i);
      closeEyes(3*i, 50);
      delay(30*i);
    }
    lastBlinkTime = millis();
    openEyes(30, 5);
  }
  activeEffect = STARING_NR;
  Serial.println("-- EFFECT: Staring");
}

/*
   SLEEPING: effect

   Eyes are closed
*/
void sleep() {
  if (status == OPEN) {
    closeEyes(30, 5);
  }
  activeEffect = SLEEPING_NR;
  Serial.println("-- EFFECT: Sleeping");
}

/*
   FLICKERING: effect

   Eyes burst into berserk flaming
*/
void flicker(long duration) {
  if (status = CLOSED) {
    openEyes(10, 15);
  }
  activeEffect = FLICKERING_NR;
  Serial.println("-- EFFECT: Flickering");
  while (millis() - startTimeEffect <= duration) {
    int value = 0;
    if (millis() % 2 == 0) {
      value = random(0, 100);
    } else {
      value = random(155, 255);
    }
    analogWrite(leftEyePin, value);
    analogWrite(rightEyePin, value);
    delay(100);
  }
  doBlink(3);
  delay(250);
  sleep();
}

void doBlink(int times) {
  Serial.println("Blinking after " + (String)(millis() - lastBlinkTime) + " ms");
  lastBlinkTime = millis();
  for (int i = 0; i < times; i++) {
    if (status == OPEN) {
      closeEyes(10, 15);
      delay(500);
      openEyes(10, 15);
    }
  }
}

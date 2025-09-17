// -------------------------------
// Traffic Light with Automatic Override Mode
// Using millis() (non-blocking, no delay())
// -------------------------------

// LED pins
const int GREEN_LED  = 13;
const int YELLOW_LED = 12;
const int RED_LED    = 11;

// Virtual switch pins (outputs this time)
const int S1 = 10;  // Force Green
const int S2 = 9;   // Force Yellow
const int S3 = 8;   // Force Red

// Cycle durations (milliseconds)
const unsigned long GREEN_TIME  = 5000;
const unsigned long YELLOW_TIME = 1000;
const unsigned long RED_TIME    = 3000;

// Override pattern duration
const unsigned long PATTERN_INTERVAL = 500; // 500 ms

// State machine for traffic cycle
enum TrafficState { STATE_GREEN, STATE_YELLOW, STATE_RED };
TrafficState currentState = STATE_GREEN;

// Timing
unsigned long previousMillis = 0;       // For traffic cycle
unsigned long patternMillis  = 0;       // For override pattern

// Pattern index (0–7)
int patternIndex = 0;

// --------------------------------------------
// Setup
// --------------------------------------------
void setup() {
  // LED outputs
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Virtual switch outputs
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);

  // Start with green ON
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  previousMillis = millis();
  patternMillis = millis();
}

// --------------------------------------------
// Main loop
// --------------------------------------------
void loop() {
  unsigned long currentMillis = millis();

  // ----------- Handle traffic light cycle -----------
  switch (currentState) {
    case STATE_GREEN:
      if (currentMillis - previousMillis >= GREEN_TIME) {
        currentState = STATE_YELLOW;
        previousMillis = currentMillis;
      }
      break;

    case STATE_YELLOW:
      if (currentMillis - previousMillis >= YELLOW_TIME) {
        currentState = STATE_RED;
        previousMillis = currentMillis;
      }
      break;

    case STATE_RED:
      if (currentMillis - previousMillis >= RED_TIME) {
        currentState = STATE_GREEN;
        previousMillis = currentMillis;
      }
      break;
  }

  // ----------- Update virtual switch pattern -----------
  if (currentMillis - patternMillis >= PATTERN_INTERVAL) {
    patternIndex = (patternIndex + 1) % 8; // 0–7 looping
    patternMillis = currentMillis;

    // Write binary pattern to S1, S2, S3
    digitalWrite(S1, (patternIndex & 0b100) ? HIGH : LOW); // MSB = S1
    digitalWrite(S2, (patternIndex & 0b010) ? HIGH : LOW); // Mid = S2
    digitalWrite(S3, (patternIndex & 0b001) ? HIGH : LOW); // LSB = S3
  }

  // ----------- Normal cycle LED states -----------
  bool greenOn  = (currentState == STATE_GREEN);
  bool yellowOn = (currentState == STATE_YELLOW);
  bool redOn    = (currentState == STATE_RED);

  // ----------- Apply overrides from virtual switches -----------
  if (digitalRead(S1) == HIGH) greenOn  = true;
  if (digitalRead(S2) == HIGH) yellowOn = true;
  if (digitalRead(S3) == HIGH) redOn    = true;

  // ----------- Write outputs to LEDs -----------
  digitalWrite(GREEN_LED,  greenOn  ? HIGH : LOW);
  digitalWrite(YELLOW_LED, yellowOn ? HIGH : LOW);
  digitalWrite(RED_LED,    redOn    ? HIGH : LOW);
}

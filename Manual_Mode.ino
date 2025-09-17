// -------------------------------
// Traffic Light with Override Switches
// Using millis() (non-blocking, no delay())
// -------------------------------

// LED pins
const int GREEN_LED  = 13;
const int YELLOW_LED = 12;
const int RED_LED    = 11;

// Switch pins
const int SW_GREEN  = 10;  // S1 = Force Green
const int SW_YELLOW = 9;   // S2 = Force Yellow
const int SW_RED    = 8;   // S3 = Force Red

// Cycle durations (milliseconds)
const unsigned long GREEN_TIME  = 5000;
const unsigned long YELLOW_TIME = 1000;
const unsigned long RED_TIME    = 3000;

// State machine for cycle
enum TrafficState { STATE_GREEN, STATE_YELLOW, STATE_RED };
TrafficState currentState = STATE_GREEN;

// Timing
unsigned long previousMillis = 0;  // stores last state change time

// --------------------------------------------
// Setup
// --------------------------------------------
void setup() {
  // LED pins as output
  pinMode(GREEN_LED, OUTPUT);
  pinMode(YELLOW_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);

  // Switch pins as input (already pulled down externally)
  pinMode(SW_GREEN, INPUT);
  pinMode(SW_YELLOW, INPUT);
  pinMode(SW_RED, INPUT);

  // Start with green ON
  digitalWrite(GREEN_LED, HIGH);
  digitalWrite(YELLOW_LED, LOW);
  digitalWrite(RED_LED, LOW);

  previousMillis = millis();
}

// --------------------------------------------
// Main loop
// --------------------------------------------
void loop() {
  unsigned long currentMillis = millis();

  // ----------- Handle cycle state machine -----------
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

  // ----------- Update LEDs based on state -----------
  bool greenOn  = (currentState == STATE_GREEN);
  bool yellowOn = (currentState == STATE_YELLOW);
  bool redOn    = (currentState == STATE_RED);

  // ----------- Apply overrides -----------
  // If a switch is pressed, force its LED ON (cannot be turned off by cycle)
  if (digitalRead(SW_GREEN) == HIGH) {
    greenOn = true;
  }
  if (digitalRead(SW_YELLOW) == HIGH) {
    yellowOn = true;
  }
  if (digitalRead(SW_RED) == HIGH) {
    redOn = true;
  }

  // ----------- Write outputs -----------
  digitalWrite(GREEN_LED, greenOn ? HIGH : LOW);
  digitalWrite(YELLOW_LED, yellowOn ? HIGH : LOW);
  digitalWrite(RED_LED, redOn ? HIGH : LOW);
}

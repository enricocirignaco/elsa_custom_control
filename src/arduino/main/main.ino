// Pinout
#define PAS_PIN 27 // digital interrupt
#define GAS_OUTPUT_PIN 25 // DAC output
#define THROTTLE_PIN 35 // ADC input
#define LED_PIN 2 // PWM output
// Config Parameters
#define PULSE_START_THRESHOLD 3 // 360°=12pulses --> 90°=3pulses
#define TIME_STOP_THRESHOLD_MS 300

uint32_t pulse_cnt = 0;
uint32_t time_elapsed_ms = 0;
uint16_t throttle_value = 0;
uint16_t min_throttle_value;
bool automatic_run = false;

void setup() {
  pinMode(PAS_PIN, INPUT);
  pinMode(THROTTLE_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(PAS_PIN), pulseHandler, RISING); //Each rising edge on PAS pin causes an interrupt
  Serial.begin(9600);
  min_throttle_value = analogRead(THROTTLE_PIN);
}


void loop() {
  // read throttle value
  throttle_value = analogRead(THROTTLE_PIN);
  // remap value
  map(throttle_value, 0, 1023, 0, 255);
  //decide whether manual throttle or pas value should be sent
  uint16_t pas_value = getAutomaticThrottleValue;
  if(pas_value > throttle_value) throttle_value = pas_value;
  // send value to controller
  dacWrite(GAS_OUTPUT_PIN, throttle_value);
  // check timeout automatic run
  checkAutomaticTimeout();
  }

void pulseHandler(){
  if(++pulse_cnt >= PULSE_START_THRESHOLD) {
    time_elapsed_ms = millis();
    automatic_run = true;
  }

void checkAutomaticTimeout(){
  if(automatic_run){
    uint32_t currentTime = millis();
    if((currentTime > time_elapsed_ms) && ((currentTime-time_elapsed_ms) > TIME_STOP_THRESHOLD_MS)){
      automatic_run = false;
    }
  } 
}
uint8_t getAutomaticThrottleValue() {
  if(automatic_run){
    // do something
    return 200;
  }
  return 0;

}
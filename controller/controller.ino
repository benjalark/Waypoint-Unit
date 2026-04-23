#include <dht_nonblocking.h>

const int NUM_PINS = 6;
const int MAX_POT_VAL = 1023;

const int outPins[NUM_PINS] = {2,3,4,5,6,7};
const int inPins[NUM_PINS]  = {A0,A1,A2,A3,A4,A5};

DHT_nonblocking dht_sensor(inPins[1], DHT_TYPE_11);

void setup() {
  Serial.begin(9600);

  for(int i = 0; i < NUM_PINS; i++){
    pinMode(outPins[i], OUTPUT);
  }
}

void loop() {
  static float temperature;
  static float humidity;

  // ALWAYS service DHT (critical)
  bool dht_ready = measure_environment(&temperature, &humidity);

  // read potentiometer
  int potValue = analogRead(inPins[0]);
  int feature = (potValue * NUM_PINS) / (MAX_POT_VAL + 1); // 0–5

  // turn everything off
  off();

  // feature selection
  switch(feature){
    case 0:
      /*digitalWrite(outPins[0], HIGH);

      if (dht_ready) {
        Serial.print("T = ");
        Serial.print(temperature, 1);
        Serial.print(" C, H = ");
        Serial.print(humidity, 1);
        Serial.println("%");
      }
      */
      debug(potValue, feature);
      break;

    case 1:
      //digitalWrite(outPins[1], HIGH);
      debug(potValue, feature);
      break;

    case 2:
      //digitalWrite(outPins[2], HIGH);
      debug(potValue, feature);
      break;

    case 3:
      //digitalWrite(outPins[3], HIGH);
      debug(potValue, feature);
      break;

    case 4:
      //digitalWrite(outPins[4], HIGH);
      debug(potValue, feature);
      break;

    case 5:
      //digitalWrite(outPins[5], HIGH);
      debug(potValue, feature);
      break;

    default:
      Serial.println("Invalid state");
  }

  delay(50); // small stability delay
}
void off(){
  // turn all pins off
  for(int i = 0; i < NUM_PINS; i++){
    digitalWrite(outPins[i], LOW);
  }
}
void debug(int i, int j){
  Serial.print("Value: ");
  Serial.print(i);
  Serial.print(" Region: ");
  Serial.println(j);
}
// DHT11 (feature 1)
static bool measure_environment(float *temperature, float *humidity)
{
  static unsigned long measurement_timestamp = 0;

  // always call measure() to advance state machine
  bool ready = dht_sensor.measure(temperature, humidity);
  if (ready) {
    measurement_timestamp = millis();
    return true;
  }
  return false;
}
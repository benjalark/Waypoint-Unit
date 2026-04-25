#include <dht_nonblocking.h>
#include <LiquidCrystal.h>

// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

const int NUM_MODES = 5;
const int MAX_POT_VAL = 1023;

const int dataPins[NUM_MODES] = {6,7,8,9,10};

DHT_nonblocking dht_sensor(dataPins[2], DHT_TYPE_11);

void setup() {
  Serial.begin(9600);
  lcd.begin(16, 2);
  for(int i = 0; i < NUM_MODES; i++){
    pinMode(dataPins[i], INPUT);
  }
}

void loop() {
  static float temperature;
  static float humidity;

  // ALWAYS service DHT (critical)
  bool dht_ready = measure_environment(&temperature, &humidity);

  // read potentiometer
  int potValue = analogRead(A0);
  int feature = (potValue * NUM_MODES) / (MAX_POT_VAL + 1); // 0–5

  // turn everything off
  off();

  // feature selection
  switch(feature){
    case 0:
      //digitalWrite(outPins[0], HIGH);

      if (dht_ready) {
        lcd.print("T = ");
        lcd.print(temperature, 1);
        lcd.print(" C\nH = ");
        lcd.print(humidity, 1);
        lcd.print("%");
      }
      
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
  for(int i = 0; i < NUM_MODES; i++){
    digitalWrite(dataPins[i], LOW);
  }
}
void debug(int value, int feature){
  static int lastFeature = -1;

  if (feature != lastFeature) {
    Serial.print("Value: ");
    Serial.print(value);
    Serial.print(" Region: ");
    Serial.println(feature);
    lastFeature = feature;
  }
}
// DHT11 (feature 1)
static bool measure_environment(float *temperature, float *humidity)
{
  static unsigned long last_output = 0;
  bool ready = dht_sensor.measure(temperature, humidity);

  if (ready && millis() - last_output > 3000ul) {
    last_output = millis();
    return true;
  }
  return false;
}
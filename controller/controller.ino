#include <dht_nonblocking.h>
#include <LiquidCrystal.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// lcd init
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//initialize gps pins and baud rate 
//for NEO-6m baud is set to default (9600)
static const int RXPin = 6, TXPin = 7;
static const uint32_t GPSBaud = 9600;

TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

// dht11 pin
const int DHT = 8;
DHT_nonblocking dht_sensor(DHT, DHT_TYPE_11);

// number of device modes
const int NUM_MODES = 5;
const int MAX_POT_VAL = 1023;

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);
  lcd.begin(16, 2);
}

void loop() {
  static float temperature;
  static float humidity;
  static int lastFeature = -1;

  // Always service DHT
  bool dht_ready = measure_environment(&temperature, &humidity);

  // read potentiometer
  int potValue = analogRead(A0);
  int feature = (potValue * NUM_MODES) / (MAX_POT_VAL + 1); // 0–5

  if (feature != lastFeature) {
    lcd.clear();           // clear screen on mode switch
    lastFeature = feature;
  }

  // update gps while it's in use
  if(1<=feature<=4){
    while (ss.available() > 0){
        gps.encode(ss.read());
    }
  }

  // feature selection
  switch(feature){
    case 0:
      if (dht_ready) {
        lcd.clear();  // clear before printing new reading
        // Line 1: Temperature
        lcd.setCursor(0, 0);
        lcd.print("T: ");
        lcd.print(temperature, 1);
        lcd.print(" C");
        // Line 2: Humidity
        lcd.setCursor(0, 1);
        lcd.print("H: ");
        lcd.print(humidity, 1);
        lcd.print(" %");
      }
      break;

    case 1:
      // This displays information every time a new sentence is correctly encoded from the neo-6m
        if (gps.location.isUpdated()){
          // latitude to 5 d.p.
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Lat="); 
          lcd.print(gps.location.lat(), 5);
          // longitude
          lcd.setCursor(0, 1);
          lcd.print(" Long="); 
          lcd.println(gps.location.lng(), 5);
        }
      break;

    case 2:
      if (gps.location.isUpdated()){
        // altitude in meters
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Altitude="); 
        lcd.print(gps.altitude.meters());
        lcd.print("m");
        // Altitude in feet
        lcd.setCursor(0, 1);
        lcd.print("Altitude="); 
        lcd.println(gps.altitude.feet());
        lcd.print("'");
      }
      break;

    case 3:
      if (gps.time.isUpdated()){
        // Raw date in DDMMYY format (u32)
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("DDMMYY=");
        lcd.println(gps.date.value());
        // Raw time in HHMMSSCC format (u32)
        lcd.setCursor(0,1);
        lcd.print("t="); 
        lcd.println(gps.time.value());
      }
      break;

    case 4:
      if (gps.location.isUpdated()){
        lcd.clear();
        // Speed in miles per hour (double)
        lcd.setCursor(0, 0);
        lcd.print("mph= ");
        lcd.println(gps.speed.mph()); 
        // Speed in meters per second (double)
        lcd.setCursor(0,1);
        lcd.print("m/s= ");
        lcd.println(gps.speed.mps());
      }
      break;

    default:
      Serial.println("Invalid state");
  }
  delay(50); // small stability delay
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
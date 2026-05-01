#include <dht_nonblocking.h>
#include <LiquidCrystal.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

// LCD
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

// GPS
static const int RXPin = 6, TXPin = 7;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

// DHT11
#define DHT_PIN 8
DHT_nonblocking dht_sensor(DHT_PIN, DHT_TYPE_11);

// Modes
#define NUM_MODES 5
#define POT_PIN A0

// State
float temperature = 0, humidity = 0;
int lastFeature = -1;
unsigned long lastDHTUpdate = 0;

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);
  lcd.begin(16, 2);
  lcd.print("Initializing...");
}

// Nice printing function
void print2(const char* l1, const char* l2) {
  lcd.setCursor(0, 0);
  lcd.print("                "); // clear line
  lcd.setCursor(0, 0);
  lcd.print(l1);

  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(l2);
}

// DHT non-blocking read
bool updateDHT() {
  if (dht_sensor.measure(&temperature, &humidity)) {
    if (millis() - lastDHTUpdate > 3000ul) { // DHT11 ~1Hz safe
      lastDHTUpdate = millis();
      return true;
    }
  }
  return false;
}

void loop() {
  // Always feed GPS parser (non-blocking)
  while (ss.available()) {
    gps.encode(ss.read());
  }

  // Read mode selector
  int feature = (analogRead(POT_PIN) * NUM_MODES) / 1024;

  // Mode change handling
  if (feature != lastFeature) {
    lcd.clear();
    lastFeature = feature;
  }

  switch (feature) {

    // MODE 0: TEMP + HUM
    case 0:
      if (updateDHT()) {
        char l1[17], l2[17];
        snprintf(l1, 17, "T: %.1f C", temperature);
        snprintf(l2, 17, "H: %.1f %%", humidity);
        print2(l1, l2);
      }
      break;

    // MODE 1: LAT/LON
    case 1:
      if (gps.location.isUpdated()) {
        char l1[17], l2[17];
        snprintf(l1, 17, "Lat: %.4f", gps.location.lat());
        snprintf(l2, 17, "Lon: %.4f", gps.location.lng());
        print2(l1, l2);
      }
      break;

    // MODE 2: ALTITUDE
    case 2:
      if (gps.altitude.isUpdated()) {
        char l1[17], l2[17];
        snprintf(l1, 17, "Alt: %.0f m", gps.altitude.meters());
        snprintf(l2, 17, "Alt: %.0f ft", gps.altitude.feet());
        print2(l1, l2);
      }
      break;

    // MODE 3: DATE/TIME
    case 3:
      if (gps.time.isUpdated() && gps.date.isUpdated()) {
        char l1[17], l2[17];
        snprintf(l1, 17, "%02d/%02d/%02d",
          gps.date.day(), gps.date.month(), gps.date.year() % 100);
        snprintf(l2, 17, "%02d:%02d:%02d",
          gps.time.hour(), gps.time.minute(), gps.time.second());
        print2(l1, l2);
      }
      break;

    // MODE 4: SPEED
    case 4:
      if (gps.speed.isUpdated()) {
        char l1[17], l2[17];
        snprintf(l1, 17, "mph: %.1f", gps.speed.mph());
        snprintf(l2, 17, "m/s: %.1f", gps.speed.mps());
        print2(l1, l2);
      }
      break;

    default:
      print2("Invalid mode", "");
  }

  delay(50);
}
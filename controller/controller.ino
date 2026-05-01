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

// DHT
#define DHT_PIN 8
DHT_nonblocking dht_sensor(DHT_PIN, DHT_TYPE_11);

// Modes
#define NUM_MODES 5
#define POT_PIN A0

// ---- Fixed-point storage ----
int temp10 = 0;   // temperature ×10
int hum10  = 0;   // humidity ×10

long lat5 = 0;    // latitude ×1e5
long lon5 = 0;    // longitude ×1e5

int alt_m = 0;
int alt_ft = 0;

int speed_mps10 = 0;
int speed_mph10 = 0;

// State
int lastFeature = -1;
unsigned long lastDHTUpdate = 0;

// ---- LCD helper ----
void print2(const char* l1, const char* l2) {
  lcd.setCursor(0, 0);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print(l1);

  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 1);
  lcd.print(l2);
}

// ---- DHT update (convert once) ----
bool updateDHT() {
  float t, h;
  if (dht_sensor.measure(&t, &h)) {
    if (millis() - lastDHTUpdate > 2000) {
      lastDHTUpdate = millis();

      temp10 = (int)(t * 10 + 0.5);
      hum10  = (int)(h * 10 + 0.5);

      return true;
    }
  }
  return false;
}

void setup() {
  Serial.begin(9600);
  ss.begin(GPSBaud);
  lcd.begin(16, 2);
  lcd.print("Initializing...");
}

void loop() {

  // ---- Always feed GPS ----
  while (ss.available()) {
    gps.encode(ss.read());
  }

  // ---- Convert GPS once ----
  if (gps.location.isUpdated()) {
    lat5 = (long)(gps.location.lat() * 100000);
    lon5 = (long)(gps.location.lng() * 100000);
  }

  if (gps.altitude.isUpdated()) {
    alt_m  = (int)gps.altitude.meters();
    alt_ft = (int)gps.altitude.feet();
  }

  if (gps.speed.isUpdated()) {
    speed_mps10 = (int)(gps.speed.mps() * 10 + 0.5);
    speed_mph10 = (int)(gps.speed.mph() * 10 + 0.5);
  }

  // ---- Mode selection ----
  int feature = (analogRead(POT_PIN) * NUM_MODES) / 1024;

  if (feature != lastFeature) {
    lcd.clear();
    lastFeature = feature;
  }

  // ---- Modes ----
  switch (feature) {

    // --- TEMP / HUM ---
    case 0: {
      if (updateDHT()) {
        char l1[17], l2[17];

        snprintf(l1, 17, "T: %d.%d C", temp10/10, abs(temp10%10));
        snprintf(l2, 17, "H: %d.%d %%", hum10/10, abs(hum10%10));

        print2(l1, l2);
      }
      break;
    }

    // --- LAT / LON ---
    case 1: {
      char l1[17], l2[17];

      snprintf(l1, 17, "Lat:%ld.%05ld",
               lat5/100000, labs(lat5%100000));

      snprintf(l2, 17, "Lon:%ld.%05ld",
               lon5/100000, labs(lon5%100000));

      print2(l1, l2);
      break;
    }

    // --- ALTITUDE ---
    case 2: {
      char l1[17], l2[17];

      snprintf(l1, 17, "Alt:%dm", alt_m);
      snprintf(l2, 17, "Alt:%dft", alt_ft);

      print2(l1, l2);
      break;
    }

    // --- DATE / TIME ---
    case 3: {
      if (gps.time.isValid() && gps.date.isValid()) {
        char l1[17], l2[17];

        snprintf(l1, 17, "%02d/%02d/%02d",
          gps.date.day(), gps.date.month(), gps.date.year() % 100);

        snprintf(l2, 17, "%02d:%02d:%02d",
          gps.time.hour(), gps.time.minute(), gps.time.second());

        print2(l1, l2);
      }
      break;
    }

    // --- SPEED ---
    case 4: {
      char l1[17], l2[17];

      snprintf(l1, 17, "mph:%d.%d",
               speed_mph10/10, abs(speed_mph10%10));

      snprintf(l2, 17, "m/s:%d.%d",
               speed_mps10/10, abs(speed_mps10%10));

      print2(l1, l2);
      break;
    }

    default:
      print2("Invalid mode", "");
  }

  delay(10);
}
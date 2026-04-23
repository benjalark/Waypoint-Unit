const int MAX_POT_VAL = 1023; // max value of the potentiometer

void setup() {
  Serial.begin(9600);
}

void loop() {
  int potMeterValue = analogRead(A0); // reading A0 pin for a potentiometer signal

  int feature = (potMeterValue * 8) / (MAX_POT_VAL + 1) + 1; // sections off signal into regions

  switch(feature){ // feature implementation
    case 1:
      break;
    case 2:
      break;
    case 3:
      break;
    case 4:
      break;
    case 5:
      break;
    case 6:
      break;
    case 7:
      break;
    case 8:
      break;
    default:
      Serial.print("Serious hardware malfunction. Check potentiometer");
  }
}
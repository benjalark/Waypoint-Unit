const int MAX_POT_VAL = 1023; // max value of the potentiometer
const int outPins[8] = {2,3,4,5,6,7,8,9};  // pins to be used for output

void setup() {
  Serial.begin(9600); // init signal processing speed in baud
  for(int i = 0; i < 8; i++){ // init output pins
    pinMode(outPins[i], OUTPUT);
  }

}

void loop() {
  int potValue = analogRead(A0); // reading A0 pin for a potentiometer signal
  int feature = (potValue * 8) / (MAX_POT_VAL + 1) + 1; // sections off signal into regions

  // turn all pins off first
  for(int i = 0; i < 8; i++){
    digitalWrite(outPins[i], LOW);
  }

  switch(feature){ // feature implementation (simple console log for the meantime)
    case 1:
      debug(potValue, feature);
      break;
    case 2:
      debug(potValue, feature);
      break;
    case 3:
      debug(potValue, feature);
      break;
    case 4:
      debug(potValue, feature);
      break;
    case 5:
      debug(potValue, feature);
      break;
    case 6:
      debug(potValue, feature);
      break;
    case 7:
      debug(potValue, feature);
      break;
    case 8:
      debug(potValue, feature);
      break;
    default:
      Serial.print("Serious hardware malfunction. Check potentiometer");
  }
  
}
void debug(int i, int j){
  Serial.print("Value: ");
  Serial.print(i);
  Serial.print(" Region: ");
  Serial.println(j);
}
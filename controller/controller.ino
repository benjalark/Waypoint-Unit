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

  off();

  switch(feature){ // feature implementation (simple console log for the meantime)
    case 1:
      off();
      digitalWrite(outPins[0], HIGH); // testing region one with hardware
      break;
    case 2:
      off();
      debug(potValue, feature);
      break;
    case 3:
      off();
      debug(potValue, feature);
      break;
    case 4:
      off();
      debug(potValue, feature);
      break;
    case 5:
      off();
      debug(potValue, feature);
      break;
    case 6:
      off();
      debug(potValue, feature);
      break;
    case 7:
      off();
      debug(potValue, feature);
      break;
    case 8:
      off();
      debug(potValue, feature);
      break;
    default:
      Serial.print("Serious hardware malfunction. Check potentiometer");
  }
  
}
void off(){
  // turn all pins off
  for(int i = 0; i < 8; i++){
    digitalWrite(outPins[i], LOW);
  }
}
void debug(int i, int j){
  Serial.print("Value: ");
  Serial.print(i);
  Serial.print(" Region: ");
  Serial.println(j);
}
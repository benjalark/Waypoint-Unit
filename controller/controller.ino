const int MAX_POT_VAL = 1024;
void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  int potMeterValue = analogRead(A0); //reads A0 pin for a potentiometer sig
  int feature; //a division of the potentiometer

  //loop to verify region of operation of the potentiometer
  for(int lowDiv = 0; lowDiv <8 ; lowDiv++){
    int highDiv = lowDiv + 1;
    feature = potStatus(lowDiv, highDiv, potMeterValue);
  }

  //executes implementation of the specific potentiometer region
  switch(feature){
    case 1:
      //exciting implementation
      break;
    case 2:
      //exciting implementation
      break;
    case 3:
      //exciting implementation
      break;
    case 4:
      //exciting implementation
      break;
    case 5:
      //exciting implementation
      break;
    case 6:
      //exciting implementation
      break;
    case 7:
      //exciting implementation
      break;
    case 8:
      //exciting implementation
      break;
    default:
      Serial.print("Serious hardware malfunction. Check potentiometer");
  }
}
// a function to return which region (of 8) the potentiometer is set to
int potStatus(int l, int u, int value){
  if (MAX_POT_VAL / 8 * l <= value <= MAX_POT_VAL / 8 * u){
    return u;
  } else { // never occurs (hopefully)
    return 0;
  }
}
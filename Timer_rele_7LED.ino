
//Library
#include "SevenSeg.h"

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
const int relePin = 12;     // Rele variables
const int dotPin = 4;

//Defines the segments A-G: SevenSeg(A, B, C, D, E, F, G);
SevenSeg disp (5,6,7,8,9,10,11);
const int numOfDigits = 1;  //Number of 7 segments
int digitPins [numOfDigits]={4};  //CC(or CA) pins of segment

//Variables
int _number = 0; //Default number

// variables will change:
int _buttonState = 0;         // variable for reading the pushbutton status

//timer variables
int _count = 0; // number of pushing the button

long int _timing = 0;
long int _timingButton = 0;
long int _initButtonTiming = 0;

// Contant that won't change
const long int _timeScale_ = 600000; // 10min each unit
const int _blinkTimer_ = 500; // timing for dot blinking
long int _delayReadingButton_ = 2000;




// SET UP --------------------------------------------------------------

void setup() {
  
  Serial.begin(9600);

  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
  
  //Defines the number of digits to be "numOfDigits" and the digit pins to be the elements of the array "digitPins"
  // disp.setDigitPins ( numOfDigits , digitPins ); // We spare one pin conecting directly sour 5V
  //Only for common cathode 7segments
  disp.setCommonAnode();
  //Control brightness (values 0-100);
  disp.setDutyCycle(50);
  disp.setDPPin(dotPin);

  //Rele
  pinMode(relePin, OUTPUT);
  releON(false);
  
}

// ----------------------------------------------------------- END SET UP


void updateLED(int n){
  if (n>=0 && n<=9){  //Valid range is from 0 to 9
    disp.writeDigit(n);  //Print number to 7 segment display
  }
  else {
    disp.clearDisp();
  }
}

bool isButtonPressed(){
  // read the state of the pushbutton value:
  _buttonState = digitalRead(buttonPin);
  if (_buttonState == HIGH){
    disp.clearDP();
    return true;
  }
  return false;
}


bool blinkLED(bool up){
  if(up){
    disp.setDP();
    return false;
  }
  else{
    disp.clearDP();
    return true;
  }
}


void releON(bool s){
  if (s == true){
    digitalWrite(relePin, HIGH);
  }
  else{
    digitalWrite(relePin, LOW);
  }
}


void buttonWatcher(){
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (isButtonPressed()) {
    Serial.println("BUTTON PRESSED---------------------------------------");
    _initButtonTiming = millis();
    _count = _count + 1;
    updateLED(_count);
    if (_count == 10){
      _count = 0;
      updateLED(_count);
    }
    Serial.print("Count:"); Serial.println(_count);
    delay(200);
    while(_timingButton < _delayReadingButton_){
      buttonWatcher();
    }
    _timingButton = 0;
  }
  _timingButton = millis() - _initButtonTiming;
}

bool runTimer(){
  int stepTime = 10;
  bool light = true;
  long int t0 = 0;
  long int timer = 0;

  timer = _count * _timeScale_;
  t0 = timer;
  while (timer >= 0){
    if ((t0-timer)/_blinkTimer_ > 0){
      t0 = timer;
      light = blinkLED(light);
    }
    if (isButtonPressed()){
      Serial.println("Button pressed, exit from loop");
      buttonWatcher();
      break;
    }
    delay(stepTime);
    _count = timer/_timeScale_;
    updateLED(_count);
    timer = timer - stepTime;
  }
  Serial.println("runTimer ended");
  return true;
}

void loop()
{
  _count = -1;
  Serial.println("Init loop");
  Serial.print("Count: "); Serial.println(_count);

  buttonWatcher();

  if (_count > 0){
    releON(true);
    runTimer();
  }
  releON(false);
  _timingButton = 0;
  disp.clearDisp();
  Serial.println("End");
}

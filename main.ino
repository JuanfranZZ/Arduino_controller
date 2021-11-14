
//Library
#include "SevenSeg.h"

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 2;     // the number of the pushbutton pin
int count = 0; // number of pushing the button

//Defines the segments A-G: SevenSeg(A, B, C, D, E, F, G);
SevenSeg disp (5,6,7,8,9,10,11);
//Number of 7 segments
const int numOfDigits = 1;
//CC(or CA) pins of segment
int digitPins [numOfDigits]={4};

//Variables
int number=0; //Default number
int flag;

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

//timer variables
int delayStart = 0;
long int timing = 0;
const int dotPin = 4;
int delayFin = 3000;
bool finish = true;
bool starting = false;
int timeScale = 2000;


// Rele variables
const int relePin = 12;

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

  //Timer
  delayStart = millis();

  //Rele
  pinMode(relePin, OUTPUT);
  releON(false);
  
}

// ----------------------------------------------------------- END SET UP


void showLED(int number){
  //Valid range is from 0 to 9
  if (number>=0 && number<=9){
    //Print number to 7 segment display
    disp.writeDigit(number);
      //Print message to serial monitor only once
      if (flag==0){ 
        flag=1;
      }
  }
  else {
    disp.clearDisp();
  }
}

void resetTiming(){
  delayStart = millis();
}

bool isButtonPressed(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  if (buttonState == HIGH){
    resetTiming();
    disp.clearDP();
    return true;
  }
  return false;
}


void buttonCounter(){
  Serial.print("Setting number:");
  Serial.println(count);
  if (count == 10){
    count = 0;
  }
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (isButtonPressed()) {
    count = count + 1;
    delay(200);
  }
}


bool isDelayFin(int delaySelected){
  timing = millis() - delayStart;
  if (timing > delaySelected){
    return true;
  }
  return false;
}


bool runTimer(int numbering){
  int stepTime = 50;
  bool light = true;
  int t0 = 0;
  int timer = 0;

  timer = numbering * timeScale;
  t0 = timer;
  while (!isButtonPressed() && timer >= 0){
    if ((t0-timer)/500 > 0){
      t0 = timer;
      light = blinkLED(light);
    }
    if (timer/1000 == 0){
      Serial.println("exit");   
      delay(1000);   
      return true;
    }
    delay(stepTime);
    count = timer/timeScale;
    showLED(count);
    timer = timer - stepTime;
  }
  Serial.println("Button pressed");
  return true;
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


void loop()
{
  Serial.println("Starting...");
  count = -1;
  starting = false;
  while (!isDelayFin(delayFin) or starting == false){
    buttonCounter();
    showLED(count);
    starting = true;
  }
  while (isDelayFin(delayFin) && (count >= 0) && starting){
    finish = false;
    starting = true;
    while (!finish){
      releON(true);
      finish = runTimer(count);
    }
    starting = false;
    releON(false);
  }

}
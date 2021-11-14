/* Arduino Tutorial - 7 Segment 
   One 7 segment is one digit, from 0 to  9.
   Dev: Vasilakis Michalis // Date: 25/7/2015 // www.ardumotive.com */

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
  
}

void loop()
{ 
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  
  if (count == 10){
    count = 0;
    
  }
  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    count = count + 1;
    delay(200);
    
  }

  //Valid range is from 1 to 9
  number = count;
  if (number>=1 && number<=9){
    //Print number to 7 segment display
    disp.writeDigit(number);
    
      //Print message to serial monitor only once
      if (flag==0){ 
        //Print number to serial monitor
        Serial.print("Number on 7 segment display:");
        Serial.println(number);
        flag=1;
      }
  }
  else {
    disp.clearDisp();
  }
}

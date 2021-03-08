
#include <TimerOne.h>
#include "engine.h"
arp a(C, 5, 2, 6, 200, c_harmonic, 0);
bool button_pressed;
int ButtonVal;
//int LastButtonVal;
int ButtonClicks;

// A6 = baseNotepin       ROOT
// A5 = baseOctavepin     TONIC      
// A4 = octaveShiftpin    PROGR.
// A3 = modepin           MODE     broken
// A2 = indelaypin        TEMPO
// A1 = orderpin          ORDER
// A0 = stepspin          STEPS


#define baseNotepin 6     // 
#define baseOctavepin 5   // 
#define octaveShiftpin 4  // 
#define modepin 7         //      broken, pin 7 temp.
#define indelaypin 2      // 
#define orderpin 1        // 
#define stepspin 0        //

#define LEDPin 5

// Synchronization: choose one of two possible options:
//#define EXT_SYNC // korg synx
#define INT_SYNC // pot tempo

void readPoties()
{
  unsigned i;
  a.setupArp(analogRead(baseNotepin), analogRead(baseOctavepin), analogRead(octaveShiftpin), analogRead(stepspin), analogRead(indelaypin), analogRead(orderpin), analogRead(modepin));
  
  // In my setup the buttons are connected to pins 6..12
  for (i=12;i>5;i--)
    if (!(digitalRead(i))) { 
      button_pressed = true; ButtonVal = 13-i;
      if (ButtonVal == 1) { // reverse button order
        ButtonVal = 7;
        ButtonClicks = (ButtonClicks+1);
      } else if (ButtonVal == 2) {
        ButtonVal = 6;
        ButtonClicks = 0;
      } else if (ButtonVal == 3) {
        ButtonVal = 5;
        ButtonClicks = 0;
      } else if (ButtonVal == 4) {
        ButtonVal = 4;
        ButtonClicks = 0;
      } else if (ButtonVal == 5) {
        ButtonVal = 3;
        ButtonClicks = 0;
      } else if (ButtonVal == 6) {
        ButtonVal = 2;
        ButtonClicks = 0;
      } else if (ButtonVal == 7) {
        ButtonVal = 1;
        ButtonClicks = 0;
      }
      return;
      }
}

void setup()
{
  a.midibegin();
  Timer1.initialize(200000);          // initialize timer1, and set a 1/10 second period
  Timer1.attachInterrupt(readPoties); // We will read potis and buttons values within timer interrupt
  
  // LED pin
  pinMode(LEDPin, OUTPUT);
  
  // Initialize pins for 2-pins pushbuttons with pullup enabled
  for (unsigned i=6;i<13;i++)
  {
    pinMode(i,INPUT_PULLUP);
    //pinMode(i, INPUT);
    //digitalWrite(i, 1);
  }
  button_pressed = false;
  ButtonVal = 1;
  //LastButtonVal = 0;
  ButtonClicks = 0;
}

void loop()
{
    if (button_pressed)
    {
      a.setProgression(ButtonVal-1);

     if((ButtonVal == 7) && (ButtonClicks > 2)){
         button_pressed = false;
         ButtonClicks = 0;
         //LastButtonVal = 0;
      }
      
      digitalWrite(LEDPin, HIGH);
      a.play();
      
      digitalWrite(LEDPin, LOW);
    }
    
}

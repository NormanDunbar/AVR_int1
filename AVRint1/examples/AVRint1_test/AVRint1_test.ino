//============================================================
// AVRint1 Demonstration.
//============================================================
// Attach an LED to Arduino pin D13, AVR pin PB5
// and toggle it in the loop.
//
// Another LED on Arduino pin D5, AVR pin PD5, will be
// toggled when a switch is pressed. The switch
// MUST be attached to Arduino pin D3, AVR pin PD3.
//
// For the sake of this test, debouncing is not used so 
// some flickering is to be expected. (Use long presses!)
//
// Norman Dunbar
// 7th August 2020.
//============================================================
//
//      R1
//  *--10K--*-----------------------------------> GND
//  |   
//  *--10K--*
//  |  R2   |
//  |       |     _|_  Switch
//  +-------|----*   *--------------------------> D3
//          |        |
//     C1  ===       |
//  100 nF ===       |
//          |        |
//          +--------+
//
// GND to R1 pin 1.
// R1 pin 2 to R2 pin 1 and switch pin 1.
// R2 pin 2 to C1 pin 1.
// C1 pin 2 to switch pin 2 and Arduino pin D3.
// Arduino pin D3 is configured as INPUT_PULLUP.
//
// D2 reads LOW when switch pressed.
//============================================================

#include "AVR_int1.h"


// Keep a count of the number of times, the ISR function
// has been called.
volatile byte disableCount = 0;


// My toggling function for the ISR to call. It should 
// be kept short and sweet.
void toggleLED() {
    digitalWrite(5, !digitalRead(5));
    //disableCount++;
}



void setup() {
    pinMode(13, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(3, INPUT_PULLUP);
    
    // Attach my function to the INT1 interrupt.
    AVRint1.onInterruptTriggered(toggleLED);

    // Enable the interrupt on RISING stimulus this
    // requires the switch to be INPUT PULLUP, the
    // toggle will occur when the switch is RELEASED.
    AVRint1.enable(int1::TRGR_RISING);
}

void loop() {
        // Disable interrupt when disableCount gets to 10.
        // Light up the built in LED to indicate this. 
        // We also extinguish the toggled LED on D5.
        if (disableCount >= 10) {
            AVRint1.disable();
            digitalWrite(5, LOW);
            digitalWrite(13, HIGH);

            while (1) ;     // Hang up the board.
        }

        // Toggle built in LED on D13/PB5.
        digitalWrite(13, !digitalRead(13));
        delay(1000);
}

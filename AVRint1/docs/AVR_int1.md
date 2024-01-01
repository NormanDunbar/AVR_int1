# AVRint1 Library

This library can be usd with the Arduino IDE and also with other systems, such as PlatformIO. The library makes handling interrupts on the Arduino D3 pin, AVR pin PD3, very simple indeed.

## Library Documentation

### Namespaces

This library uses the *int1* namespace.

### Types

#### onInterrupt

This type defines the function that will be called when the interrupt is triggered. The function must be as follows:

```
void interruptFunction() {
    // Do something on interrupt here;
}
```

It will be called if, and only if, it has been attached to the interrupt by the `onInterruptTriggered()` function. See below for details.


#### trigger_t

The `trigger_t` type defines the 4 different triggers that can be passed to the `enable()` function. The different values are:

* **int1::TRGR_LOW**    The interrupt will trigger when pin D3 is help `LOW`.
* **int1::TRGR_CHANGE** The interrupt fires every time pin D3 changes -- from `LOW` to `HIGH` or from `HIGH` to `LOW`.
* **int1::TRGR_FALLING**    The interrupt triggers whenever pin D3 changes from `HIGH` to `LOW`.
* **int1::TRGR_RISING** The interrupt triggers whenever pin D3 changes from `LOW` to `HIGH`.


### Functions

**`void AVRint1.enable(trigger_t trigger)`**

Enables the interrupt and should be called when the `onInterruptTriggered()` function has been called to set up a function to be called. The interrupt will be handled from this point onwards.

For example, to enable the interrupt whenever pin D3 changes state, use:

```
AVRint1.enable(AVRint1.TRGR_CHANGE);
```

**`void AVRint1.disable()`**

Disables interrupt handling. This may be premanently, or temporarily. To re-enable the interrupt handling, simply call `enable()` again with a suitable trigger.

For example:

```
AVRint1.disable();
```

**`bool AVRint1.isEnabled()`**

Returns `true` if the interrupt has been enabled. For example:

```
if (AVRint1.enabled())
    AVRint1.disable();
```

**`bool AVRint1.isTriggered()`**

Interrupts, while disabled, can be triggered but while this doesn't fire the interrupt function (they are disabled after all) the interrupt flag is set internally in the microcontroller.

If the interrupt handler is subsequently enabled, the interrupt will *immediately* fire -- which might not be the desired action.

Calling `enable()` always clears the interrupt flag to prevent this. Your code can query if the flag is set as follows:

```
if (AVRint1.isTriggered()) {
    // Oh dear, calling enable() will fire immediately.
    // Maybe I should clear the interrupt flag here?
    ...
}
```

**`void AVRint1.resetFlagBit()`**

This function will reset the interrupt flag internally to the microcontroller. As mentioned above, calling `enable()` will always clear the flag, but your code can also clear it, if required.

```
if (AVRint1.isTriggered()) {
    AVRint1.resetFlagBit();
    ...
}
```

**`void AVRint1.onInterruptTriggered(onInterrupt functionName)`**

This function attaches a function in your code to the INT1 interrupt. The function will be called each time the interrupt is triggered.

```
void myFunction() {
    digitalWrite(13, HIGH);
}

void setup() {
...
   AVRint1.onInterruptTriggered(myFunction);
...
}
```

**`void AVRint1.interrupt()`**

This function is intended for internal use only. It is what allows the ISR (Interrupt Service Routine) for INT1 to execute the function passed to `onInterruptTriggered()`.

The `interrupt()` function can be called from user code, as a means of manually executing the interrupt function, but this feature should be used with great care.

The function has to be `public` because the ISR isn't part of the `AVRint1` object, and cannot be. (Unless you know different?)


## Steps in Setting Up the Interrupt

Once the library has been installed on your system, see `README.md` for details, it is simple to use. All you are required to do is:

* `#include AVR_int1.h`;
* Write a `void` function which takes no parameters to handle the interrupt;
* Call `AVRint1.enable()`  with a suitable trigger;
* That's it!

Taking each step in order, we have....

### The Header file

Adding the header file is simple. In your main source file, just add the following line at the top somewhere:

```
#include "AVR_int1.h"
```

That's all. When you do this, a new object is created in the background. This is named `AVRint1` (note the lack of an underscore in the name!) and this object controls the interrupts. At the moment, however, it is disabled and not yet configured.


### The Interrupt Function

You need to write a function to handle the interrupt, when one occurs. The function should look like this:

```
void myInterruptFunction() {
    // Do something ...
}
```

The function takes no parameters and does not return a result.

To add this function to the interrupt requires the following line:

```
AVRint1.onInterruptTriggered(myInterruptFunction);
```

Note, there are no parenthesis on the end of `myInterruptFunction` in the above. That would not work!

All that is required now is to enable the interrupt, and tell it when to trigger.


### Enabling the Interrupt

The interrupt can be triggered by 4 different triggers:

* A `LOW` on pin D3;
* D3 changing from `LOW` to `HIGH` or from `HIGH` to `LOW` -- a `CHANGE` trigger.;
* D3 changing from `LOW` to `HIGH` - a `RISING` change; 
* D3 changing from `HIGH` to `LOW` - a `FALLING` change; 

Of these, the latter three are quite useful. The first is *interesting* in that it continues to fire the interrupt, and hence, your interrupt function, as long as the pin is held `LOW`. The other three trigger the interrupt, and your function, only when the change takes place.

## Example Sketches

The following code shows an example of using this interrupt to toggle an LED.
### Example Arduino Sketch

```
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

#include "AVR_int1.h"


// My toggling function for the ISR to call. It should 
// be kept short and sweet.
void toggleLED() {
    digitalWrite(5, !digitalRead(5));
}



void setup() {
    pinMode(13, OUTPUT);
    pinMode(5, OUTPUT);
    pinMode(2, INPUT_PULLUP);
    
    // Attach my function to the INT1 interrupt.
    AVRint1.onInterruptTriggered(toggleLED);

    // Enable the interrupt on FALLING stimulus this
    // requires the switch to be INPUT PULLUP.
    AVRint1.enable(int1::TRGR_FALLING);
}

void loop() {
        // Toggle built in LED on D13/PB5.
        digitalWrite(13, !digitalRead(13));
        delay(1000);
}
```

### Example for PlatformIO

```
#include "AVR_int1.h"
#include <util/delay.h>

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


// My toggling function for the ISR to call. It should 
// be kept short and sweet.
void toggleLED() {
    PIND |= (1 << PIND5);
}


int main() {

    // SETUP:
    DDRB |= (1 << DDB5);        // D13/PB5 is OUTPUT.
    PORTB &= ~(1 << PORTB5);    // D13/PB5 is OFF/LOW.

    DDRD &= ~(1 << DDD3);       // D3/PD3 is INPUT ...
    PORTD |= (1 << PORTD3);     // D3/PD3 is INPUT PULLUP.

    DDRD |= (1 << DDD5);        // D5/PD5 is OUTPUT.
    PORTD &= ~(1 << PORTD5);    // D5/PD5 is OFF/LOW.


    // Attach my function to the INT1 interrupt.
    AVRint1.onInterruptTriggered(toggleLED);

    // Enable the interrupt on FALLING stimulus this
    // requires the switch to be INPUT PULLUP.
    AVRint1.enable(int1::TRGR_FALLING);

    // Enable Global interrupts.
    sei();


    // LOOP:
    while (1) {
        // Toggle built in LED on D13/PB5.
        PINB |= (1 << PINB5);
        _delay_ms(1000);
    }
}
```

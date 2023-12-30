#include "AVR_int1.h"

namespace int1 {

// Constructor. Set up the base class details
// to define an INT1 handler. We use:
// INT1 as the enable bit,
// INTF1 as the interrupt flag bit,
// ISC11:0 as the trigger bit mask.
AVR_int1::AVR_int1() 
    : intbase::AVR_int_base(INT1, INTF1, ((1 << ISC11)|(1 << ISC10)) ) {}

} // end of namespace.


// Define our one instance of AVR_int1 class.
int1::AVR_int1 AVRint1;


// This is the ISR for the INT0 interrupt. It will
// call the required user function if there is one.
ISR(INT1_vect) {
    AVRint1.interrupt();
}


#ifndef INT1_H
#define INT1_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "AVR_int_base.h"

namespace int1 {

       // Enumeration of the 4 different INT1
    // triggering stimuli.
    typedef enum trigger : uint8_t {
        TRGR_LOW = 0,
        TRGR_CHANGE = 4,
        TRGR_FALLING = 8,
        TRGR_RISING = 12
    } trigger_t ;

    class AVR_int1 : public intbase::AVR_int_base {

    public:    

        // Constructor. There's no destructor as the one
        // AVRint1 object will never be destructed.
        AVR_int1();
    };

} // end of namespace.

//There will be one of these in the cpp file for this class.
extern int1::AVR_int1 AVRint1;

#endif // INT1_H

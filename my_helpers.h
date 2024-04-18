#ifndef MY_HELPERS_H 
#define MY_HELPERS_H 

#include <stddef.h>

#define INTERRUPT_GlobalInterruptEnable()       (INTCONbits.GIE = 1)
#define INTERRUPT_GlobalInterruptDisable()      (INTCONbits.GIE = 0)
#define INTERRUPT_PeripheralInterruptEnable()   (INTCONbits.PEIE = 1)
#define INTERRUPT_PeripheralInterruptDisable()  (INTCONbits.PEIE = 0) 

#define ATOMIC_LOCK()   INTERRUPT_GlobalInterruptDisable()
#define ATOMIC_UNLOCK() INTERRUPT_GlobalInterruptEnable()


#define RC5_TOGGLE()    LATCbits.LATC5 = ~LATCbits.LATC5;


void dump_buffer(const void *buffer, size_t size);


#endif
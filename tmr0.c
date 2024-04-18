#include "tmr0.h"
#include <xc.h>


void tmr0_init() {

    // T0CS FOSC/4; T0CKPS 1:1024; T0ASYNC synchronised;
    T0CON1 = 0x40;

    // TMR0H 243;
    TMR0H = 0xF3;

    // TMR0L 0;
    TMR0L = 0x00;

    // Clear Interrupt flag before enabling the interrupt
    PIR0bits.TMR0IF = 0;

    // Enabling TMR0 interrupt.
    PIE0bits.TMR0IE = 1;

    // T0OUTPS 1:1; T0EN enabled; T016BIT 8-bit;
    T0CON0 = 0x80;
}
#ifndef switches_included
#define switches_included

#define SW1 BIT0
#define SW2 BIT1
#define SW3 BIT2
#define SW4 BIT3

#define SWITCHES (SW1 | SW2 | SW3 | SW4)

#include "msp430.h"

extern char switch_state_down,
            switch_state_down2,
            switch_state_down3,
            switch_state_down4; /* effectively boolean */
extern char stat;
extern int redrawScreen;

unsigned int p2sw_read();
void p2sw_init(unsigned char mask);

#endif // included

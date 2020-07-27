#include <msp430.h>
#include "p2switches.h"
#include "lcdutils.h"
#include "lcddraw.h"
static unsigned char switch_mask;
static unsigned char switches_last_reported;
static unsigned char switches_current;
char switch_state_down,
            switch_state_down2,
            switch_state_down3,
            switch_state_down4; /* effectively boolean */
char stat = 0;
  
static void
switch_update_interrupt_sense()
{
  switches_current = P2IN & switch_mask;
  /* update switch interrupt to detect changes from current buttons */
  P2IES |= (switches_current);  /* if switch up, sense down */
  P2IES &= (switches_current | ~switch_mask); /* if switch down, sense up */
}

void 
p2sw_init(unsigned char mask)
{
  switch_mask = mask;
  P2REN |= mask;    /* enables resistors for switches */
  P2IE = mask;      /* enable interrupts from switches */
  P2OUT |= mask;    /* pull-ups for switches */
  P2DIR &= ~mask;   /* set switches' bits for input */

  switch_update_interrupt_sense();
}

/* Returns a word where:
 * the high-order byte is the buttons that have changed,
 * the low-order byte is the current state of the buttons
 */
unsigned int 
p2sw_read() {
  unsigned int sw_changed = switches_current ^ switches_last_reported;
  switches_last_reported = switches_current;
  return switches_current | (sw_changed << 8);
}
void
switch_interrupt_handler()
{
  u_int switches = p2sw_read();

  switch_state_down =  (~switches & BIT0) ? 1 : 0; /* 0 when SW1 is up */
  switch_state_down2 = (~switches & BIT1) ? 1 : 0;
  switch_state_down3 = (~switches & BIT2) ? 1 : 0;
  switch_state_down4 = (~switches & BIT3) ? 1 : 0;
  
  /*handle all 4 buttons, if the button is push switch_statedownN = 1 otherwise is 0*/
  
  
  if(switch_state_down){
    stat = 1;
  }
  if(switch_state_down2){
    stat = 2;
  }
  if(switch_state_down3){
    stat = 3;
  }
  if(switch_state_down4){
    stat = 4;
  }  
}

/* Switch on P2 (S1) */
void
__interrupt_vec(PORT2_VECTOR) Port_2(){
  if (P2IFG & switch_mask) {  /* did a button cause this interrupt? */
    P2IFG &= ~switch_mask;	/* clear pending sw interrupts */
    switch_update_interrupt_sense();
    switch_interrupt_handler();	/* single handler for all switches */
  }
}

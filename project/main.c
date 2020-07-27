#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include "p2switches.h"
#include <shape.h>
#include <abCircle.h>
#include "buzzer.h"
#include "stateMachine.h"
#include "toggle.h"

#define GREEN_LED BIT6
#define RED_LED BIT0

int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

int col = 10;
int row = 90;
int state = 0;
/** Initializes everything, enables interrupts and green LED, 
 *  and handles the rendering for the screen
 */
void main()
{
    
  P1DIR |= GREEN_LED;		/**< Green led on when CPU on */		
  P1OUT |= GREEN_LED;

  configureClocks();
  lcd_init();
  shapeInit();
  p2sw_init(15);
  buzzer_init();
  shapeInit();
  
  clearScreen(COLOR_WHITE);

  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

  for(;;) { 
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
      
      or_sr(0x10);	      /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;

    fillRectangle(col,row, 20, 20, COLOR_ORANGE);   
  }  
}

/** Watchdog timer interrupt handler. 15 interrupts/sec */
void wdt_c_handler()
{
  static short count = 0;
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count ++;
  if (count == 15) {
   moveShape();

   
   buttonState();// assembly function

   count = 0;
  }
   P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}

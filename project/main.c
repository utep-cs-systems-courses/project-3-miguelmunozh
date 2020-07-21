#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>
#include "buzzer.h"
//#include "stateMachine.h"
#define GREEN_LED BIT6

int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

int col = 10;
int row = 90;
int state =0;
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
    u_int switches = p2sw_read(), i;
    char str[5];
    for (i = 0; i < 4; i++){
      str[i] = (switches & (1<<i)) ? '-' : '0'+i;
    }
  static short count = 0;
  P1OUT |= GREEN_LED;		      /**< Green LED on when cpu on */
  count ++;
  if (count == 15) {

    /**state machine to move the SQUARE and paint my first initial */
switch(state){
  case 0:
     row--;
      if(row == 30){
	state = 1;
      }      
    break;
  case 1:
    	col++;
	row++;
	if(col==50){
	  state = 2;
      }
    break;
  case 2:
    	col++;
	row--;
	if(col==90){	
	  state = 3;
      }
    break;
  case 3:
    row++;
	if(row==90){	
	  col = 10;
	  row = 90;
	  state = 0;
	}
    break;
 }

/**control what the butons do when they are pressed*/
    if(str[0]=='0'){
      buzzer_set_period(0);
      redrawScreen = 1; //not update the screen,
    }
    if(str[1]=='1'){
     buzzer_set_period(0);
     customShape(COLOR_BLACK,COLOR_WHITE, COLOR_RED,COLOR_PINK);
    }
    if(str[2]=='2'){
      buzzer_set_period(0);
      customShape(COLOR_WHITE,COLOR_BLACK,COLOR_PINK,COLOR_TURQUOISE);
    }
    if(str[3]=='3'){
      drawCustomString();
    }
    count = 0;
  } 
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}

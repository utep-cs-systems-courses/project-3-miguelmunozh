#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include "p2switches.h"
#include <shape.h>
#include <abCircle.h>
#include "buzzer.h"
#include "stateMachine.h"
#include "led.h"

#define GREEN_LED BIT6
#define RED_LED BIT0

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
  led_init();
  
  clearScreen(COLOR_WHITE);

  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */

  //red_on =1;
  for(;;) { 
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
    //P1OUT &= ~RED_LED;      /**< Ren led on when CPU on */
    
      or_sr(0x10);	      /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    //P1OUT |= RED_LED;       /**< Reen led on when CPU on */
    redrawScreen = 0;

    fillRectangle(col,row, 20, 20, COLOR_ORANGE);   
  }  
}

/** Watchdog timer interrupt handler. 15 interrupts/sec */
void wdt_c_handler()
{
  //u_int switches = p2sw_read();
  
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

 btnState();


 /* switch(stat){
  case 1:
     stopSound();      //function in assembly to stop sounds
     redrawScreen = 1; // update the screen,
     stat = 1;
    break;
  case 2:
     stopSound();
     customShape(COLOR_BLACK,COLOR_WHITE, COLOR_RED,COLOR_PINK);
     stat = 0;
    break;
  case 3:
      customShape(COLOR_WHITE,COLOR_BLACK,COLOR_PINK,COLOR_TURQUOISE);
      soundEffect();
      stat = 0;
    break;
  case 4:
      drawCustomString();
      stat = 4;
    break;
 }
 */
   /**control what the butons do when they are pressed*/
 /*if(stat == 1){
      stopSound();      //function in assembly to stop sounds
      redrawScreen = 1; // update the screen,
      stat = 0;
    }
    if(stat == 2){
     stopSound();
     customShape(COLOR_BLACK,COLOR_WHITE, COLOR_RED,COLOR_PINK);
     stat = 0;
    }
    
    if(stat == 3){
      customShape(COLOR_WHITE,COLOR_BLACK,COLOR_PINK,COLOR_TURQUOISE);
      soundEffect();
      stat = 0;
    }
     
    if(stat == 4){
      drawCustomString();
      
      }*/
   count = 0;
  }
   P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}

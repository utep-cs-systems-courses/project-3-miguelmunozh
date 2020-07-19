/** \file shapemotion.c
 *  \brief This is a simple shape motion demo.
 *  This demo creates two layers containing shapes.
 *  One layer contains a rectangle and the other a circle.
 *  While the CPU is running the green LED is on, and
 *  when the screen does not need to be redrawn the CPU
 *  is turned off along with the green LED.
 */  
#include <msp430.h>
#include <libTimer.h>
#include <lcdutils.h>
#include <lcddraw.h>
#include <p2switches.h>
#include <shape.h>
#include <abCircle.h>

#define GREEN_LED BIT6


u_int bgColor = COLOR_BLUE;     /**< The background color */
int redrawScreen = 1;           /**< Boolean for whether screen needs to be redrawn */

Region fieldFence;		/**< fence around playing field  */
char color = COLOR_BLACK;
int firstLine = 1;
int secondLine = 0;
int thirdLine = 0;
int fourthLine = 0;
int col = 10;
int row = 90;
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

  shapeInit();
  clearScreen(COLOR_BLUE);
  //layerInit(&layer0);
  //layerDraw(&layer0);


  //layerGetBounds(&fieldLayer, &fieldFence);


  enableWDTInterrupts();      /**< enable periodic interrupt */
  or_sr(0x8);	              /**< GIE (enable interrupts) */


  for(;;) { 
    while (!redrawScreen) { /**< Pause CPU if screen doesn't need updating */
      P1OUT &= ~GREEN_LED;    /**< Green led off witHo CPU */
      or_sr(0x10);	      /**< CPU OFF */
    }
    P1OUT |= GREEN_LED;       /**< Green led on when CPU on */
    redrawScreen = 0;

    //clearScreen(COLOR_BLUE); not here because this loops runs forever
    fillRectangle(col,10, 50, 50, COLOR_ORANGE);
   
    // movLayerDraw(&ml0, &layer0);
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
  
    if(firstLine == 1){
      row--;
      if(row == 30){
	firstLine =0;
	secondLine = 1;
	}      
     }
    if(secondLine == 1){
	col++;
	row++;
	if(col==50){	
	  secondLine =0;
	  thirdLine =3;
      }
    }
    if(thirdLine == 3){
	col++;
	row--;
	if(col==90){	
	  thirdLine =0;
	  fourthLine = 4;
      }
    }
    if(fourthLine == 4){	
	row++;
	if(row==90){	
	  fourthLine = 0;
	  //clearScreen(COLOR_BLUE);
	
	  col = 10;
	  row = 90;
	  firstLine = 1; // to keep drawing it over and over again
       }
    }
  
    //mlAdvance(&ml0, &fieldFence);
    if(str[0]=='0'){
     //clear screen and redraw the figure with a diferent position, same problem than above
      //clearScreen(COLOR_BLUE);
      redrawScreen = 1; //not update the screen,
    }
    if(str[1]=='1'){
     
     clearScreen(COLOR_BLUE);
     fillRectangle(20,20, 50, 50, bgColor); // another figure
     
    }
    if(str[2]=='2'){
      //bgColor = COLOR_WHITE;
     clearScreen(COLOR_RED);
     fillRectangle(20,20, 50, 50, COLOR_WHITE);
    }
    //clear screen and write something to the screen
    count = 0;
  } 
  P1OUT &= ~GREEN_LED;		    /**< Green LED off when cpu off */
}

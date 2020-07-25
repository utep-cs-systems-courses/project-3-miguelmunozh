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

void btnState(){
 switch(stat){
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
}

/** song played when showing */
void demoSong(){
  static char state =0;
  switch(state){
  case 0:
    //play a tone
    buzzer_set_period(100);
    state =1;
    break;
  case 1:
    buzzer_set_period(200);
    state = 2;
    break;
  case 2:
    buzzer_set_period(300);
    state =3;
    break;
  case 3:
    buzzer_set_period(400);
    state = 4;
    break;
  case 4:
    buzzer_set_period(500);
    state = 5;
    break;
  case 5:
    buzzer_set_period(600);
    state = 6;
    break;
  case 6:
    buzzer_set_period(700);
    state = 7;
    break;
  case 7:
    buzzer_set_period(800);
    state = 8;
    break;
  case 8:
    buzzer_set_period(900);
    state = 9;
    break;
  case 9:
    buzzer_set_period(1000);
    state = 10;
    break;
  case 10:
    buzzer_set_period(900);
    state = 11;
    break;
  case 11:
    buzzer_set_period(800);
    state = 12;
    break;
  case 12:
    buzzer_set_period(700);
    state = 13;
    break;
  case 13:
    buzzer_set_period(700);
    state = 14;
    break;
  case 14:
    buzzer_set_period(600);
    state = 15;
    break;
  case 15:
    buzzer_set_period(500);
    state = 0;
    break;
 }
}

/*creates a sound effect for inverting colors*/
void soundEffect(){
  for(int i =1000 ;i<10000;i++){                
	for(int j =0 ; j<10; j++){
	  buzzer_set_period(i+10);
	}
      }
        for(int i =10000 ;i>1000;i--){                
	for(int j =100 ; j>0; j--){ 
	  buzzer_set_period(i-10);
	}
      }
      buzzer_set_period(0);
}
/*
void toggle_red(){
  red_on = 1;
}
*/

#include "stateMachine.h"
#include "buzzer.h"
/***/
void demoSong(){
  static char state = 0;
  switch(state){
  case 0:
    buzzer_set_period(1000);
    state=1;
    break;
  case 1:
    buzzer_set_period(2000);
    state = 2;
    break;
  case 2:
    buzzer_set_period(4000);
    state = 3;
    break;
  case 3:
    buzzer_set_period(5000);
    state = 0;
    break;
 }
}

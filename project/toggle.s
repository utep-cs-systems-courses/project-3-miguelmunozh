	.arch msp430g2553
	
	.data
state:				;state
	.word 0

	.text
jt:				;jump table
	.word default
	.word case1
	.word case2
	.word case3
	.word case4

	
	.global toggle

toggle:
	cmp #5, &state 		;range check
	jhs default		;jump to default
	mov &state, r12 	;moves state into r12
	add r12, r12		;add r12 into r12 
	mov jt(r12), r0		;start at first in jt

default:
	mov #1, &state			;set state to 1 to go to case1
	jmp end

case1:
	mov.b #0, &green_on		;set green led to be off
	mov.b #0, &red_on		;set red led to be off
	mov #2, &state			;set state to 2 so that it moves to case2
	jmp end

case2:
	mov.b #1, &green_on
	mov.b #0, &red_on
	mov #3, &state
	jmp end

case3:
	mov.b #0, &green_on
	mov.b #1, &red_on
	mov #4, &state
	jmp end

case4:	
	mov.b #1, &green_on
	mov.b #1, &red_on
	mov #1, &state
	jmp end



end:
	ret		


	
	/*some functions in assembly*/
	.global togger_red
toggle_red:
	xor.b #1, red_on
	mov.b #0, green_on
	jmp end

	
	.global togge_green
toggle_green:
	xor.b #1, green_on
	mov.b #0, red_on
	jmp end


	
	/*function to stop the music*/
	.global stopSound
stopSound:
	mov #0, r12
	call #buzzer_set_period
	ret

	
	/*state machine to plsy a song*/
st:				;state
	.word 0

	.text
jumptable:				;jump table
	.word default_
	.word case_1
	.word case_2
	
	.global demoSong_
demoSong_:
	cmp #12, &st 		;range check
	jmp default_		;jump to default
	mov &st, r12	 	;moves state into r12
	add r12, r12		;add r12 into r12 
	mov jumptable(r12), r0		;start at first in jt

default_:			
	mov #0, r12
	call #buzzer_set_period				

case_1:
	call #demoSong
	mov #2, &st
        jmp end_

case_2:
	
	call #stopSound
	mov #1, &st
        jmp end_
	
end_:
	ret		


	

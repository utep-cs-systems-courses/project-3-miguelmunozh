	.arch msp430g2553
	
	.data

	/*state machine to play a song*/
st:				;state
	.word 0

	.text
jumptable:				;jump table
	.word default_
	.word case_1
	.word case_2
	
	.global demoSong_
demoSong_:
	cmp #3, &st 		;range check
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


	
	/*function to stop the music*/
	.global stopSound
stopSound:
	mov #0, r12
	call #buzzer_set_period
	ret

	

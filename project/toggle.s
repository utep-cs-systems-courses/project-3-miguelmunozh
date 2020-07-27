	.arch msp430g2553
	
	
	.p2align 2,0
	/*state machine to control buttons*/

jt:
	.word default
	.word case1
	.word case2
	.word case3
	.word case4
	
	.global buttonState
buttonState:
	cmp #5, &stat 		;range check
	jhs default		;jump to default
	mov &stat, r12	 	;moves state into r12
	add r12, r12		;add r12 into r12 
	mov jt(r12), r0		;start at first in jt

case1:
	call #stopSound
	mov #1, &redrawScreen
	mov #1, &stat
	jmp end
case2:
	call #stopSound
	mov #0x0000, r12
	mov #0xffff, r13
	mov #0xf800, r14
	mov #0x001f, r15
	call #customShape
	mov #0, &redrawScreen
	mov #0, &stat
	jmp end
case3:
	mov #0xffff, r12
	mov #0x0000, r13
	mov #0x07ff, r14
	mov #0x053f, r15
	call #customShape
	call #soundEffect
	mov #0, &stat
	jmp end
case4:
	call #drawCustomString
	mov #4, &stat
	jmp end
default:
	jump end
end:
	ret



	
	
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

	

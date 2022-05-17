;
;
;
.ORIG x3000
	
;your code goes here
	
GET_CHAR
	GETC
	OUT

	LD R6, INV_SPACE ;if character in is a space, ignore it
	ADD R6, R6, R0
	BRz GET_CHAR

	LD R6, INV_EQUAL ;if character in is equal sign then the program can Halt
	ADD R6, R6, R0
	BRz DONE

	JSR EVALUATE ;if character in is neither a space or equal sign, check for operator and evaluate accordingly
	BR GET_CHAR
DONE
	LD R6, STACK_TOP
	LD R7, STACK_START
	NOT R6, R6		;invert stack_top, but don't convert to 2's complement
	ADD R6, R6, R7  ;copmare to STACK_START to check if the stack is empty
	BRnp INVALID_INPUT

	JSR POP
	ADD R5, R0, #0
	BRnzp PRINT_HEX
DONETWO
	HALT

INVALID_INPUT
	LEA R0, INVALID_STRING
	PUTS
	HALT

INV_SPACE		.FILL xFFE0 ;inverse of ' ' ASCII value
INV_EQUAL		.FILL xFFC3 ;inverse of '=' ASCII value
INVALID_STRING	.STRINGZ "Invalid expression"

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R3- value to print in hexadecimal
PRINT_HEX
    AND R4, R4, #0  ;DIGIT COUNTER
DIGITPRINTEDLOOP 
	ADD R4, R4, #-4 ; check if number of digits printed is greater than 3. if so, branch to outer
    BRzp DONETWO
    ADD R4,R4, #4  ;
    AND R0, R0, #0 ; DIGIT TO BE PRINTED
    AND R2, R2, #0 ; BIT COUNTER
FOURBITCOUNTER 	
	ADD R2, R2, #-4	;decrement 4 from r2 and branch to print digit if its not INVative
    BRzp DIGITPRINTER
    ADD R2, R2, #4   ;increment bit counter by 4
    ADD R0, R0, R0   ;shift R0 to the left
    ADD R5, R5, #0	 ;update nzp for for R5 and branch to addzero if zero or positive
    BRzp ADDZERO
    ADD R0, R0, #1	 ; add 1 to R0 if R5 is INVative , otherwise add 0
ADDZERO         
	ADD R0, R0, #0	 
    ADD R5, R5, R5	 ; Multiply R5 by 2
    ADD R2, R2, #1	 ; add 1 to R2 and then branch back to fourbitcounter loop
    BRnzp FOURBITCOUNTER

DIGITPRINTER 	
	ADD R0,R0,#-9	;decrement R0 by 9 and branch if it is nonpositive to Undernine loop
    BRnz UNDERNINE
    ADD R0,R0,#15	; increment R0 by 64
    ADD R0,R0,#15
    ADD R0,R0,#15
    ADD R0,R0,#10
    ADD R0,R0,#9
    BRnzp PRINT		; Branch to print code
UNDERNINE 		
	ADD R0,R0,#9	; Increment R0 by 67
    ADD R0,R0,#15
    ADD R0,R0,#15
    ADD R0,R0,#15
    ADD R0,R0,#3
PRINT           
	OUT				; Print whatever is in R0, increment counter, and branch to DIGITPRINTEDLOOP Loop
	ADD R4, R4, #1	;increment R4
	BRnzp DIGITPRINTEDLOOP
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;R0 - character input from keyboard
;R6 - current numerical output
;
;
EVALUATE
	ST R1, EVAL_R1Store
	ST R7, EVAL_R7Store

CHECK_NUM
	LD R1, INV_ZERO ;compare input with ASCII char '0'
	ADD R1, R1, R0	
	BRn NAN 		;if R0 is less than 0, then input is not a number
	ADD R1, R1, #-9
	BRp NAN			;if R0 is greater than 0+9 then it cannot be considered a valid number
NUM
	;this label is meant to indicate a valid numeric input, now subtract 0 to get value and push to stack
	LD R1, INV_ZERO
	ADD R0, R0, R1  ;translate ASCII number to an integer number
	JSR PUSH		;push this to the stack
	BR EVAL_RESET   ;return to get the next input value

NAN	
	;so the input is not a number eh? 
	;compare it to the operators to check if it's still a valid input, otherwise, it's Invalid

	LD R1, INV_MUL
	ADD R1, R1, R0
	BRz OPERATORS

	LD R1, INV_PLUS
	ADD R1, R1, R0
	BRz OPERATORS

	LD R1, INV_MIN
	ADD R1, R1, R0
	BRz OPERATORS

	LD R1, INV_SLASH
	ADD R1, R1, R0
	BRz OPERATORS

	LD R1, INV_EXP
	ADD R1, R1, R0
	BRz OPERATORS

	;we've yet to branch eh? probably means the input is not a number,
	;nor  an operator, thus it's invalid

	BR INVALID_INPUT ;loop back to invalid input string to halt the program and output invalid-case string
	
OPERATORS
	;so you've received an operator input eh? Each operator must pop two values,
	;so this is what happens first. Then the current operator is compared once more to list of valid operators
	;and is branched to the appropriate loop/case which then applies the operation
	;and pushes the result. Result is stored in R5 and returns to EVALUATE
	
	ADD R2, R0, #0	;save R0 in R2

	JSR POP			;popping the first value
	ADD R4, R0, #0	;store this value in R4
	JSR POP			;popping the second value
	ADD R3, R0, #0	;store this second value in R3
	ADD R5, R5, #0	;checking for underflow
	BRnp INVALID_INPUT	;if underflow, branch to invalid input string and halt

	ADD R0, R2, #0	;restore R0 from R2

	;so you've made it this far eh? now it's time to compare the current operator
	;value and apply its function

	LD R1, INV_MUL
	ADD R1, R1, R0
	BRz OP_MUL

	LD R1, INV_PLUS
	ADD R1, R1, R0
	BRz OP_ADD

	LD R1, INV_MIN
	ADD R1, R1, R0
	BRz OP_MIN

	LD R1, INV_SLASH
	ADD R1, R1, R0
	BRz OP_DIV

	LD R1, INV_EXP
	ADD R1, R1, R0
	BRz OP_EXP

OP_MUL
	JSR MUL				;R3 * R4 = R0
	BR STORE_RESULT

OP_ADD
	JSR PLUS				;R3 + R4 = R0
	BR STORE_RESULT

OP_MIN
	JSR MIN				;R3 - R4 = R0
	BR STORE_RESULT

OP_DIV
	JSR DIV				;R3 / R4 = R0
	BR STORE_RESULT

OP_EXP
	JSR EXP				;R3 ^ R4 = R0
	BR STORE_RESULT

STORE_RESULT
	JSR PUSH   ;move R0 to the stack
	ADD R5, R0, #0 ;Storing R0 in R5
EVAL_RESET
	LD R1, EVAL_R1Store
	LD R7, EVAL_R7Store
	BR GET_CHAR

EVAL_R1Store 	.BLKW #1
EVAL_R7Store 	.BLKW #1

INV_MUL 	.FILL xFFD6 ;additive inverse of '*' ASCII value
INV_PLUS    .FILL xFFD5 ;additive inverse of '+' ASCII value
INV_MIN     .FILL xFFD3 ;additive inverse of '-' ASCII value
INV_SLASH   .FILL xFFD1 ;additive inverse of '/' ASCII value
INV_ZERO    .FILL xFFD0 ;additive inverse of '0' ASCII value
INV_EXP     .FILL xFFA2 ;additive inverse of '^' ASCII value


;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
PLUS	
	ADD R0, R3, R4   ; add R3 and R4
    RET
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MIN	
    NOT R0, R4              ;negate R4 using R0 to store the result
    ADD R0, R0, #1			;converting to 2's copmlement
    ADD R0, R0, R3          ;(-R4) + R3 = R0 
    RET
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
MUL	
    AND R0,R0,#0	
	ADD R4,R4,#0
	BRzp MUL_LOOP
	NOT R4, R4
	NOT R3, R3
	ADD R4, R4, #1
	ADD R3, R3, #1
MUL_LOOP ADD R0,R3,R0
    ADD R4,R4,#-1
    BRp MUL_LOOP
	RET

	
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
DIV	
	ST R3, DIV_R3Store
    AND R0, R0, #0          ;set R0 to -1
    ADD R0, R0, #-1
    NOT R4, R4              ;negate R4
    ADD R4, R4, #1
DIV_LOOP
    ADD R0, R0, #1          ;increment subtraction count
    ADD R3, R3, R4          ;try another subtraction
    BRzp DIV_LOOP           ;if R3 is still greater than 0, subtract again

    LD R3, DIV_R3Store      ;restore R3
    NOT R4, R4              ;restore R4 by negating again
    ADD R4, R4, #1
    RET

DIV_R3Store      .BLKW #1
;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;
;input R3, R4
;out R0
EXP
;exponents eh?
;uses Subroutine for multiplication, and sets R1 as counter, 
;and multiplies repeatedly until exponent counter goes down to 0. 
;originally subtracts 1 from exponent
;to account for R3 times R3 being equal to R3^2
    ST R1,EXP_R1Store			
	AND R1,R1,#0
    AND R0,R0,#0
	ADD R0,R0,#1
	ADD R1,R4,#-1
	ADD R4,R3,#0
	ADD R1,R1,#0    
    BRz ZERO_POWER
UP
	ADD R1,R1,#0
    BRz DOWN
	ST R7,EXP_R7Store
	JSR MUL
	LD R7,EXP_R7Store
	ADD R4,R0,#0
    ADD R1,R1,#-1
    BRp UP

DOWN
    LD R1,EXP_R1Store
    ADD R0,R0,#0
    RET
   
ZERO_POWER		
    LD R1,EXP_R1Store
    ADD R0,R0,#1
    RET

EXP_R1Store      .BLKW #1
EXP_R7Store      .BLKW #1

	
;IN:R0, OUT:R5 (0-success, 1-fail/overflow)
;R3: STACK_END R4: STACK_TOP
;Pushin P
PUSH	
	ST R3, PUSH_SaveR3	;save R3
	ST R4, PUSH_SaveR4	;save R4
	AND R5, R5, #0		;
	LD R3, STACK_END	;
	LD R4, STACk_TOP	;
	ADD R3, R3, #-1		;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz OVERFLOW		;stack is full
	STR R0, R4, #0		;no overflow, store value in the stack
	ADD R4, R4, #-1		;move top of the stack
	ST R4, STACK_TOP	;store top of stack pointer
	BRnzp DONE_PUSH		;
OVERFLOW
	ADD R5, R5, #1		;
DONE_PUSH
	LD R3, PUSH_SaveR3	;
	LD R4, PUSH_SaveR4	;
	RET


PUSH_SaveR3	.BLKW #1	;
PUSH_SaveR4	.BLKW #1	;


;OUT: R0, OUT R5 (0-success, 1-fail/underflow)
;R3 STACK_START R4 STACK_TOP
;
POP	
	ST R3, POP_SaveR3	;save R3
	ST R4, POP_SaveR4	;save R3
	AND R5, R5, #0		;clear R5
	LD R3, STACK_START	;
	LD R4, STACK_TOP	;
	NOT R3, R3		;
	ADD R3, R3, #1		;
	ADD R3, R3, R4		;
	BRz UNDERFLOW		;
	ADD R4, R4, #1		;
	LDR R0, R4, #0		;
	ST R4, STACK_TOP	;
	BRnzp DONE_POP		;
UNDERFLOW
	ADD R5, R5, #1		;
DONE_POP
	LD R3, POP_SaveR3	;
	LD R4, POP_SaveR4	;
	RET

POP_SaveR3	.BLKW #1	;
POP_SaveR4	.BLKW #1	;
STACK_END	.FILL x3FF0	;
STACK_START	.FILL x4000	;
STACK_TOP	.FILL x4000	;


.END

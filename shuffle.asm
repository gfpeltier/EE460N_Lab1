	.ORIG x3000
	LEA R0, M		;LOAD ADDRESS OF MASK INTO R0
	LEA R4, S		;R4 HOLDS ADDRESS FOR START OF ORIGINAL DATA
	LEA R5, E		;R5 HOLDS ADDRESS FOR START OF WRITE TO
	LDB R0, R0, #0		;LOAD MASK INTO R0
	LEA R1, B
	AND R0, R0, R1		;CLEAR BIG END BYTE OF REGISTER
	LSHF R0, R0, #2
	AND R3, R3, #0		;R3 WILL BE COUNTER
	ADD R3, R3, #-1
LOOP	RSHFL R0, R0, #2
	ADD R3, R3, #1
	AND R1, R0, x3		;CLEAR ALL BUT BITS 1,0 OF MASK
	BRz FIRST
	ADD R1, R1, #-1
	BRz SECOND
	ADD R1, R1, #-1
	BRz THIRD
	BRnzp FOURTH
FIRST	LDB R2, R4, #0
	ADD R6, R5, R3
	STB R2, R6, #0
	BRnzp CHECK
SECOND	LDB R2, R4, #1
	ADD R6, R5, R3
	STB R2, R6, #0
	BRnzp CHECK
THIRD	LDB R2, R4, #2
	ADD R6, R5, R3
	STB R2, R6, #0
	BRnzp CHECK
FOURTH	LDB R2, R4, #3
	ADD R6, R5, R3
	STB R2, R6, #0
	BRnzp CHECK
CHECK	ADD R6, R3, #-3
	BRn LOOP
DONE	HALT
S	.FILL x4000
M	.FILL x4004
E	.FILL x4005
B	.FILL x00FF
	.END

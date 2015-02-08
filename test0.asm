	;; 
	;; 
	;;
	.ORIG x3000
	;;
	;;

	that 	AND R1, R1, R0
		AND R1, R1, #6
		ADD R0, R1, R2
		BR that 		;something
		BRn that
		BRz that
		BRP that		
		BRnz that
		BRnp that
		BRzp that
		BRnzp that
	;;
	NOP
	this	JMP R1
		RET
		JSR this
	JSRR R2
	LDB R1, R5, #20
	LDW R2, R4, #-13
	LEA R3, that
	NOT R3, R4
	RTI
	LSHF R4, R3, #2
	RSHFL R2, R3, #1
	RSHFA R6, R7, #3
	STB R3, R5, #10
	STW R5, R2, #2
	TRAP x25
	XOR R3, R1, R3
	XOR R2, R2, #-1
		LEA R2, that
		ADD R2, R6, R5 		;
		ADD R1, R4, #12
		HALT
	.END
	
	

LIMIT EQU 0x3E8 ;global limit value*4
	
		AREA My_Arrays, DATA, READWRITE
			ALIGN
primeNumbers   SPACE	LIMIT
primeNumbers_end
isPrimeNumber   SPACE	LIMIT 
isPrimeNumber_end

		AREA array_copy, code, readonly	;Declare new area
		ENTRY							;Declare as entry point
		THUMB
		ALIGN							;Ensures that _main addresses the fallowing instruction
__main	FUNCTION						;Enable debug
		EXPORT __main
		LDR R5,=primeNumbers ;Load primeNumbers array start address to R5
		LDR R6,=isPrimeNumber ;Load isPrimeNumber array start address to R6
		LDR R0,=LIMIT ; limit value is R0
		LSRS R0,R0,#2 ;decimal of the limit
		BL	SieveOf ; call function
		
stop	B 	 stop

SieveOf	MOVS R1,#0 ;R1=i=0 index value
L1		MOVS R2,#0 ; R2 is the initilization value of the arrays
		CMP  R1,R0 ;while i<limit
		BGE  L1_end ;jump to loop2
		LSLS R1,R1,#2
		STR  R2,[R5,R1] ;PrimeNumbers[i] = 0
		MOVS R2,#1     ; R2 is now 1
		STR	 R2,[R6,R1]	;isPrimeNumber[i]= true
		LSRS R1,R1,#2  ; divide R1 with 4 for decimal value
		ADDS R1,R1,#1   ;i++
		B	 L1
L1_end		
		MOVS R1,#1 ;i=1
L2		ADDS R1,R1,#1 ;i=2
		MOVS R2,R1 ;y=i=2
		MULS R2,R1,R2 ;R2 = i*i which is the test condition for the loop
		CMP  R2,R0 ;while i*i<LIMIT
		BGT  L2_end ; if condition not met
		MOVS R2,R1 ;R2 = i
		LSLS R2,R2,#2 ;R2 = i*4 is the index of the array
		LDR  R3,[R6,R2]; R3 = isPrimeNumber[i]
		CMP  R3,#1 ;if isPrimeNumber[i]==1 -> go to L3
		BNE  L2	   ;else back to L2
		MOVS R4,#0 ;R4 = 0, counter for i multiplier (0, 1*i, 2*i)
L3		MOVS R2,R1 ; R2=R1 R1 is the i value so R2=i
		MULS R2,R2,R2 ;R2=R2*R2=(i*i)  
		MOVS R3,R1 ;R3=R1 R1 is the i value so R3=i
		MULS R3,R4,R3 ;R3=R3*R4 = i*counter to get  (0),(1i),(2i),...
		ADDS R2,R3,R2 ;j = R2=R3*R2  = (i*i+0),(i*i +1i),(i*i+2*i),... which is the j value
		CMP  R2,R0   ;while j<=limit
		BGT  L2  ;else jump back to L2
		MOVS R3,#0 ;false value
		LSLS R2,R2,#2 ;multiply R2 with 4 to get index value
		STR  R3,[R6,R2] ;isPrimeNumber[j] = false
		ADDS R4,R4,#1 ;increase counter R4
		B	 L3	;go back to L3
L2_end  
		MOVS R1,#0 ;R1=index=0	
		MOVS R2,#1 ;R2=i=1
L4		ADDS R2,R2,#1 ;inc R2,R2=2
		CMP  R2,R0 ;while i<limit
		BGT  finish;else finish the function
		LSLS R4,R2,#2 ;index of the array i*4
		LDR  R3,[R6,R4] ;isPrimeNumber[i] value
		CMP  R3,#1  ;if isPrimeNumber[i]==1
		BNE  L4    ;else jump back to L4
		STR  R2,[R5,R1] ;primeNumbers[index] = i
		ADDS R1,R1,#4 ;index++, next array element
		B	 L4 ; return to L4

finish  BX   LR ; return with the link 

		ALIGN
		ENDFUNC
		END

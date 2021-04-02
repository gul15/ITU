index EQU 12	
	
		AREA My_Arrays, DATA, READWRITE
			ALIGN
array   SPACE	index
array_end

		AREA array_copy, code, readonly	;Declare new area
		ENTRY							;Declare as entry point
		THUMB
		ALIGN							;Ensures that _main addresses the fallowing instruction
__main	FUNCTION						;Enable debug
		EXPORT __main
			
		LDR R5,=array ;Load primeNumbers array start address to R5
		MOVS R3,#index ; index value is R3		
		MOVS R4,#0     ;R4 = i = 0 starting value
Loop	CMP  R4,R3	  ;while i<index 
 		BGE  stop    ; else finish program
		MOVS R0,R4   ; R0 is function argument, set it to R4(i) value
		BL	fact ; call function, result value is on R2 now
		LSLS R4,R4,#2 ; multiply R4(i) value by for to reach array elements
		STR  R2,[R5,R4] ; store the result in the array, array[i] = fact(i)
		LSRS R4,R4,#2 ; divide by 4 get decimal index value
		ADDS R4,R4,#1 ; increase i with 1, i++
		B Loop ;return to loop
		
stop	B 	 stop ;program finished, while(1)
		 
fact	PUSH  {LR} ;push return address to stack
		PUSH  {R0} ;n value as argument to stack
		CMP   R0,#2 ;if n>1 call fact again
		BGE   b_link  ;branch link part
		
		MOVS   R2,#1 ;else since n<2, result is 1, save it in R2
		B     finish ;finish part
		
b_link  SUBS  R0,R0,#1 ;since n is pushed to stack,now decrease n by 1 and call fact function again
		BL 	  fact ;used this part to branch with link

finish	POP  {R1} ;pop n-1 from stack
		CMP  R1,#0 ;if R1=0 the multiplication results with 0.Since fact(0) = 1, manually set R1 to 1.
		BEQ  set1  ;Branch to set operation
test	MULS R2,R1,R2 ;multiply n * (n-1) save it in R2
		POP  {PC} ;pop return address to PC
set1    MOVS R1,#1 ;set R1=1
		B	 test  ;return without link

		ALIGN
		ENDFUNC
		END

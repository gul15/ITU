ArraySize EQU 13 ;size of the factorial array
	
		AREA My_Array, DATA, READWRITE
		ALIGN
f_array   SPACE	ArraySize
f_array_end

		AREA array_copy, code, readonly	;Declare new area
		ENTRY							;Declare as entry point
		THUMB
		ALIGN							;Ensures that _main addresses the fallowing instruction
__main	FUNCTION						;Enable debug
		EXPORT __main
		LDR R5,=f_array ;Load array size
		MOVS R6,#ArraySize
		PUSH {R5} ;push start of the array as parameter
		PUSH {R6} ;push size of the array(n) as parameter
		B	Fact  ;call factorial function
stop	B 	 stop ;while(1) program ended
		
Fact	POP {R0} ;first parameter sent from main, n value
		POP {R1} ;start address of the array taken from stack
		MOVS R2,#1 ;temp = 1
		STR  R2,[R1,#0] ; array[0] = 1 , first element of the array is 1 
		MOVS R3,#1 ;R3=i=1 is the condition value of the loop
Loop	CMP  R3,R0 ;while i<n, loop
		BGE  stop  ;else stop
		LSLS R3,R3,#2 ;multiply R3(i) with 4 to get the index value for array save it 
		SUBS R3,R3,#4 ;get the index value of the previous value (i-1)
		LDR  R4,[R1,R3] ; save f_array[i-1] of R4
		ADDS R3,R3,#4 ;return to index value i
		LSRS R3,R3,#2 ;divide by 4 to get decimal value for multiplication
		MULS R4,R3,R4 ;multiplication i * f_array[i-1]
		LSLS R3,R3,#2 ;index value i
		STR  R4,[R1,R3] ;array[i]=i*array[i-1]
		LSRS R3,R3,#2 ;decimal condition value i
		ADDS R3,R3,#1 ; i++
		B Loop ;loop

		ALIGN
		ENDFUNC
		END

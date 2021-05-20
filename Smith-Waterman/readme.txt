Pseudo-code
	penalty ← -4       //gap penalty
	mismatch ← -2
	match ← 1
		Matrix [rows][columns]  ← 0 //initilization
		for x = 0 to rows	// matrix filling
			for y=0 to columns
				a = matrix [x-1][y] + penalty    //left of the current index
				b = matrix [x][y-1] + penalty   // upper of the current index
				if match:
					c = matrix[x-1][y-1] + match
				else:
					c=matrix[x-1][y-1] + mismatch
				matrix[x][y] = biggest(a, b, c)
		emptystring
		for x = 0 to rows	
			for y=0 to columns
				if matrix[x][y] is biggest number in matrix:
					while matrix[x][y] != 0:	// Traceback
						emptystring = char + emptystring  
						x = x - 1
						y = y - 1
					
		return result = emptystring
		
Complexity
	Since it contains a loop with lengths of rows and columns,
the complexity is O(rows*columns)


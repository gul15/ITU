/*
	NAME:	Yusuf Utku Gül
	NO:		150150006
	ASSIGNMENT 3 - SMITH-WATERMAN
	
	The program does not take any command line arguments, 
	reads “strings.txt” and then outputs the results to “output.txt”. 
*/


#include <iostream>
#include <fstream>
#include <string>		
#include <vector>		//matrix operations
#include <algorithm>	//delete duplicates, sort aphabetically

using namespace std;


int biggest(int a, int b, int c) { // returns biggest from three integers
	if (a > b && a > c) {
		return a;
	}
	else if (b > a && b > c) {
		return b;
	}
	else {
		return c;
	}
}

void printmatrix(vector<vector<int>> matrix) { //prints out the matrix
	int rows = matrix.size();
	int cols = matrix[0].size();
	for (int i = 0;i < rows;i++) {
		for (int j = 0;j < cols;j++) {
			cout << matrix[i][j] << " ";
		}
		cout << endl;
	}
}

int biggest_in_matrix(vector<vector<int>> matrix) { // returns biggest value in matrix, for printing longest sequence
	int rows = matrix.size();
	int cols = matrix[0].size();
	int max = 0;
	for (int i = 0;i < rows;i++) {
		for (int j = 0;j < cols;j++) {
			if (matrix[i][j] >= max) {
				max = matrix[i][j];
			}
		}
	}
	return max;
}

void print_results(vector<string> results) {	// prints out the result vector to console
	for (int i = 0; i < int(results.size());i++) {
		cout << '"' << results[i] << '"' << " ";
	}
}

void smith_water(string first, string second) {
	ofstream outfile;
	outfile.open("output.txt", ios_base::app); //open output.txt with appending

	int match = 1;			//variables
	int mismatch = -2;
	int penalty = -4;

	vector<vector<int>> matrix;  // the matrix

	int rows = second.size() + 1; 
	int cols = first.size() + 1;

	matrix.assign(rows, vector < int >(cols, 0));  // initilize matrix with sizes and all zero

	int a, b, c;		
	for (int i = 1;i < rows;i++) {	// matrix initlization
		for (int j = 1;j < cols;j++) {
			a = matrix[i - 1][j] + penalty;		//left side of the matrix with gap penalty
			b = matrix[i][j - 1] + penalty;		//upper side of the matrix with gap penalty

			if (first[j - 1] == second[i - 1]) {	// check if string match in this index, if so match else mismatch
				c = matrix[i - 1][j - 1] + match;
			}
			else {
				c = matrix[i - 1][j - 1] + mismatch;
			}
			if (a < 0) {		// all negative values becomes 0
				a = 0;
			}
			if (b < 0) {
				b = 0;
			}
			if (c < 0) {
				c = 0;
			}

			matrix[i][j] = biggest(a, b, c); //write the biggest from these three to current index
		}
	}

	//printmatrix(matrix);
	int max_match = biggest_in_matrix(matrix); // biggest number of the matrix, length of the maximum match

	vector<string> results;
	string result;
	int x;
	int y;

	for (int i = 0;i < rows;i++) {	// traverse the matrix
		for (int j = 0;j < cols;j++) {  
			if (matrix[i][j] == max_match) {	//if there is a match, traceback and find the string
				result = "";
				x = i;
				y = j;

				while (matrix[x][y] != 0) {
					result = first[y - 1] + result;
					x--;
					y--;
				}
				results.push_back(result);
			}
		}
	}

	outfile << first << " - " << second << endl;	// print results to the fiile
	sort(results.begin(), results.end());
	results.erase(unique(results.begin(), results.end()), results.end());
	outfile << "Score: " << max_match << " Sequence(s):";
	if (max_match != 0) {
		for (int i = 0; i < int(results.size());i++) {
			outfile << " " << '"' << results[i] << '"';
		}
	}
	outfile << endl;
	outfile.close();

}
int main()
{
	
	ofstream outfile;
	outfile.open("output.txt");	//erase content of the current output.txt
	outfile << "";
	ifstream file;	
	file.open("strings.txt");

	vector<string> words;

	string temp;	//read words from input and save them in an vector
	while (!file.eof()) {
		getline(file, temp);
		words.push_back(temp);
	}
	file.close(); 
	
	sort(words.begin(), words.end()); // sort words
	
	int j;
	for (int i = 0;i < int(words.size());i++) {	// traverse all words and call smith_waterman algo
		j = i + 1;
		while (j<int(words.size())) {
			smith_water(words[i], words[j]);
			j++;
		}
	}

	return 0;
}

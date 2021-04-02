/* 	Name: Yusuf Utku Gul
 *  Number: 150150006
 *  BLG 335E - Analysis of Algorithms I / CRN:15175	
 * 	Assignment 2 - Heapsort-Priority-Queue-Analysis*/
 
// Note : Since it uses STL when compiling i used "-std=c++11". 

#include<iostream>	
#include<fstream>
#include <math.h>
#include<vector>  // i used vector of tuples to store the data
#include<cstdlib> // to convert command line argument to int
#include<time.h> // process time calculation
#include <iomanip> // setprecision() function
#define LON 33.40819
#define LAT 39.19001
using namespace std;

struct taxi{
	float lon;
	float lat;
	float dist;
};
int parent(int i) { //parent of the given element, indexing starts with 1
    return i/2; 
} 

int left(int i) {  // left child of the given element 
    return (2 * i); 
} 

int right(int i) { // right child of the given element 
    return ((2 * i) + 1); 
} 

void printVector(vector<taxi> &x){     // Prints the longtitude latitude and distance of taxis
	cout << fixed << setprecision(6);
	for(int i = 1;i<x.size();i++){
		cout<<x[i].lon<<"\t"<<x[i].lat<<"\t"<<x[i].dist<<endl; 
	}
}

void swap(taxi* a, taxi* b){  // swap 2 taxi element
    taxi temp = *a;  
    *a = *b;  
    *b = temp;  
}  

void decraseDist(vector<taxi> &A,int i,float dist){ // distance decrease update function
	if (dist> A[i].dist){
		cout<<"New distance is bigger"<<endl;
	}
	else{
		A[i].dist = dist;  //new distance defined in the main with (distance-0.01)         				
		while(i>=1 && A[parent(i)].dist > A[i].dist){ //when distance decreased, the node gets changed with its parent while it is smaller
			swap(A[parent(i)],A[i]);
			i = parent(i);
		}
	}
}

void insert(vector<taxi> &A,taxi* new_taxi){ //insert function  is basicly takes the new element from end of the list, until it is no longer smaller than parent
	A.push_back(*new_taxi);
	decraseDist(A,A.size()-1,new_taxi->dist);
}

void writeToFile(vector<taxi> &A,string new_filename){ // this function is writes the vector taxi data to txt file for test purposes
	ofstream file;
	file.open(new_filename);
	cout << fixed << setprecision(5);
	for(int i = 1;i<A.size();i++){
		file<<A[i].lon<<" "<<A[i].lat<<" "<<A[i].dist<<endl; 
	}
	file.close();
}

void heapify(vector<taxi> &A,int i){ // heapify transfers bigger element to lower on the list, used in popMin function
	int left_child = left(i); //indexes of the childs
	int right_child = right(i);
	int min = i; //for now the minimum is the i

	if(left_child<A.size() && A[left_child].dist<A[i].dist){ //compare parent with child with boundary check
		min = left_child; //if smaller new min is left child
	}
	else{ //if not i is still min
		min = i;
	}
	if(right_child<A.size() && A[right_child].dist<A[min].dist){ //compare right child with minimum
		min = right_child;
	}
	if(i != min){ // if i not minimum swap with minimum and heapify the new minimum
		swap(A[i],A[min]);
		heapify(A,min);
	}
}

taxi popMin(vector<taxi> &A){ 
	if(A.size()<1){ //heap underflow check
		cout<<"No element in heap"<<endl;
		exit(1);
	}
	taxi min_taxi = A[1]; //in a min pq heap first element always the minimum
	swap(A[1],A[A.size()-1]); //change first element with last and pop the last(min) element
	A.pop_back();
	heapify(A,1); // heapify the first element to proper place
	
	return min_taxi;
}



int main(/*int argc,char* argv[]*/){
	
	vector<taxi> list; //heap pq list  
	
	taxi temp; // it is pushed to list for placeholding since index starts with 1
	list.push_back(temp);
	
	int m=100000; //test purposes
	float p=0.9;
	//int m = atoi(argv[1]);
	//float p = atof(argv[2]);
	
	int random_taxi; // random taxi index for update operation
	
	srand(time(NULL)); // seed random with time
	
	string line; //File operations
	ifstream file;
	file.open("locations.txt");
	getline(file, line); //this is the header line
	
	clock_t time; // time variable
	int add_count = 0;
	int upd_count = 0;
	int choice; // selects if addition or update
	time = clock();//start the clock before execution
	for(int i=1;i<=m;i++){
		//cout<<i<<" ";
		if(i%100==0){ //taxi called
			temp = popMin(list);
			cout<<"Taxi called. Longtitude:"<<temp.lon <<"   \tLatitude:"<<temp.lat<<"   \tDistance:"<<temp.dist<<endl;
		}
		else{
		choice = rand()%100; 
		if(add_count<5){ // firstly add 5 taxi to the heap
			choice = 101;
		}
			if(choice<(int)(p*100)){ //update
				if(list.size()<=1){ // list size check
					cout<<"No element on the list"<<endl;
				}
				else{
					random_taxi = rand()%(list.size()-1) + 1; // get a random taxi index
					decraseDist(list,random_taxi,(list[random_taxi].dist-0.01)); // decrase dist with 0.01
				//	cout<<"taxi "<<random_taxi<<" updated"<<endl;
					upd_count++;
				}
			}
			else{ //addition
				file>>temp.lon; //read new taxi from the txt
				file>>temp.lat;
				temp.dist = sqrt((LON-temp.lon)*(LON-temp.lon)+(LAT-temp.lat)*(LAT-temp.lat)); //calc dist
				getline(file, line, '\n'); //this is for reading the \n character into dummy variable.
				insert(list,&temp); // insert to the heap
			//	cout<<temp.lon<<"\t"<<temp.lat<<"\t"<<temp.dist<<" added."<<endl;
				add_count++;
			}
		}
	}
	time = clock() - time; // runtime = endtime - starttime
	
	cout<<"Total taxi additions:"<<add_count<<" and taxi updates:"<<upd_count<<endl;
	cout<<"Program with m="<<m<<" and p="<<p<<" finished in "<<(float)time*1000/CLOCKS_PER_SEC<<" milliseconds."<<endl;
	
	// writeToFile(list,"test.txt"); if you want to write the heap to text,faster than printing to console
		
	file.close();
	return 0; 
}

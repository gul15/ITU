/* 	Name: Yusuf Utku Gul
 *  Number: 150150006
 *  BLG 335E - Analysis of Algorithms I / CRN:15175	
 * 	Assignment 1 - Quicksort Analysis*/
 
// Note : Since it uses STL when compiling i used "-std=c++11". 

#include<iostream>	
#include<fstream>
#include<string> 
#include<vector>  // i used vector of tuples to store the data
#include<cstdlib> // to convert command line argument to int
#include<time.h> // process time calculation
#include <iomanip> // setprecision() function
using namespace std;
struct sale{
	string country;
	string item;
	string order;
	int sold_unit;
	float profit;
};
//Function prototypes
void print_vector(vector<sale> x); //prints the vector list of sales to terminal
void print_vector_tofile(vector<sale> x,string filename);//creates a txt file with selected name and prints the vector list of sales to that file
void swap(sale* a, sale* b); // swap to sales in vector list,used in partition
void quicksort_by_country(vector<sale> &x,int p,int r); //quicksort by country, takes a vector of sales, lowest index and highest index
int partition(vector<sale> &x, int p,int  r); // partition used in quicksort takes the same values and returns the new pivot value for divided lists
void quicksort_by_profit(vector<sale> &x,int p,int r); // quicksort by profit, used for the b part of the report
int partition_profit(vector<sale> &x, int p, int r);
vector<sale> read_data(ifstream& file,int N,string filename); // this function reads data from text file and returns the vector list
void write_runt_res_file(ifstream& datafile,string datafilename,string newfilename);// This function is used to calculate avarege running times of quicksort and write the results to a text file
         

int main(){  //Main start
	
	ifstream file;
	int N = 10000; //readed value from command line
	vector<sale> sales= read_data(file,N,"sorted.txt"); // read data from sales.txt and save it in sales vector.
	
		//quicksort_by_profit(sales,0,sales.size()-1);  // used for the part b of the report. First sort the sales according to profit then sort alphabeticaly
		//print_vector_tofile(sales,"sorted_by_profit.txt");
		
	clock_t time; // time variable definition
	time = clock(); // starting time of sorting
	quicksort_by_country(sales,0,sales.size()-1); // sort sales vector
	time = clock() - time; // ending time of sorting - starting time = process runtime 
	cout<<"Files printed to the file 'sorted.txt'."<<endl;
	cout<<"Sorting "<<N<<" line of sales took "<<(float)time/CLOCKS_PER_SEC<<" seconds."<<endl;
	cout<<"Now printing new list to a file"<<endl; 
	print_vector_tofile(sales,"sorted2.txt"); // print sales to file named "sorted.txt"
	cout<<"Writing finished."<<endl;
	cout<<"Exiting..."<<endl;
}   //Main end

void print_vector(vector<sale> x){     // I used vector as data structure and this prints the vector
	cout << fixed << setprecision(5);
	for(int i = 0;i<x.size();i++){
		cout<<x[i].country<<"\t"<<x[i].item<<"\t"<<x[i].order<<"\t"<<x[i].sold_unit<<"\t"<<x[i].profit<<endl; 
	}
}
void print_vector_tofile(vector<sale> x,string filename){     // I used vector as data structure and this prints the vector to file
	ofstream file;
	file.open(filename);
	if(!file){
		cerr << "File cannot be opened!";
		exit(1);
	}
	file << fixed << setprecision(5);		//to get rid of "e" values
	file<<"Country"<<"\t"<<"Item Type"<<"\t"<<"Order ID"<<"\t"<<"Units Sold"<<"\t"<<"Total Profit"<<endl; 
	for(int i = 0;i<x.size();i++){
		file<<x[i].country<<"\t"<<x[i].item<<"\t"<<x[i].order<<"\t"<<x[i].sold_unit<<" "<<x[i].profit<<endl; 
	}
}
void write_runt_res_file(ifstream& datafile,string datafilename,string newfilename){ // This function is used to calculate avarege running times of quicksort and write the results to a text file
	
	ofstream runtimefile;
	runtimefile.open(newfilename);
	if(!runtimefile){
		cerr << "File cannot be opened!";
		exit(1);
	}
	clock_t time;
	float average_time=0;
	vector<sale> list;
	for(int j=10;j<=1000000;j=j*10){
		average_time=0;
		list = read_data(datafile,j,datafilename);
		vector<sale> temp_list;
		for(int i=1;i<11;i++){
			temp_list = list;
			time = clock();
			quicksort_by_country(temp_list,0,temp_list.size()-1);
			time = clock() - time;
			cout<<"Trial:"<<i<<" - N:"<<j<<" - Time = "<<float(time)/CLOCKS_PER_SEC<<endl;
			average_time = average_time + time; 
		}
		average_time = average_time/(CLOCKS_PER_SEC*10);
		runtimefile<<"Avarage time for N="<<j<<" is :"<<average_time<<endl;
		if( j== 100000){
			list = read_data(datafile,500000,datafilename);
			for(int i=1;i<11;i++){
				temp_list = list;
				time = clock();
				quicksort_by_country(temp_list,0,temp_list.size()-1);
				time = clock() - time;
				cout<<"Trial:"<<i<<" - N:"<<500000<<" - Time = "<<float(time)/CLOCKS_PER_SEC<<endl;
				average_time = average_time + time; 
			}
				average_time = average_time/(CLOCKS_PER_SEC*10);
				runtimefile<<"Avarage time for N="<<500000<<" is :"<<average_time<<endl;
		}
	}
	runtimefile.close();
}
void swap(sale* a, sale* b){  
    sale t = *a;  
    *a = *b;  
    *b = t;  
}  
void quicksort_by_country(vector<sale> &x,int p,int r){ // Deterministic quicksort function. First value of the list taken as pivot (Sort by country names alphabeticly)
    if(p < r) {
        int q = partition(x,p,r); // returns the pivot value of next sorts
        quicksort_by_country(x, p, q-1); // recursively call for partitions
        quicksort_by_country(x, q+1, r);
    }
}
int partition(vector<sale> &x, int p, int r){
	int pivot_index = r; //last element as pivot
	int i = p-1;
	for(int j=p;j<=r-1;j++){ // initilize j (checking iterator) with the lowest index and increment until highest
		if(x[j].country.compare(x[pivot_index].country)<0){ // compare by country name
			i++;
			swap(x[i],x[j]);  
		}
		else if(x[j].country.compare(x[pivot_index].country)==0){ // if same country compare by profits
			if(x[j].profit>x[pivot_index].profit){
				i++;
				swap(x[i],x[j]);
			}
		}
	}
	swap(x[i+1],x[r]);
	return i + 1; // return new pivot index
}

void quicksort_by_profit(vector<sale> &x,int p,int r){ // Deterministic quicksort function. First value of the list taken as pivot 
    if(p < r) {
        int q = partition_profit(x,p,r); // returns the pivot value of next sorts
        quicksort_by_profit(x, p, q-1); // recursively call for partitions
        quicksort_by_profit(x, q+1, r);
    }
}
int partition_profit(vector<sale> &x, int p, int r){
	int pivot_index = r; //last element as pivot
	int i = p-1;
	for(int j=p;j<=r-1;j++){
		if(x[j].profit>x[pivot_index].profit){ // compare by profit
			i++;
			swap(x[i],x[j]); 
		}
	}
	swap(x[i+1],x[r]);
	return i + 1; // return new pivot index
}
vector<sale> read_data(ifstream& file,int N,string filename){ // this function reads data from text file and returns the vector list
	vector<sale> list;
	file.open(filename);
	
	if (!file){
		cerr << "File cannot be opened!";
		exit(1);
		}		
	//cout<<"N is "<<N<<endl;
	string line;
	getline(file, line); //this is the header line
	
	string country;		// definitions 
	string item;
	string order;
	int sold_unit;
	float profit;
	sale temp_sale;
	for(int i = 0; i<N; i++){		
		getline(file, country, '\t'); //country (string)
		getline(file, item, '\t'); //item type (string)
		getline(file, order, '\t'); //order id (string)
		file >> sold_unit; //units sold (integer)
		file >> profit; //total profit (float)
	    getline(file, line, '\n'); //this is for reading the \n character into dummy variable.
	    temp_sale.country = country;
	    temp_sale.item = item;
	    temp_sale.order = order;
	    temp_sale.sold_unit = sold_unit;
	    temp_sale.profit = profit;
	    list.push_back(temp_sale); // pushes to the list
	}
	file.close();
	return list; // returns list
}

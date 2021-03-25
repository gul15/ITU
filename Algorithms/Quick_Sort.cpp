/* 	Name: Yusuf Utku Gul
 *  BLG 335E - Analysis of Algorithms I / CRN:15175	
 * 	Quicksort Analysis*/


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
	double profit;
};

void print_vector(vector<sale> x){     // I used vector as data structure and this prints the vector
	cout << fixed << setprecision(2);
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
	file << fixed << setprecision(2);		//to get rid of "e" values
	file<<"Country"<<"\t"<<"Item Type"<<"\t"<<"Order ID"<<"\t"<<"Units Sold"<<"\t"<<"Total Profit"<<endl; 
	for(int i = 0;i<x.size();i++){
		file<<x[i].country<<"\t"<<x[i].item<<"\t"<<x[i].order<<"\t"<<x[i].sold_unit<<" "<<x[i].profit<<endl; 
	}
}

vector<sale> quicksort_by_country(vector<sale> &x){ // Deterministic quicksort function. First value of the list taken as pivot (Sort by country names alphabeticly)

	int size = x.size();  //size of the list
	
	if(size<2){ // if size < 2 returns the remaining list
		return x;
	}
	else{
		vector<sale> right; // two lists filled by comparing to the pivot value
		vector<sale> left;
		for(int i = 1;i<x.size();i++){   // i starts from 1 since first elemenet(0) is pivot
			int res = (x[i].country).compare(x[0].country); // compare two element
					
			if(res>0 ){ // if compared element > pivot, go right list
				right.push_back(x[i]);
			}
			else if(res<0){ // if smaller , go left list
				left.push_back(x[i]);
			}
			else if(res == 0){ //if two element equal, compare their profits
				
				if(x[i].profit <= x[0].profit){ //if profit bigger go right
					right.push_back(x[i]);
				}
				else{	//else go left
					left.push_back(x[i]);
				}			
			}
		}
		right = quicksort_by_country(right); // then quicksort right and left lists recursively
		left = quicksort_by_country(left);
		
		left.push_back(x[0]); // push the pivot element to right side of the left. ((left+pivot)+right)
		left.insert(left.end(),right.begin(),right.end()); // combine two lists into left
		return left; 
	}	
}
vector<sale> quicksort_by_profit(vector<sale> &x){ // Deterministic quicksort function. First value of the list taken as pivot (Sort based on profits)

	int size = x.size();  //size of the list
	
	if(size<2){ // if size < 2 returns the remaining list
		return x;
	}
	else{
		vector<sale> right; // two lists filled by comparing to the pivot value
		vector<sale> left;
		for(int i = 1;i<x.size();i++){   // i starts from 1 since first elemenet(0) is pivot
			bool res = (x[i].profit)>(x[0].profit); // compare two element
					
			if(res){ // if compared element > pivot, go right list
				right.push_back(x[i]);
			}
			else{ // if smaller , go left list
				left.push_back(x[i]);
			}
		}
		right = quicksort_by_profit(right); // then quicksort right and left lists recursively
		left = quicksort_by_profit(left);
		
		left.push_back(x[0]); // push the pivot element to right side of the left. ((left+pivot)+right)
		left.insert(left.end(),right.begin(),right.end()); // combine two lists into left
		return left; 
	}	
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
		for(int i=1;i<11;i++){
			time = clock();
			quicksort_by_country(list);
			time = clock() - time;
			cout<<"Trial:"<<i<<" - N:"<<j<<" - Time = "<<float(time)/CLOCKS_PER_SEC<<endl;
			average_time = average_time + time; 
		}
		average_time = average_time/(CLOCKS_PER_SEC*10);
		runtimefile<<"Avarage time for N="<<j<<" is :"<<average_time<<endl;
		if( j== 100000){
			list = read_data(datafile,500000,datafilename);
			for(int i=1;i<11;i++){
				time = clock();
				quicksort_by_country(list);
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
                            
int main(int argc,char* argv[]){  //Main start
	ifstream file;
	int N = atoi(argv[1]); //you should read value of N from command line
	vector<sale> sales= read_data(file,N,"sales.txt");
	clock_t time;
	time = clock();
	sales = quicksort_by_country(sales);
	time = clock() - time;
	print_vector_tofile(sales,"sorted.txt");
	cout<<"Files printed to the file 'sorted.txt'."<<endl;
	cout<<"Sorting "<<N<<" files took "<<(float)time/CLOCKS_PER_SEC<<" seconds."<<endl;

}   //Main end


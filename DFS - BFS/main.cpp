/* 	Name: Yusuf Utku Gul
 *  Number: 150150006
 *  BLG 336E - Analysis of Algorithms II / CRN:21160	
 * 	Assignment 1 - BSF-DSF Analysis*/

#include <iostream>
#include <string>// store words
#include <list> // queue bfs 
#include <map> // solution strcuture
#include <stack> // dfs
#include <vector> // discovered nodes list
#include <cmath> //pow 
#include <time.h> // runtime calculation
#include <fstream> // writing to file

using namespace std;

											//Global variables
int treesize = 0; //to measure tree size
int visitednodes = 0; // visited counter
int maxsize=0;   // maximum node in memory
bool solved = false; // when true returns to main
map<char,int> solution; // final solution

// function prototypes
void printtofile(map<char, int> sol,string filename);  //prints solution to the file
void printmap(map<char, int> sol); //print current solution table
bool allfull(map<char, int> sol);//if all values have non empty values return true
bool letternum(map<char, int> sol);// if same value more than one return false
bool issolved(string word1,string word2,string word3,map<char, int> sol);  //returns true if current solution is correct
bool ispossibletest(string word1,string word2,string word3,map<char, int> sol);

struct node{ // node structure   
	int num; // key of the node
	map<char, int> sol; // solution table
};

class Tree
{
	int size;    // max size of the tree 
	list<node> *adj_list;   // adj list for stack or queue
	string word1;    //input words
	string word2;
	string word3;
public:
	Tree(string word1,string word2,string word3,int size);        // constructor with size and words
	void connect(node &root, node &leaf);   //adds edge to adjency list
	void DFS(node a);   //traverse with dfs
	void BFS(node b); 	//traverse with bfs
};

Tree::Tree(string word1,string word2,string word3,int size)
{
	this->word1 = word1;  // initilize words, size and adjency list with constructor
	this->word2 = word2;
	this->word3 = word3;
	this->size = size;
	adj_list = new list<node>[size];
}

void Tree::connect(node &a, node &b)  // add connection between nodes to list
{
	adj_list[a.num].push_back(b);    
}

void Tree::DFS(node start_node) //traverse with DFS
{											
    vector<bool> discovered(size, false);  // save if nodes are discovered, at start all false
 
    stack<node> dfs_stack;   // stack for dfs
    dfs_stack.push(start_node); // starting node inserted 
 
    while (!dfs_stack.empty()){  // until stack is empty, meaning all nodes are visited
    	if(dfs_stack.size()>maxsize){
    		maxsize = dfs_stack.size();
		}
        start_node = dfs_stack.top(); // save the top node before popping
        dfs_stack.pop(); 

        if (!discovered[start_node.num]){ //if current node is not discovered yet
			map<char, int> sol = start_node.sol; // check the solution table of the node
			visitednodes++;
			if(allfull(sol) && ispossibletest(word1,word2,word3,sol)){         // check if all letters have values
				if(issolved(word1,word2,word3,sol)){  //check if problem solved
					cout<<"Solved"<<endl;
					solution = sol;
					solved = true;
					return;
				}
			}
			if(solved){
				break;
			}
            discovered[start_node.num] = true;   // mark the node as discovered
        }

        for (auto it = adj_list[start_node.num].begin(); it != adj_list[start_node.num].end(); ++it){  // add current nodes adjacent nodes to the stack if not discoevered yet
        	if ((!discovered[it->num]) && ispossibletest(word1,word2,word3,start_node.sol)){
                dfs_stack.push(*it);  		
			}
		}
    }  // return to the start of the while loop and continue until stack is empty
}

void Tree::BFS(node start_node) //traverse with BFS
{
	vector<bool> discovered(size, false); 

	list<node> bfs_queue; // create the queue 
	 
	discovered[start_node.num] = true;  // insert starting node
	bfs_queue.push_back(start_node);
	
	while(!bfs_queue.empty())   // while queue is not empty
	{
		start_node = bfs_queue.front();  // starting node is the front of the queue
		bfs_queue.pop_front();  // after saving pop the node
		
		map<char, int> sol = start_node.sol; // save the sol table of the node
		visitednodes++;
		if(allfull(sol) && ispossibletest(word1,word2,word3,sol)){ // if solved print out
			if(issolved(word1,word2,word3,sol)){
				cout<<"Solved"<<endl;
				solution = sol;
				solved = true;
				return;
			}
		}
		if(solved){
			break;
		}
		for (auto itr = adj_list[start_node.num].begin(); itr != adj_list[start_node.num].end(); ++itr){ // add current node adjacents to the queue
			if (!discovered[itr->num] && ispossibletest(word1,word2,word3,start_node.sol)){
				discovered[itr->num] = true;
				bfs_queue.push_back(*itr);		
			}
		}
	}
}

void createTree(Tree &mytree,map<char, int> sol,string word1,string word2,string word3){ // creates the tree recursively
	int rootnumber = treesize;
	
	node* root = new node;      //create the root node   
	root->num = rootnumber;
	root->sol = sol;
	
	char letter;   // search for unassigned letter in the solution table
	bool found = false;  
	for(auto itr=sol.begin();itr!=sol.end();itr++){
		if(itr->second == -1){
			letter = itr->first; //if found assign letter
			found = true;    
		}
	}
	
	if(!found){   // if not found all meaning all letters assigned return from function
		found = false;
		return;
	}
	
	for(int i=0;i<10;i++){			// assign numbers to the letter found from 0-9
		auto itr = sol.find(letter);
		itr->second = i;
		if(ispossibletest(word1,word2,word3,sol)){  // if solution is possible connect edge with root node
			treesize++;		// node number increased
			
			node* tmp = new node;  // node to assign
			tmp->num = treesize;
			tmp->sol = sol;
			
			mytree.connect(*root,*tmp);  // connect new node to root
			createTree(mytree,sol,word1,word2,word3); // call createTree for new node and so on				
		}
	}
}

int main(int argc, char *argv[])
{
	/* --------------------------------------------------------------- INITILIZATIONS */ 
	
string algo  = argv[1];	// command line arguments
cout<<"Algorithm:"<<algo<<endl;
string word1 = argv[2];
cout<<"Word 1:"<<word1<<endl;
string word2 = argv[3];
cout<<"Word 2:"<<word2<<endl;
string word3 = argv[4];
cout<<"Word 3:"<<word3<<endl;
string filename = argv[5];
//cout<<"Filename:"<<filename<<endl;

map<char, int> sol;        // Creating the solution table using map
for(int i=0;i<word3.size();i++){
		sol.insert(pair<char,int>(word3[i],-1));
		if(i<word1.size()){
			sol.insert(pair<char,int>(word1[i],-1));
		}
		if(i<word2.size()){
			sol.insert(pair<char,int>(word2[i],-1));
		}
}
// printmap(sol);
/* --------------------------------------------------------------- MAIN CODE  */

Tree* mytree= new Tree(word1,word2,word3,10000000); // create base tree with letters
//cout<<"Creating a tree"<<endl;
createTree(*mytree,sol,word1,word2,word3);
//cout<<"Tree created."<<endl;

node* root = new node; // root node
root->num=0;
root->sol=sol;
	
clock_t time; // time measurement
time = clock(); //start time

if(algo == "BFS"){
	mytree->BFS(*root);
}
else{
	mytree->DFS(*root);
}
time = clock() - time;

if(solved){
	cout<<"Algorithm: "<<algo<<endl;
	cout<<"Number of the visited nodes: "<<visitednodes<<endl;
	cout<<"Maximum number of nodes kept in memory: "<<maxsize<<endl;
	cout<<"Running time: "<<(float)time/CLOCKS_PER_SEC<<" seconds."<<endl;
	cout<<"Solution: "<<endl;
	printmap(solution);
	
	printtofile(solution,filename);
}
else{
	cout<<"Problem not solved."<<endl;
}

//cout<<"Finished."<<endl;

	return 0;
}

bool ispossibletest(string word1,string word2,string word3,map<char, int> sol){ // returns true if solution is still possible
	bool possible = true;
	
	if (!letternum(sol)){ // more than one letter have same value
		//cout<<"More than one same value"<<endl;
		return false;
	}
	if(sol.find(word1[0])->second == 0 || sol.find(word2[0])->second == 0 || sol.find(word3[0])->second == 0 ){ // number cannot start with a zero
		//cout<<"Number cannot start with 0"<<endl;
		return false;
	}
		int number1;
	int number2;
	int number3;
	int carry[word3.size()];
	carry[0] = 0;
	
	for(int i=0;i<word3.size();i++){
		if(i>=word1.size()){
			 number1 = 0;
		}
		else{
			 number1 = sol.find(word1[word1.size()-i-1])->second;
		}
		if(i>=word2.size()){
			number2 = 0;
		}
		else{
			number2 = sol.find(word2[word2.size()-i-1])->second;
		}

		number3 = sol.find(word3[word3.size()-i-1])->second;
		
		if(number1 == -1 || number2 == -1  || number3 == -1){
			break;
		}
		else{
			if(carry[i] + number1 + number2 == number3 ){
				carry[i+1] = 0;
				continue;
			}
			else if( carry[i] + number1 + number2 == number3+10){
				carry[i+1] = 1;
			}
			else if(carry[i] + number1 + number2 != number3){
				//cout<<number1 <<" + "<<number2<<" = "<<number3<<endl;
				//cout<<"Math problem?"<<endl;
				possible = false;
				return false;
			}
			else if(carry[i] + number1 + number2 != number3+10){
				//cout<<number1 <<" + "<<number2<<" = "<<number3<<endl;
				//cout<<"Math problem?"<<endl;
				possible = false;
				return false;
			}
		}
	}
	return true;
}












void printtofile(map<char, int> sol,string filename){ //prints solution to the file
	filename = filename + ".txt";
	ofstream myfile;
	myfile.open(filename);
	for(int i = 0;i<10;i++){
		myfile<<'\t'<<i;
	}
	myfile<<'\t'<<endl;
	for (auto itr = sol.begin(); itr != sol.end(); ++itr) {
    	myfile<< itr->first<< '\t';
    	int num = itr->second;
    	for(int i=0;i<10;i++){
    		if(num == i){
    			myfile<<"1"<<'\t';
			}
			else{
				myfile<<"."<<'\t';
			}
		}
		myfile<<endl;
	}	
	
	myfile.close();
}
void printmap(map<char, int> sol){ //print current solution table
	for (auto itr = sol.begin(); itr != sol.end(); ++itr) {
    	cout<< itr->first<< '\t' << itr->second <<endl;
	}	
}
bool allfull(map<char, int> sol){ //if all values have non empty values return true
	bool ret = true;
	for (auto itr = sol.begin(); itr != sol.end(); ++itr) {
    	if(itr->second==-1){
    		ret = false;
    		return ret;
		} 
	}
	return ret;	
}
bool letternum(map<char, int> sol){ // if same value more than one return false
	int count=0;
	for(int i=0;i<10;i++){
		for (auto itr = sol.begin(); itr != sol.end(); ++itr) {
        	if(itr->second == i){
        		count++;
			}
   		}
   		if(count>1){
   			return false;
		}
		else{
			count = 0;
			continue;
		}
	}
	return true;
}
bool issolved(string word1,string word2,string word3,map<char, int> sol){  //returns true if current solution is correct
	
int num1=0;
int num2=0;
int num3=0;
	for (auto itr = sol.begin(); itr != sol.end(); ++itr) {
    	if(itr->second==-1){
    		return false;
		}
	}
	for(int i=0;i<word1.size();i++){
		num1 = num1 + pow(10,word1.size()-1-i)*sol.find(word1[i])->second; 
	}
	for(int i=0;i<word2.size();i++){
		num2 = num2 + pow(10,word2.size()-1-i)*sol.find(word2[i])->second; 
	}
	for(int i=0;i<word3.size();i++){
		num3 = num3 + pow(10,word3.size()-1-i)*sol.find(word3[i])->second; 
	}
	if(num1+num2==num3){
		return true;
	}
	else{
		return false;
	} 

}























/* 	Name: Yusuf Utku Gul
 *  Number: 150150006
 *  BLG 336E - Analysis of Algorithms II / CRN:21160	
 * 	Assignment 2 - Kruskal MST*/

#include <iostream> //i/o
#include <vector> //edge list
#include <tuple> // edge info
#include <string> //node names
#include <bits/stdc++.h> // sort
#include <fstream> //file


using namespace std;

class Graph{
	public:
		vector <tuple<int,string,string>> edgelist; // contains all the edges
		vector <tuple<int,string,string>> solution_edges; // contains solution edges
		
		vector <string> node_list; // saves every new node to list
		void addedge(int weight,string source,string destination); // saves	edges as tuple
		void addedge_to_solution(int weight,string source,string destination);	// saves edge to solution list
		void printedges();  // prints all graph edges
		void printsolution(); //prints all soulution edges
		int solutioncost();	 //returns solution cost
		void sortedges();	// sort edges according to weight
		void sortsolution();  // sort edges according to weight
		bool iscycle();			// rteurns true if there is a 
		bool iscyclerecursive(string v,bool visited[],string parent);
};


void Graph::addedge(int weight,string source,string destination){
	edgelist.push_back(make_tuple(weight,source,destination));
}

void Graph::addedge_to_solution(int weight,string source,string destination){	//adds solution and updates node list
	solution_edges.push_back(make_tuple(weight,source,destination));
	
	bool s = true;
	bool d = true;
	for(int i=0;i!=(int)node_list.size();i++){
		if(node_list[i]==source){
			s = false;
		}
		else if(node_list[i]==destination){
			d = false;
		}
	}
	if(s){
		node_list.push_back(source);
	}
	if(d){
		node_list.push_back(destination);
	}
}

void Graph::printedges(){	// print all edges
	for(int i = 0; i<(int)edgelist.size(); i++){
		cout<<get<0>(edgelist[i])<<" : "<<get<1>(edgelist[i])<<" -> "<<get<2>(edgelist[i])<<endl;
	}
}

void Graph::printsolution(){	//print solution edges
	int w = 0;
	string s;
	string d;
	for(int i = 0; i<(int)solution_edges.size(); i++){
		w = get<0>(solution_edges[i]);
		s = get<1>(solution_edges[i]);
		d = get<2>(solution_edges[i]);
		//cout<<get<1>(solution_edges[i])<<' '<<get<2>(solution_edges[i])<<' '<<get<0>(solution_edges[i])<<'\n';
		printf("%s %s %d\n",s.c_str(),d.c_str(),w);
	}
	//cout<<solutioncost();
	printf("%d",solutioncost());
	printf("\n");
}
int Graph::solutioncost(){	// sums up edge costs
	int cost = 0;
	for(int i = 0; i<(int)solution_edges.size(); i++){
		cost = cost + get<0>(solution_edges[i]);
	}
	return cost;
}

void Graph::sortedges(){			//sort edges
	sort(edgelist.begin(),edgelist.end());
}
void Graph::sortsolution(){
	sort(solution_edges.begin(),solution_edges.end());
}

bool Graph::iscyclerecursive(string v,bool visited[],string parent){
	//cout<<"Parent: "<<parent<<" Current: "<<v<<endl;
	for(int i = 0;i<(int)node_list.size();i++){ //set current node as visited
		if(node_list[i] == v){
			visited[i] = true;
		}
	}
	
	for(int i=0;i<(int)solution_edges.size();i++){      // from solution edges
		if(v == get<1>(solution_edges[i])){ 			// find the current nodes adj noedes and call iscycle recursive for them
			string adj_node = get<2>(solution_edges[i]);  
			//cout<<"Adj_node : "<<adj_node<<endl;
			int x = 0;
			for(int j = 0;j<(int)node_list.size();j++){		//here finds the nodes order in visited list
				if(node_list[j] == adj_node){
					x = j;
				}
			}			
			if(!visited[x]){								// if adj ndoe not visited search for
				if(iscyclerecursive(adj_node,visited,v)){
					return true;
				}
			}
			else if(adj_node != parent){
				return true;
			}	
		}
		else if(v == get<2>(solution_edges[i])){		// same as the if before but source and dest reversed
			string adj_node = get<1>(solution_edges[i]);		
			//cout<<"Adj_node : "<<adj_node<<endl;
			int x = 0;
			for(int j = 0;j<(int)node_list.size();j++){
				if(node_list[j] == adj_node){
					x = j;
				}
			}			
			if(!visited[x]){
				if(iscyclerecursive(adj_node,visited,v)){
					return true;
				}
			}
			else if(adj_node != parent){
				return true;
			}	
		}
	}
	return false;
}

bool Graph::iscycle(){  // returns true if there is a cycle in graph
	int size = node_list.size(); 
	bool* visited = new bool[size];  //visited nodes list initially false
	for(int i = 0;i<size;i++){
		visited[i] = false;
	}
	
	for(int i = 0;i<size;i++){   //search for every node
		if(!visited[i]){
			if(iscyclerecursive(node_list[i],visited," ")){
				return true;
			}
		}
	}
	return false;
}


int main(){

	string fname;	// take city info file name
	cin >> fname;	
	ifstream city_plan(fname);
	string source;
	string dest;
	int weight;
	string empty;
	
	Graph kruskal_graph;
	
	while(!city_plan.eof()){// read from file and edges
		getline(city_plan,source,',');
		getline(city_plan,dest,',');
		city_plan>>weight;
		getline(city_plan,empty,'\n');
		//cout<<source<<" "<<dest<<" "<<weight<<endl;
		kruskal_graph.addedge(weight,source,dest);
	}
	city_plan.close();
	kruskal_graph.sortedges();

	for(int i= 0;i<(int)kruskal_graph.edgelist.size();i++){  //start with GP -> Ch, it is mandatory so take lowest distance Ch and add
		auto start = kruskal_graph.edgelist[i];
		string startsource = get<1>(start);
		string startdest = "";
		startdest = get<2>(start)[0];
		startdest = startdest + get<2>(start)[1];
		if(startsource == "GP" and startdest == "Ch"){		//find soruce=GP  and dest=Ch?
			kruskal_graph.edgelist.erase(kruskal_graph.edgelist.begin()+i);
			kruskal_graph.addedge_to_solution(get<0>(start),get<1>(start),get<2>(start));   //erase from the edgelist and then add to solution list
			break;					//after finding, break
		}
	}
	
	for(int i= 0;i<(int)kruskal_graph.edgelist.size();i++){  // direct connection between Hipp -> GP is a must have	
		auto start = kruskal_graph.edgelist[i];
		string startsource = get<1>(start);
		string startdest = get<2>(start);
		if((startsource == "Hipp" and startdest == "GP") or (startsource == "GP" and startdest == "Hipp")){	// after finding break
			kruskal_graph.edgelist.erase(kruskal_graph.edgelist.begin()+i);
			kruskal_graph.addedge_to_solution(get<0>(start),get<1>(start),get<2>(start));		//erase from the edgelist and then add to solution list
			break;
		}
	}

	for(int i = 0;i<(int)kruskal_graph.edgelist.size();i++){
		auto edge = kruskal_graph.edgelist[i];
		source = get<1>(edge);
		dest = get<2>(edge);
		if((source == "Hipp" and dest[0] == 'B') or (source[0] == 'B' and dest == "Hipp")){			//no conection between Bas adn Hipp
			continue;
		}
		else if(source[1] == 'p' and dest[1] == 'p'){				//no direct connection between important person houses
			continue;
		}
		else{
			kruskal_graph.addedge_to_solution(get<0>(edge),get<1>(edge),get<2>(edge));		// if no other condition add the edge to solution
			//cout<<"Put :"<<get<0>(edge)<<" "<<get<1>(edge)<<" "<<get<2>(edge)<<endl;
			if(kruskal_graph.iscycle()){		//after adding the edge if an cycle occured, pop back the edge and continue
				kruskal_graph.solution_edges.pop_back();
			}			
		}

	}
	
	kruskal_graph.sortsolution();	//sort final solution	
	kruskal_graph.printsolution();	//print solution list and cost

	return 0;
}

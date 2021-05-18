/*Zeel Mehta: 2020csm1021*/

#include<bits/stdc++.h>
using namespace std;
using namespace std::chrono;

//Global::

int Invalid = INT_MIN+1;
string dij_source, dij_destination;
int track_length=0;
unordered_map<string, vector<pair<string,long unsigned int>>> list_of_nodes;	//adjacency list containg nodename, connected_nodename and 											//distance
unordered_map<long unsigned int,long signed int> route;	

//funtion prototypes::

void display_reachable_track(long unsigned int index);		
void split(string row, char delimeter);
void display_track(unordered_map<string,long unsigned int> &distance);
void dijkastra_funtion();

void display_reachable_track(long unsigned int index)   //display_reachable_track funtion prints the track from source to destination 
{
	if (route[index]==Invalid)
		return;

	display_reachable_track(route[index]);
	cout<<"->"<<index;
	track_length++;
		
}

//split funtion::  is used to split row from file on the basis of delimeter passed

void split(string row, char delimeter)	
{
string parameter1,parameter2;
long unsigned int parameter3;

	stringstream ss (row);

	while ( getline (ss ,row, delimeter))
	{
		parameter1=row;
		getline (ss ,row, delimeter);
		
		parameter2=row;
		getline (ss ,row, delimeter);
		
		parameter3=stoul(row);
	}
	
	list_of_nodes[parameter1].push_back(make_pair(parameter2, parameter3)); //	ex: '1': '2':3  '3':4
								  //storing node name and distance from that specfic node
								  //i.e. storing data in adjacency list
}

//display_track funtion:: if node is unreachable from source then prints msg
//else calls funtion to print path from source to destination

void display_track(unordered_map<string,long unsigned int> &distance)
{
	for(auto x:distance)			//print path
        {
        		if(x.first==dij_destination)
        		{
            			if(x.second==INT_MAX)
            			{
            				cout<<"Node "<<x.first<<" is unreacheable from source node "<<dij_source<<endl;
            			}
            			else
            			{
            		cout<<"Destination node "<<x.first<<" is located at distance of "<<x.second<< " from source "<<dij_source<<endl;
            			cout<<endl;
            			cout<<"PATH::"<<endl<<endl;
             			cout<<dij_source;
             			display_reachable_track(stoul(dij_destination));
             			}    			
    			}
      	}     
}

//Dijkastra_funtion:: 

void dijkastra_funtion()
{

unordered_map<string,long unsigned int> distance; 

	for(auto j:list_of_nodes)	//intializing all nodes to INIT_MAX
	{
            distance[j.first] = INT_MAX;
        }
        
        
        vector<pair<long unsigned int, string>> dij_vector;  //distance and nodename
        
        distance[dij_source]=0;		//distance of source node from itself is 0
        
        dij_vector.push_back(make_pair(0,dij_source));	
        
        while(!dij_vector.empty())
        {
        	sort(dij_vector.begin(), dij_vector.end());	//vector is sorted in ascending order so, minimum will alwyas be at 0
        	auto z=dij_vector[0];
        	
        	long unsigned int node_dist=z.first;	//node_dist contains p.first i.e. minimum distance
        	string node=z.second;			//name of node with minimum distance
        	
        	vector<pair<long unsigned int, string>>::iterator it=dij_vector.begin();   //distance, node id
        	
        	dij_vector.erase(it);		//minimum node deleted
        	
        	
        	for(auto iterator_list_of_nodes : list_of_nodes[node])	
        	{
        		if(node_dist + iterator_list_of_nodes.second < distance[iterator_list_of_nodes.first])
        		{
        			string dest = iterator_list_of_nodes.first;
        			
        			
        			vector<pair<long unsigned int, string>>::iterator it2=dij_vector.begin();
        			it2 = find(dij_vector.begin(), dij_vector.end(), make_pair(distance[dest], dest));
        			
        			if(it2!=dij_vector.end())
        			{	
        				dij_vector.erase(it2);
        			}
        			
        			
        			long unsigned int second_node_id=stoul(iterator_list_of_nodes.first);
        			long signed int first_node_id=stoul(z.second);	
        			
        			route[second_node_id]=first_node_id;		
        			
        			distance[dest] = node_dist + iterator_list_of_nodes.second;	//new minimum distance updated in distance
        			
        			dij_vector.push_back(make_pair(distance[dest],dest));	//pair with new minimum distance and node id 
        											//pushed in vector
        		}
        	}
        } 
        
      	display_track(distance);
}

int main()
{
	bool found1=false,found2=false;
	
	string temp;
	string row;
	ifstream stream_nodes, stream_edges;
	
	cout<<"Shortest distance from source to destination by Dijkastra's algorithm"<<endl;
	stream_nodes.open("nodes.txt");
	stream_edges.open("edges.txt");
	
	if(!stream_nodes.is_open())
	{
		cout<<"Error:: nodes.txt file cannot be opened";
		exit(Invalid);
	}
	if(!stream_edges.is_open())
	{
		cout<<"Error:: edges.txt file cannot be opened";
		exit(Invalid);
	}
	
	
	cout<<"Please Enter Source node:";
	cin>>dij_source;
	
	cout<<"Please Enter Destination node:";
	cin>>dij_destination;
	
		
	while(getline(stream_nodes,row))
	{
     		temp=row;
     		
     		if(temp==dij_source)					
     			found1=true;		//if source found in file, make found1=1
     		if(temp==dij_destination)
     			found2=true;		//if detsination node found in file, make found2=1
     			
		list_of_nodes[temp];     //adding all nodes in map 
		
		route[stoul(temp)]=Invalid;   //inializing distance of all nodes as INT_MIN+1	
	}
	
	if(false==found1 && true==found2)		//checking is entered source node is present		
	{
		cout<<"Source Node not found in data"<<endl;
		exit(Invalid);
	}
	
	if(false==found2 && true==found1)		//checking is entered destination node is present
	{
		cout<<"Destination Node not found in data"<<endl;
		exit(Invalid);
	}
	
	if(false==found1 && false==found2)
	{
		cout<<"Source and Destination Nodes not found in data"<<endl;
		exit(Invalid);
	}
	
	while ( getline(stream_edges,row ))
	{
			split(row, ',');		//function split called with delimeter as : ','
	}
	
	auto start_timer=high_resolution_clock::now();	//start time
	
	dijkastra_funtion();	//calling dijkastra funtion
	
	auto stop_timer=high_resolution_clock::now();		//end time
	
	cout<<endl<<endl<<"Path Length: "<<track_length<<endl<<endl;;
	
	auto difference=duration_cast<microseconds>(stop_timer - start_timer);	
	auto difference_in_ms=difference.count()/1000;
	
	cout<<"Time taken in miliseconds:"<<difference_in_ms<<endl;
	
stream_nodes.close();
stream_edges.close();				
}







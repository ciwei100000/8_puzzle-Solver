#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <unordered_map>
#include <algorithm> //swap, max
#include <math.h> //abs

using namespace std;

const int N=8; //define the puzzle size, e.g 8 puzzles
const int BOARD_SIZE = 3; //define the board size, e.g 3x3 board 

class Puzzle // the puzzle class
{
    private:
	
	vector<int> solution(N+1); // The goal state;	

	int blank_position; // store the postion of blank

	int gn; //Path Cost

	int hn; //Heuristic Cost

	vector<int> initstate(N+1); // store the initial state

	vector<int> state(N+1); // Store the current state

	int depth; //node depth in the tree
    public:

	Puzzle(vector<int> state, int gn = 0, int hn = 0, int depth = 0){

            blank_position = 0;

	    if(depth == 0)      //store state of root node
		initstate = state;

	    this->state = state;  // set current state;

	    solution[N]=0;  // Set goal state;

	    for(size_t i=0; i<N; i++){
	     	
		solution[i] = i+1; // Set goal state;

		if(state[i]==0)
		    blank_position = i; // store initial blank position
	    }
	    this->gn = gn;
	    this->hn = hn;
	    this->depth = depth;

	}
	
	int showTotalCost(){

	    return gn+hn; //return total cost 

	}

	int showGn(){
	    return gn;
	}

	int showHn(){
	    return hn;
	}

	void setHCost(int c){  //set heuristic cost

	    this->hn = c;

	}

	vector<int> doSwap(int index1, int index2){ //return the new state after swaping 
	                                            //two elements in current state
	    vector<int> swptmp = this->state[index1];
	    swptmp[index2] = this->state[index1];
	    swptmp[index1] = this->state[index2];

	    return swptmp;
	}

	vector<int> doMove(int direction){ //Move Blank to a specific direction
	                                   //0-up; 1--down; 2-left; 3-right;

	    vector<int> movetmp;

	    if(direction == 0 && (this->blank_postion / BOARD_SIZE) != 0){
		//Blank can move upwards only if it's not at the first line	
		movetmp = this->doSwap(this->blank_position, this->blank_position-BOARD_SIZE);
	    }

	    if(direction == 1 && (this->blank_postion / BOARD_SIZE + 1) != BOARD_SIZE){
		//Blank can move downwards only if it's not at the last line	
		movetmp = this->doSwap(this->blank_position, this->blank_position-BOARD_SIZE);
	    }
	    if(direction == 2 && (this->blank_postion % BOARD_SIZE) != 0){
		//Blank can move leftwards only if it's not at the leftmost row	
		movetmp = this->doSwap(this->blank_position, this->blank_position-BOARD_SIZE);
	    }

	    if(direction == 3 && (this->blank_postion % BOARD_SIZE + 1) != BOARD_SIZE ){
		//Blank can move rightwards only if it's not at the rightmost line	
		movetmp = this->doSwap(this->blank_position, this->blank_position-BOARD_SIZE);
	    }

	    return movetmp;
	}


	vector<int> showState(){ //Return current state

	    return this->state;

	}

	int isGoal(){
	    return this->state == this->solution; //Determine if current state is goal state
	}

};

class Heuristic{

    public:                                //Solution is hard coded to
                                           //[1 2 3]
	int doMisplace(int * state){       //[4 5 6]
                                           //[7 8 0]
	    int sum=0;

	    for(size_t i=0; i<N-1; i++){
		if(state[i] != i+1)        //The num of misplaced Non-zero Numbers
		    sum++;      
	    }

	    return sum;

	}

	int doManhattan(int * state){


	    int sum = 0;
	    for(size_t i=0; i<N-1; i++){

		if(state[i] != 0){
		    sum = sum + abs(state[i]-i)/BOARD_SIZE  // Vertical Distance 
			      + abs(state[i] % BOARD_SIZE - i % BOARD_SIZE); 
		                                            // Horizonal Distance 
		}

	    }

	    return sum;
	}
};


struct cmp {   // define the operator used in the priority queue to sort
    bool operator() (Puzzle a, Puzzle b){
	return a.showTotalCost > b.showTotalCost;
    }
};

//Implement Hash for vector<int>, reference:https://stackoverflow.com/questions/29855908/c-unordered-set-of-vectors

struct VectorHash {
    size_t operator()(const std::vector<int>& v) const {
        std::hash<int> hasher;
        size_t seed = 0;
        for (int i : v) {
            seed ^= hasher(i) + 0x9e3779b9 + (seed<<6) + (seed>>2);
        }
        return seed;
    }
};

int doCleanup(priority_queue<Puzzle> * nodes){
    
    while(!nodes->empty)
	delete node->pop;

    return 1;
}


int doExpand(priority_queue<Puzzle, vector<Puzzle>, cmp> * nodes){

    size_t i,j;
    Puzzle newnode;
    unordered_set<vector<int>,VectorHash> visted; //Store visted state 

    if(nodes.empty)
	return 0;

    Puzzle node_t = nodes->pop;
    visted.insert(node_t.state);

    
    cout << "The best state to be expanded with a g(n)= " << node_t.showGn() 
    cout << "and h(n) = " << node_t.showHn() << "is" << endl;

    for(i=0; i<BOARD_SIZE; i++){
	for(j=0; j<BOARD_SIZE; j++){
	    cout << node_t[i*N+j] << " ";
	}
	cout << endl;
    }

    cout << "Expanding this node" << endl;

    if(node_t.isGoal()){
	cout << "Goal!" << endl;
	int depth = node_t.depth;

	delete node_t;

	doCleanup();

	return depth;
    }

    for(i=0; i<4; i++){ //expand it by moving blank to different directions
	                //0-up; 1-down; 2-left; 3-right
	vector<int> tmpstate = node_t.doMove();
        if(!tmpstate.empty)
	    if(!visted.count(tmpstate)){
		newnode = new Puzzle(tmpstate, node_t.showGn(), node_t.showHn(), node_t.showDepth()+1);
		if()
	    }
    }

    delete node_t;
    return 1;
}

int general_search(vector<int> inputPuzzle, int choiceNum){
    
    int count=0; //The number of nodes expanded
    int maxqueue = 0; //The maximum number of nodes in the queue
    int depth = 0; //The depth of the goal node

    node = new puzzle(inputPuzzle);  //root node in the search tree;

    if(choiceNum == 0)   // set heuristic cost for root node;
	node.setHCost(0);
    if(choiceNum == 1)
	node.setHCost(heuristic.doMisplace(inputPuzzle)); 
    if(choiceNum == 2)
	node.setHCost(heuristic.doManhattan(inputPuzzle));

    cout << "Expanding state" << endl;

    for(i=0; i<BOARD_SIZE; i++){
	for(j=0; j<BOARD_SIZE; j++){
	    cout << node[i*N+j] << " ";
	}
	cout << endl;
    }

    if(node.isGoal){ //need to modify
	cout << "Goal!" << endl;
	delete node;
	return 0;
    }

    priority_queue<Puzzle, vector<Puzzle>, cmp> nodes; // create priority queue that will pop
                                                       // the node with lowest cost
    nodes.push(node);  //push the first element

    while(!nodes.empty){

       if(!depth = expand(&nodes)){
	   cout << "The nodes shouldn't be empty here" << endl;
	   return -1;
       }

       count++;
       maxqueue = maxqueue < nodes.size? nodes.size: maxqueue;
    }
    
    cout << "To solve this problem the search algorithm expanded a total of " << count << "nodes." <<endl; 
    cout << "The maximum number of nodes in the queue at any one time was " << maxqueue <<"." <<endl;
    cout << "The depth of the goal node was "<< depth <<endl;

    return 0;
}


void main(int * , char **){

    int puzzlechoice=1;
    int numchoice = 1;
    vector<int> state(N+1);
    int input1, input2, input3;

    cout << "Welcome to Bertie Woosters 8-puzzle solver." << endl;
    cout << "Type “1” to use a default puzzle, or “2” to enter your own puzzle." << endl;

    cin >> puzzlechoice;

    if(puzzlechoice == 1){
	state = {1,2,3,4,0,6,7,5,8};
    }
    else{
	cout << "Enter your puzzle, use a zero to represent the blank" << endl;
	cout << "Enter the first row, use space or tabs between numbers" << endl;
	cin >> input1 >> input2 >> input3;
	state.push_back(input1);
	state.push_back(input2);
	state.push_back(input3);
	cout << "Enter the second row, use space or tabs between numbers" << endl;
	cin >> input1 >> input2 >> input3;
	state.push_back(input1);
	state.push_back(input2);
	state.push_back(input3);
	cout << "Enter the third row, use space or tabs between numbers" << endl;
	cin >> input1 >> input2 >> input3;
	state.push_back(input1);
	state.push_back(input2);
	state.push_back(input3);
    }


    cout << "Enter your choice of algorithm" << endl;
    cout << "1. Uniform Cost Search" << endl;
    cout << "2. A* with the Misplaced Tile heuristic" << endl;
    cout << "3. A* with the Manhattan distance heuristic" << endl;

    cin >> choicenum;

    auto starttime = chrono::steady_clock().now();

    general_search(state, choicenum); 

    auto endtime = chrono::steady_clock().now();
    auto timetook = endtime - starttime;
    cout << "Elapsed time: " << chrono::duration_cast<chrono::microseconds>(timetook).count() << " ms" << endl;
}

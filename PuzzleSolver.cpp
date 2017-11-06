#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <math.h> //abs
#include <chrono> //timer

using namespace std;


const int N=8; //define the puzzle size, e.g 8 puzzles
const int BOARD_SIZE = 3; //define the board size, e.g 3x3 board 

class Puzzle // the puzzle class
{
    private:
	
	vector<int> solution; // The goal state;	

	int blank_position; // store the postion of blank

	int gn; //Path Cost

	int hn; //Heuristic Cost

	vector<int> initstate; // store the initial state

	vector<int> state; // Store the current state

	int depth; //node depth in the tree

    public:

	Puzzle(vector<int> state, int gn = 0, int hn = 0, int depth = 0){

            this->blank_position = 0;

	    if(depth == 0)      //store state of root node
		initstate = state;

	    this->state = state;  // set current state;

	    for(int i=0; i<N+1; i++){

		if(state[i]==0){
		    this->blank_position = i; // store blank position
                    break;
                }

	    }

	    this->gn = gn;
	    this->hn = hn;
	    this->depth = depth;

	}
	
	int showTotalCost(){

	    return gn+hn; //return total cost 

	}

	int showGn(){ //return path cost
	    return gn;
	}

	int showHn(){  //return heuristic cost
	    return hn;
	}

	void setHCost(int c){  //set heuristic cost

	    this->hn = c;

	}

	vector<int> doSwap(int index1, int index2){ //return the new state after swaping 
	                                            //two elements in current state
	    vector<int> swptmp = this->state;
	    swptmp[index2] = this->state[index1];
	    swptmp[index1] = this->state[index2];

	    return swptmp;
	}

	vector<int> doMove(int direction){ //Move Blank to a specific direction
	                                   //0-up; 1--down; 2-left; 3-right;

	    vector<int> movetmp;

	    if(direction == 0 && (this->blank_position / BOARD_SIZE) != 0){
		//Blank can move upwards only if it's not at the first line	
		movetmp = this->doSwap(this->blank_position, this->blank_position-BOARD_SIZE);
	    }

	    if(direction == 1 && (this->blank_position / BOARD_SIZE + 1) != BOARD_SIZE){
		//Blank can move downwards only if it's not at the last line	
		movetmp = this->doSwap(this->blank_position, this->blank_position+BOARD_SIZE);
	    }
	    if(direction == 2 && (this->blank_position % BOARD_SIZE) != 0){
		//Blank can move leftwards only if it's not at the leftmost row	
		movetmp = this->doSwap(this->blank_position, this->blank_position-1);
	    }

	    if(direction == 3 && (this->blank_position % BOARD_SIZE + 1) != BOARD_SIZE ){
		//Blank can move rightwards only if it's not at the rightmost line	
		movetmp = this->doSwap(this->blank_position, this->blank_position+1);
	    }

	    return movetmp;
	}


	vector<int> showState(){ //Return current state

	    return this->state;

	}

        int showDepth(){ //Return depth of current state

            return this->depth;

        }

	int isGoal(){
	    for(int i=0; i<N; i++) { //Determine if current state is goal state
                if(this->state[i] != i+1)
                    return false;
            }
            return true;
	}

};


                               
                                           
int doMisplace(vector<int> state){       
                                          
        int sum=0;

	for(int i=0; i<N; i++){
	    if(state[i] != i+1)        //The num of misplaced Non-zero Numbers
		sum++;     
	}

	return sum;

}

int doManhattan(vector<int> state){


	int sum = 0;
	for(int i=0; i<N+1; i++){

	    if(state[i] != 0){
		sum = sum + abs((state[i]-i-1)/BOARD_SIZE) // Vertical Distance 
		      + abs((state[i]-1) % BOARD_SIZE - i % BOARD_SIZE); 
		                                        // Horizonal Distance 
	    }
            

	}

	return sum;
}

struct cmp {   // define the operator used in the priority queue to sort
    bool operator() (Puzzle *a, Puzzle *b){
	return a->showTotalCost() > b->showTotalCost();
    }
};

int doCleanup(priority_queue<Puzzle*, vector<Puzzle*>, cmp> * nodes){ // Garbage Collection

    Puzzle *deltmp;
    
    while(!nodes->empty()){
	deltmp = nodes->top();
	delete deltmp;
	nodes->pop();
    }

    return 1;
}


int doExpand(priority_queue<Puzzle*, vector<Puzzle*>, cmp> * nodes, map<vector<int>,bool> *visted, int choicenum){    //Expand the nodes

    int i,j;
    int hn = 0;
    Puzzle *newnode;

    if(nodes->empty())
	return -1;

    Puzzle *node_t = nodes->top();
    nodes->pop();
    
    cout << "The best state to be expanded with a g(n)= " << node_t->showGn(); 
    cout << " and h(n) = " << node_t->showHn() << " is" << endl;

    vector<int> statetmp = node_t->showState(); 

    for(i=0; i<BOARD_SIZE; i++){
	for(j=0; j<BOARD_SIZE; j++){
	    cout << statetmp[i*BOARD_SIZE+j] << " ";
	}
	cout << endl;
    }

    cout << "Expanding this node" << endl;

    if(node_t->isGoal()){
	cout << "Goal!" << endl;
	int depth = node_t->showDepth();

	delete node_t;

	doCleanup(nodes);

	return depth;
    }

    vector<int> tmpstate;

    for(i=0; i<4; i++){ //expand it by moving blank to different directions
	                //0-up; 1-down; 2-left; 3-right
        
	tmpstate = node_t->doMove(i);
        if(!tmpstate.empty() && !(*visted)[tmpstate]){
            
            if(choicenum == 1)
                hn = 0;
            if(choicenum == 2)
                hn = doMisplace(tmpstate);
            if(choicenum == 3)
                hn = doManhattan(tmpstate);
	    newnode = new Puzzle(tmpstate, node_t->showGn()+1, hn, node_t->showDepth()+1);
            (*visted)[tmpstate] = true;
            nodes->push(newnode);		
	}
    }

    delete node_t;
    return 0;
}

int general_search(vector<int> inputPuzzle, int choicenum){
    
    int count=0; //The number of nodes expanded
    int maxqueue = 0; //The maximum number of nodes in the queue
    int depth = 0; //The depth of the goal node

    map<vector<int>,bool> visted; //Store visted state 

    Puzzle *node = new Puzzle(inputPuzzle);  //root node in the search tree;

    if(choicenum == 1)   // set heuristic cost for root node;
	node->setHCost(0);
    if(choicenum == 2)
	node->setHCost(doMisplace(inputPuzzle)); 
    if(choicenum == 3)
	node->setHCost(doManhattan(inputPuzzle));

    cout << "Expanding state" << endl; 

    for(int i=0; i<BOARD_SIZE; i++){
	for(int j=0; j<BOARD_SIZE; j++){
	    cout << inputPuzzle[i*BOARD_SIZE+j] << " ";
	}
	cout << endl;
    }

    if(node->isGoal()){ //need to modify
	cout << "Root node is Goal!" << endl;
	delete node;
	return 0;
    }

    priority_queue<Puzzle*, vector<Puzzle*>, cmp> nodes; // create priority queue that will pop
                                                       // the node with lowest cost
    nodes.push(node);  //push the first element

    visted[inputPuzzle] = true;

    while(!nodes.empty()){

       depth = doExpand(&nodes,&visted,choicenum);
       count++;
       maxqueue = maxqueue < nodes.size()? nodes.size(): maxqueue;

    }

    if(depth <= 0){
       cout << "No solution!!" << endl;
       return -1;
    }
    
    cout << "To solve this problem the search algorithm expanded a total of " << count<< " nodes." <<endl; 
    cout << "The maximum number of nodes in the queue at any one time was " << maxqueue <<"." <<endl;
    cout << "The depth of the goal node was "<< depth <<endl;

    return 0;
}


int main(int argc , char **argv){

    int puzzlechoice=1;
    int choicenum = 1;

    int definput[]={1,2,3,4,0,6,7,5,8};
    vector<int> state(definput,definput+9);
    int input1, input2, input3;

    cout << "Welcome to Bertie Woosters 8-puzzle solver." << endl;
    cout << "Type “1” to use a default puzzle, or “2” to enter your own puzzle." << endl;

    cin >> puzzlechoice;

    if(puzzlechoice != 1){

        state.clear();

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
    cout << "Elapsed time: " << chrono::duration_cast<chrono::microseconds>(timetook).count() << " us" << endl;
}

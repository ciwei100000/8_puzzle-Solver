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
	
	int solution[N+1]; // The goal state;	

	int blank_position; // store the postion of blank

	int gn; //Path Cost

	int hn; //Heuristic Cost

	int state[N+1]; // Store the current state

	vector<string> paths; //store the path to the state from initial state

    public:

	Puzzle(int * s){

            blank_position = 0;

	    solution[N]=0;  // Set goal state;

	    for(int i=0; i<N; i++){
	     	
		solution[i] = i+1; // Set goal state;

		if(state[i]==0)
		    blank_position = i; // store initial blank position
	    }
	    state = s;  // set initial state;
	    gn = 0;
	    hn = 0;

	}
	
	int showTotalCost(){

	    return gn+hn; //return total cost 

	}

	void setHCost(int c){  //set heuristic cost

	    this->hn = c;

	}

	int *  doSwap(int index1, int index2){ //swap two elements in current state
	    int tmp = this->state[index1];
	    this->state[index1] = this->state[index2];
	    this->state[index2] = tmp;
	    this->hn++;

	    return this->state;
	}

	int * doMove(int direction){ //Move Blank to a specific direction
	                             //0-up; 1--down; 2-left; 3-right;
	    if(direction == 0 && (this->blank_postion / BOARD_SIZE) != 0){
		//Blank can move upwards only if it's not at the first line	
		path.push_back("Up");
		return this->doSwap(this->blank_position, this->blank_position-BOARD_SIZE);
	    }

	    if(direction == 1 && (this->blank_postion / BOARD_SIZE + 1) != BOARD_SIZE){
		//Blank can move downwards only if it's not at the last line	
		path.push_back("Down");
		return this->doSwap(this->blank_position, this->blank_position-BOARD_SIZE);
	    }
	    if(direction == 2 && (this->blank_postion % BOARD_SIZE) != 0){
		//Blank can move leftwards only if it's not at the leftmost row	
		path.push_back("Left");
		return this->doSwap(this->blank_position, this->blank_position-BOARD_SIZE);
	    }

	    if(direction == 3 && (this->blank_postion % BOARD_SIZE + 1) != BOARD_SIZE ){
		//Blank can move rightwards only if it's not at the rightmost line	
		path.push_back("Right");
		return this->doSwap(this->blank_position, this->blank_position-BOARD_SIZE);
	    }
	}


	int * showState(){ //Return current state

	    return this->state;

	}

	int isGoal(){
	    return this->state == this->solution; //Determin if current state is goal state
	}

}

class Heuristic{

    public:                                //Solution is hard coded to
                                           //[1 2 3]
	int doMisplace(int * state){       //[4 5 6]
                                           //[7 8 0]
	    int sum=0;

	    for(int i=0; i<N-1; i++){
		if(state[i] != i+1)        //The num of misplaced Non-zero Numbers
		    sum++;      
	    }

	    return sum;

	}

	int doManhattan(int * state){


	    int sum = 0;
	    for(int i=0; i<N-1; i++){

		if(state[i] != 0){
		    sum = sum + abs(state[i]-i)/BOARD_SIZE  // Vertical Distance 
			      + abs(state[i] % BOARD_SIZE - i % BOARD_SIZE); 
		                                            // Horizonal Distance 
		}

	    }

	    return sum;
	}
}


struct cmp {   // define the operator used in the priority queue to sort
    bool operator() (Puzzle a, Puzzle b){
	return a.showTotalCost > b.showTotalCost;
    }
}


int expand(priority_queue<Puzzle, vector<Puzzle>, cmp> * nodes){

    if(nodes.empty)
	return -1;

    Puzzle node_t = node->pop;


    
    cout << "Expanding state" << endl;

    for(int i=0; i<BOARD_SIZE; i++){
	for(int j=0; j<BOARD_SIZE; j++){
	    cout << node_t[i*N+j] << " ";
	}
	cout << endl;
    }

}

void general_search(int * inputPuzzle, int choiceNum){

    node = new puzzle(inputPuzzle);  //root node in the search tree;

    if(node.isGoal); //need to modify

    if(choiceNum == 0)   // set heuristic cost for root node;
	node.setHCost(0);
    if(choiceNum == 1)
	node.setHCost(heuristic.doMisplace(inputPuzzle)); 
    if(choiceNum == 2)
	node.setHCost(heuristic.doManhattan(inputPuzzle));

    priority_queue<Puzzle, vector<Puzzle>, cmp> nodes; // create priority queue that will pop
                                                       // the node with lowest cost
    nodes.push(node);  //push the first element

    expand(nodes);




}


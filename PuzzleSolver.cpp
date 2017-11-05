#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <algorithm> //swap, max
#include <math.h> //abs
#include <tuple> //tuple

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

	vector<int> paths; //store the path to the state from initial state

    public:

	Puzzle(int * s){

            blank_position = 0;

	    solution[N]=0;  // Set goal state;

	    for(i=0; i<N; i++){
	     	
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

	    this.hn = c;

	}

	int *  doSwap(int index1, int index2){ //swap two elements in current state
	    int tmp = this.state[index1];
	    this.state[index1] = this.state[index2];
	    this.state[index2] = tmp;
	    this.hn++;

	    return state;
	}

	int * showState(){ //Return current state

	    return this.state;

	}

}


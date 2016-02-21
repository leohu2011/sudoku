#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <list>
#include <sstream>
#include <cstdio>
#include <map>
#include <cstdlib>
#include <cassert>
#include <string>
#include <cmath>

using namespace std;

//keeps the current state of the board
int board[9][9];

char* filename;

//keeps count of the number of steps
int counter = 0;


//these three vectors keep tracks of what is already filled in each horizontal/vertical/grid
vector< vector<bool> > horizontal;
vector< vector<bool> > vertical;
vector <vector<bool> > grid;

//contains 1 to 9
vector<int> standard;


//read in the initial state
void read_board(int (&board)[9][9]){
   int value;
   ifstream infile;
   //setting the name of the file
   infile.open( filename );
   
   int i = 0;
   int j = 0;
   
   while (!infile.eof()){
      infile >> value;
      //if(value != 0) counter++;
      board[i][j] = value;
      j++;
      
      if (j == 9) {
         j = 0;
         i ++;
      }
   }
   
   vector<bool> temp;
   for (int i = 0; i < 9 ; i++){
      temp.push_back(false);
      standard.push_back(i+ 1);
   }
   
   /*
   for (int i = 0; i < 9 ; i++){
      cout << standard[i] << " ";
   }
   cout << endl;
   */
   
   //construct initial horizontal arrays
   for (int k = 0; k < 9; k++){
      vector<bool> temp1 = temp;
      
      for (int j =0; j < 9; j++){
         if (board[k][j] != 0){
            int value = board[k][j];
            
            temp1[value - 1] = true;
         }
      }     
      horizontal.push_back(temp1);
   }
   
   //construct initial vertical arrays
   for (int k = 0; k < 9; k++){
      vector<bool> temp2 = temp;
      for (int j =0; j < 9; j++){
         if (board[j][k] != 0){
            int value = board[j][k];
            temp2[value -1] = true;
         }
      }
      vertical.push_back(temp2);
   }
   
   
   //construct initial grid arrays, note the grid are scheduled first go across horizontally then go down
   int k1 = 0;
   int k2 = 0;
   while (k1 <= 6){
      while (k2 <= 6){
         vector<bool> temp3 = temp;
         for (int j = k1; j < k1+3; j++){
            for (int g = k2; g < k2+3; g++){
               if (board[j][g] != 0){
                  int value = board[j][g];
                  //cout << "found value: " << value << endl;
                  //cout << "position on the board is: " << j << " " << g << endl;
                  temp3[value - 1] = true;
               }
            }
         }
         grid.push_back(temp3);
         k2 += 3;
      }
      k2 = 0;
      k1 += 3;
   }
            
}


//remove an element from the vector
void vec_remove(vector<int> &vec, int num){
   for (int i =0 ; i< vec.size(); i++){
      if(vec[i] == num){
         //cout << "value found!!!!!!!!!!!" << endl;
         vec.erase(vec.begin() + i);
      }
   }
   //cout << "size of vector is now: " << vec.size() << endl;
}

//print the board
void print_board(int (&board)[9][9]) {
   for (int i = 0; i < 9; i++){
      for (int j = 0; j < 9; j++){
         cout << board[i][j] << " | ";
      }
      if ( (i == 2) || (i == 5) ){
         cout << endl;
         cout << "----------*-----------*------------" << endl;
      }
      else cout << endl;
   }
}


//auxilliary function to print the horizontal/vertical/gird arrays
void print_table() {
   for (int i = 0; i < 9; i++){
      for (int j = 0; j < 9; j++){
         cout << horizontal[i][j] << " | ";
      }
      if ( (i == 2) || (i == 5) ){
         cout << endl;
         cout << "----------*-----------*------------" << endl;
      }
      else cout << endl;
   }
}

//key function to find the most contrained position in the current board and output the avaliable values for that position
vector<int> most_constrained (pair<int, int> &temp, int (&board)[9][9]){
   int max_x =0;
   int max_y = 0;
   vector<int> least_remaining = standard;
   for (int i = 0; i<9 ; i++){
      for (int j = 0; j < 9; j++){
         if (board[i][j] == 0){
            vector<int> remaining = standard;
            //cout << "initial size is: " << remaining.size() << endl;
            
            
            //search through horizontal/vertical/gird that this position belongs to, and find out what valid values are left
            for(int k =0; k< 9; k++){
               
               if(horizontal[i][k] == true) {
                  //cout << "removed1 " << k+1 <<endl;
                  vec_remove(remaining, k+1);
                  //cout << "size of remaining updated: "<< remaining.size() << endl;
               }
            }
            
            for(int k =0; k< 9; k++){
               
               if(vertical[j][k] == true) {
                  //cout << "removed2 " << k+1 <<endl;
                  vec_remove(remaining, k+1);
                  //cout << "size of remaining updated: "<< remaining.size() << endl;
               }
            }
            
            int h = 0;
            if(i>=0 && i <3 && j>=0 && j<3) h = 0;
            else if(i>=0 && i <3 && j>=3 && j<6) h = 1;
            else if(i>=0 && i <3 && j>=6 && j<9) h = 2;
            else if(i>=3 && i <6 && j>=0 && j<3) h = 3;
            else if(i>=3 && i <6 && j>=3 && j<6) h = 4;
            else if(i>=3 && i <6 && j>=6 && j<9) h = 5;
            else if(i>=6 && i <9 && j>=0 && j<3) h = 6;
            else if(i>=6 && i <9 && j>=3 && j<6) h = 7;
            else if(i>=6 && i <9 && j>=6 && j<9) h = 8;
            
            for(int k =0; k< 9; k++){
               
               if(grid[h][k] == true) {
                  
                  //cout << "removed3 " << k+1 <<endl;
                  //cout << "          type is: " << h << endl;
                  
                  vec_remove(remaining, k+1);
                  //cout << "size of remaining updated: "<< remaining.size() << endl;
               }
            }
            
            int restriction = remaining.size();
            //cout << "remaining possibility at: " << i << " " << j << " is: " << restriction << endl;
            if ( restriction < least_remaining.size() ){
               max_x = i;
               max_y = j;
               least_remaining = remaining;
            }
         }
      }
   }
   
   temp = make_pair(max_x, max_y);
   //cout << temp.first << " " << temp.second << endl;
   //cout << least_remaining.size() << endl;
   //assert(least_remaining.size() > 0);
   return least_remaining;
}

//main recursion function
bool solve(int (&board)[9][9]){
   //print_board(board);
   
   //find the next best position to fill in using most_contrained
   pair<int, int>next;
   
   vector<int>possibility;
   possibility = most_constrained(next, board);
   
   int x = next.first;
   int y = next.second;
   
   //cout << "most restricted position is: " << x << " " << y << " with just: " << possibility.size() << " possibility left" << endl;
   
   
   
   //assert(possibility.size() > 0);
   
   
   
   /*
   for (int i =0; i < possibility.size(); i++){
      cout << "possibilities are: " << possibility[i] << " ";
   }
   cout << endl;
   */
   

   
   if (board[x][y] == 0){
      for (int i = 0; i < possibility.size(); i++){
         
         //put/update the value
         int value = possibility[i];
         board[x][y] = value;
         counter++;
         
         //update horizontal/vertical/grid
         horizontal[x][value-1] = true;
         vertical[y][value-1] = true;
         
         int h = 0;
         if(x>=0 && x <3 && y>=0 && y<3) h = 0;
         else if(x>=0 && x <3 && y>=3 && y<6) h = 1;
         else if(x>=0 && x <3 && y>=6 && y<9) h = 2;
         else if(x>=3 && x <6 && y>=0 && y<3) h = 3;
         else if(x>=3 && x <6 && y>=3 && y<6) h = 4;
         else if(x>=3 && x <6 && y>=6 && y<9) h = 5;
         else if(x>=6 && x <9 && y>=0 && y<3) h = 6;
         else if(x>=6 && x <9 && y>=3 && y<6) h = 7;
         else if(x>=6 && x <9 && y>=6 && y<9) h = 8;
         
         grid[h][value-1] = true;
         
         /*
         cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
         //print_table();
         cout << "horizontal: " << horizontal[1][7] << endl;
         cout << "vertical: " << vertical[6][7] << endl;
         cout << "grid: " << grid[2][7] << endl;
         cout << "H value is: " << h << endl;
         print_board(board);
         cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << endl;
         
         
      cout << "value: " << value << " is tried at position: " << x << " " << y << endl;
      */
      
         //recurse, if solve return true then it means that the solution is found so return true
         if ( solve(board) ){
            return true;
         }

            //cout << "back-tracking is in effect on position: " << x << " " << y << endl;
            
            //if all remaining values at this position is exhausted, then set position back to zero and backtrack
            board[x][y] = 0;
            horizontal[x][value - 1] = false;
            vertical[y][value - 1] = false;
            grid[h][value -1] = false;
      }
   //if loop exited means after all possible backtracking still no solution, so return false
   return false;
   
   }
   

   //if all values are filled then return true
   return true;
}
   
   

int main(int argc, char** argv){

	if(argc > 1){
		filename = argv[1];
	}
	
	//int board[9][9];
	
   read_board(board);
   
   print_board(board);
   cout << endl;
   bool result;
   
   result = solve(board);
   
   //cout<< "this sudoku is: " << result << endl;
   cout << "number of iterations is: " << counter << endl;
   
   print_board(board);
   cout << endl;
   //pair<int, int>next;
   //vector<int>possibility;
   //possibility = most_constrained(next, board);
   //cout << "most restricted position is: " << next.first << " " << next.second << " with just: " << possibility.size() << " possibility left" << endl;
   
   //cout <<"$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$$" << endl;
   //print_table();
   
   //cout << "horizontal value is: " << horizontal[8][1] << endl;
   //cout << "vertical value is: " << vertical[1][2] << endl;
   
   //cout << "value I am looking for is: " << board[8][7] << endl;
   //cout << "number of non zero values is: " << counter << endl;

   return 1;
}

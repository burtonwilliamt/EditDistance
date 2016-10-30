#include <iostream>
#include <Vector>
#include <String>
#include <ncurses.h>

using namespace std;


//allows printing of the fill carachter from (x,y) to (x,y+hight-1)
void print_col(int x, int y, char fill, int hight){
    for(int i = 0; i < hight; ++i){
        mvaddch(y+i, x, fill);//moving to y+i line, x column and add fill
    }
}

//allows printing of the fill carachter from (x,y) to (x+hight-1,y)
void print_row(int x, int y, char fill, int hight){
    for(int i = 0; i < hight; ++i){
        mvaddch(y, x+i, fill);//moving to y+i line, x column and add fill
    }
}



//draw the whole board
void draw(const vector< vector <int> >& board, const string& input, const string& compare, int curs_x, int curs_y){
    //print the input string across the top
    for(int i = 0; i < input.size(); ++i){
      mvaddch(0 ,6+(3*i), input.at(i));
    }
    //print the compare string along the left
    for(int i = 0; i < compare.size(); ++i){
      mvaddch(4+(2*i), 0, compare.at(i));
    }
    //create the horizontal row lines
    for(int i = 0; i < compare.size()+2; ++i){
      print_row(2, 1+(2*i), '-', board.size()*3);
    }

    //create the vertical column lines
    for(int i = 0; i < input.size()+2; ++i){
      print_col(2+(3*i), 1, '|', board[0].size()*2+1);
    }
    //print the cell values
    for(int x = 0; x < board.size(); ++x){
        for(int y = 0; y < board[0].size(); ++y){
          //if(board[x][y] >= 0){
            char buffer[3];
            snprintf (buffer, 3, "%02d", board[x][y]);
            // string temp = to_string(board[x][y]);
            string temp = buffer;
            mvprintw(2+(2*y), 3+(3*x), buffer);
          //}
        }
    }
    //highlight the selected cell
    mvprintw(1+curs_y*2, 2+(3*curs_x), "*");
    mvprintw(3+curs_y*2, 2+(3*curs_x), "*");
    mvprintw(1+curs_y*2, 5+(3*curs_x), "*");
    mvprintw(3+curs_y*2, 5+(3*curs_x), "*");
    refresh();

}

void solve(vector<vector<int> >& board, const string& input, const string& compare){
  for(int x = 0; x < board.size(); ++x){
    for(int y = 0; y < board[0].size(); ++y){
      if(x == 0){
        board[x][y] = y;
      }else if(y == 0){
        board[x][y] = x;
      }else{
        if(input.at(x-1) == compare.at(y-1)){
          board[x][y] = min( min(board[x-1][y], board[x][y-1])+1,  board[x-1][y-1]);
        }else{
          board[x][y] = min( min(board[x-1][y], board[x][y-1])+1,  board[x-1][y-1]+1);
        }
      }
    }
  }
}


//reset the board for a new game
void reset(vector<vector< int > >& board, const string& input, const string& compare, int curs_x, int curs_y){
  //clear the ncurses window
  clear();
  //empty all positions
  for(int x = 0; x < board.size(); ++x){
      for(int y = 0; y < board[0].size(); ++y){
          board[x][y] = -1;
      }
  }

  board = vector<vector<int> > (input.size()+1, vector<int> (compare.size()+1, -1));

  draw(board, input, compare, curs_x, curs_y);
}

int main(int argc, char* argv[]){

  //initialize the ncurses screen
  initscr();

  //the character that we will read in from the user
  int c;

  //where the user is "Pointing"
  //this will be represented by a '*' character in the corners of the cell
  int curs_x = 0;
  int curs_y = 0;

  string input = "sunny";
  string compare = "snowy";

  vector<vector<int> > board (input.size()+1, vector<int> (compare.size()+1, -1));

  //clear the board, and draw it.
  reset(board, input, compare, curs_x, curs_y);

  //ncurses initialization
  //we will just be using the stdscr window(default)
  keypad(stdscr, true);//allows using the arrow keys
  curs_set(0);//remove the cursor from the screen
  noecho();//stop user keypresses from appearing on the screen

  solve(board, input, compare);

  //draw the board
  draw(board, input, compare, curs_x, curs_y);

  //main game loop, run once for each key press
  //if the escape keep (27) is pressed, escape
  while((c=getch()) !=27){
    //refresh the screen
    refresh();

    //Different arrow keys
    if(c == 259 && curs_y > 0){--curs_y;}
    else if(c == 258 && curs_y < board[0].size()-1){++curs_y;}
    else if(c == 260 && curs_x > 0){--curs_x;}
    else if(c == 261 && curs_x < board.size()-1){++curs_x;}
    //if spacebar, reset the board
    else if(c == 32){
      reset(board, input, compare, curs_x, curs_y);
    }
    //draw what's changed
    draw(board, input, compare, curs_x, curs_y);
  }

  //close the ncurses window
  endwin();
  return 0;
}

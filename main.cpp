#include <iostream>
#include <Vector>
#include <String>
#include <ncurses.h>

using namespace std;


struct board{
  vector<vector<int> > vec;
  string input;
  string compare;
  int curs_x;
  int curs_y;
};



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


void pushAlign(const string& full, string& align, int pos){
  if(pos == -1){
    align = "_" + align;
  }else{
    align = full.at(pos) + align;
  }
}


void alignment(const board& b, string& iAlign, string& cAlign){
  iAlign = "";
  cAlign = "";
  int x = b.curs_x;
  int y = b.curs_y;
  while(x != 0 || y != 0){

    //the the two things are the same, then don't add 1 when going diagonally
    int same = 0;
    if(x > 0 && y > 0 && b.input.at(x-1) != b.compare.at(y-1)){
      same = 1;
    }
    //can go diagonally
    if(x > 0 && y > 0 && b.vec[x-1][y-1]+same == b.vec[x][y]){
      pushAlign(b.input, iAlign, x-1);
      pushAlign(b.compare, cAlign, y-1);
      --x;
      --y;
    }
    //can add a space to iAlign
    else if(y > 0 && b.vec[x][y-1]+1 == b.vec[x][y]){
      pushAlign(b.input, iAlign, -1);
      pushAlign(b.compare, cAlign, y-1);
      --y;
    }
    //can add a space to cAlign
    else if(x > 0 && b.vec[x-1][y]+1 == b.vec[x][y]){
      pushAlign(b.input, iAlign, x-1);
      pushAlign(b.compare, cAlign, -1);
      --x;
    }
  }
}


//draw the whole board
void drawBoard(const board& b, int pos_x, int pos_y){
    //print the input string across the top
    for(int i = 0; i < b.input.size(); ++i){
      mvaddch(pos_y ,pos_x+6+(3*i), b.input.at(i));
    }
    //print the compare string along the left
    for(int i = 0; i < b.compare.size(); ++i){
      mvaddch(pos_y+4+(2*i), pos_x, b.compare.at(i));
    }
    //create the horizontal row lines
    for(int i = 0; i < b.compare.size()+2; ++i){
      print_row(pos_x+2, pos_y+1+(2*i), '-', b.vec.size()*3);
    }

    //create the vertical column lines
    for(int i = 0; i < b.input.size()+2; ++i){
      print_col(pos_x+2+(3*i), pos_y+1, '|', b.vec[0].size()*2+1);
    }
    //print the cell values
    for(int x = 0; x < b.vec.size(); ++x){
        for(int y = 0; y < b.vec[0].size(); ++y){
          //if(b.vec[x][y] >= 0){
            char buffer[3];
            snprintf (buffer, 3, "%02d", b.vec[x][y]);
            // string temp = to_string(b.vec[x][y]);
            string temp = buffer;
            mvprintw(pos_y+2+(2*y), pos_x+3+(3*x), buffer);
          //}
        }
    }
    //highlight the selected cell
    mvprintw(pos_y+1+b.curs_y*2, pos_x+2+(3*b.curs_x), "*");
    mvprintw(pos_y+3+b.curs_y*2, pos_x+2+(3*b.curs_x), "*");
    mvprintw(pos_y+1+b.curs_y*2, pos_x+5+(3*b.curs_x), "*");
    mvprintw(pos_y+3+b.curs_y*2, pos_x+5+(3*b.curs_x), "*");
    refresh();

}

void draw(const board& b){
  drawBoard(b, 0,0);
  //the board uses b.vec[0].size()*2+2 lines
  int board_end = b.vec[0].size()*2+2;
  string iAlign = "";
  string cAlign = "";
  alignment(b,iAlign, cAlign);
  //clear the readout lines
  string temp(2*max(b.vec.size(), b.vec[0].size()), ' ');
  mvprintw(board_end+2, 0, temp.c_str());
  mvprintw(board_end+3, 0, temp.c_str());

  //populate the readouts
  mvprintw(board_end+1, 0, "Number of edits shown by value at position on board");
  mvprintw(board_end+2, 0, iAlign.c_str());
  mvprintw(board_end+3, 0, cAlign.c_str());

}

void solve(board& b){
  for(int x = 0; x < b.vec.size(); ++x){
    for(int y = 0; y < b.vec[0].size(); ++y){
      if(x == 0){
        b.vec[x][y] = y;
      }else if(y == 0){
        b.vec[x][y] = x;
      }else{
        if(b.input.at(x-1) == b.compare.at(y-1)){
          b.vec[x][y] = min( min(b.vec[x-1][y], b.vec[x][y-1])+1,  b.vec[x-1][y-1]);
        }else{
          b.vec[x][y] = min( min(b.vec[x-1][y], b.vec[x][y-1])+1,  b.vec[x-1][y-1]+1);
        }
      }
    }
  }
}


//reset the board for a new game
void reset(board& b){
  //clear the ncurses window
  clear();
  //empty all positions
  for(int x = 0; x < b.vec.size(); ++x){
      for(int y = 0; y < b.vec[0].size(); ++y){
          b.vec[x][y] = -1;
      }
  }

  b.vec = vector<vector<int> > (b.input.size()+1, vector<int> (b.compare.size()+1, -1));

  draw(b);
}

int main(int argc, char* argv[]){

  //initialize the ncurses screen
  initscr();

  //the character that we will read in from the user
  int c;

  //this is the struct that will contain all the values for the board
  board b;

  //where the user is "Pointing"
  //this will be represented by a '*' character in the corners of the cell
  b.curs_x = 0;
  b.curs_y = 0;

  b.input = "sunny";
  b.compare = "snowy";

  b.vec = vector<vector<int> > (b.input.size()+1, vector<int> (b.compare.size()+1, -1));

  //clear the board, and draw it.
  reset(b);

  //ncurses initialization
  //we will just be using the stdscr window(default)
  keypad(stdscr, true);//allows using the arrow keys
  curs_set(0);//remove the cursor from the screen
  noecho();//stop user keypresses from appearing on the screen

  solve(b);

  //draw the board
  draw(b);

  //main game loop, run once for each key press
  //if the escape keep (27) is pressed, escape
  while((c=getch()) !=27){
    //refresh the screen
    refresh();

    //Different arrow keys
    if(c == 259 && b.curs_y > 0){--b.curs_y;}
    else if(c == 258 && b.curs_y < b.vec[0].size()-1){++b.curs_y;}
    else if(c == 260 && b.curs_x > 0){--b.curs_x;}
    else if(c == 261 && b.curs_x < b.vec.size()-1){++b.curs_x;}
    //if spacebar, reset the board
    else if(c == 32){
      reset(b);
    }
    //draw what's changed
    draw(b);
  }

  //close the ncurses window
  endwin();
  return 0;
}

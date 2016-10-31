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
  int sols;
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

void popAlign(string& align){
  align = string(align, 1, align.size()-1);
}


void alignmentHelper(const board& b, string& iAlign, string& cAlign, vector<string>& iSols, vector<string>& cSols, int x, int y){
  if(x == 0 && y == 0){
    iSols.push_back(iAlign);
    cSols.push_back(cAlign);
  }else{

    //the the two things are the same, then don't add 1 when going diagonally
    int same = 0;
    if(x > 0 && y > 0 && b.input.at(x-1) != b.compare.at(y-1)){
      same = 1;
    }
    //can go diagonally
    if(x > 0 && y > 0 && b.vec[x-1][y-1]+same == b.vec[x][y]){
      pushAlign(b.input, iAlign, x-1);
      pushAlign(b.compare, cAlign, y-1);
      alignmentHelper(b, iAlign, cAlign, iSols, cSols, x-1, y-1);
      popAlign(iAlign);
      popAlign(cAlign);
    }
    //can add a space to iAlign
    if(y > 0 && b.vec[x][y-1]+1 == b.vec[x][y]){
      pushAlign(b.input, iAlign, -1);
      pushAlign(b.compare, cAlign, y-1);
      alignmentHelper(b, iAlign, cAlign, iSols, cSols, x, y-1);
      popAlign(iAlign);
      popAlign(cAlign);
    }
    //can add a space to cAlign
    if(x > 0 && b.vec[x-1][y]+1 == b.vec[x][y]){
      pushAlign(b.input, iAlign, x-1);
      pushAlign(b.compare, cAlign, -1);
      alignmentHelper(b, iAlign, cAlign, iSols, cSols, x-1, y);
      popAlign(iAlign);
      popAlign(cAlign);
    }
  }
}

void alignment(const board& b, vector<string>& iSols, vector<string>& cSols){
  string iAlign = "";
  string cAlign = "";
  alignmentHelper(b, iAlign, cAlign, iSols, cSols, b.curs_x, b.curs_y);
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
            snprintf (buffer, 3, "%2d", b.vec[x][y]);
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

void draw(board& b){
  drawBoard(b, 0,0);
  //the board uses b.vec[0].size()*2+2 lines
  int board_end = b.vec.size()*3+4;
  int spacing = 4;
  vector<string> iSols;
  vector<string> cSols;
  alignment(b, iSols, cSols);


  //populate the readouts
  mvprintw(1, board_end+spacing, "Number of edits shown by value at position on board");
  mvprintw(2, board_end+spacing, "Move with arrow keys, leave with esc.");
  mvprintw(3, board_end+spacing, "Alignmnets:");


  int docs_end = 5;

  //clear the readout lines
  for(int i = 0; i < b.sols; ++i){
    string temp(2*max(b.vec.size(), b.vec[0].size()), ' ');
    mvprintw(docs_end+(i*3), board_end+spacing, temp.c_str());
    mvprintw(docs_end+(i*3)+1, board_end+spacing, temp.c_str());
  }

  b.sols = iSols.size();
  for(int i = 0; i < b.sols; ++i){
    mvprintw(docs_end+(i*3), board_end+spacing, iSols[i].c_str());
    mvprintw(docs_end+(i*3)+1, board_end+spacing, cSols[i].c_str());
  }

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

  b.input = "snowy";
  b.compare = "sunny";

  b.vec = vector<vector<int> > (b.input.size()+1, vector<int> (b.compare.size()+1, -1));

  b.sols = 0;

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
    //draw what's changed
    draw(b);
  }

  //close the ncurses window
  endwin();
  return 0;
}

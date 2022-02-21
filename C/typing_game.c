/******** A Typing Game ********/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ncurses.h>

#define WIN_HEIGHT 40
#define WIN_WIDTH 65
#define WIN1_START_X 10
#define WIN2_START_X (WIN1_START_X + WIN_WIDTH + 20)
#define WIN_START_Y 10

#define TOT_ALPHA 26
#define TOT_NUMBERS 10

/* store alphabet frequency */
int alpha[TOT_ALPHA]={0};
/* store number frequency */
int nums[TOT_NUMBERS]={0};

bool is_num(int c) {
  if(c >= '0' && c <= '9') {
    return true;
  }
  return false;
}

bool is_char(int c) {
  if( (c >= 'a' && c <= 'z')
      || (c >= 'A' && c <= 'Z') ) {
    return true;
  }
  return false;
}

char get_first_char_for_case(int c) {
	if(c >= 'A' && c <= 'Z')
		return 'A';
	if(c >= 'a' && c <= 'z')
		return 'a';
	return ' ';
}

void printStat(WINDOW* output, time_t start_t, time_t end_t, int words){

	/* TODO 5a: print alphabet frequency stats using mvwprintw */
	// FUNCTION: mvwprintw(WINDOW, LINE, COLUMN, FORMAT, [ARGUMENTS...]);
	mvwprintw(output, LINE, COLUMN, "%s", [ARGUMENTS...]);
	/* TODO 5b: print number frequency stats using mvwprintw */
	mvwprintw(output, LINE, COLUMN, "%s", [ARDUMENTS...]);

	/* Print timing stats */
	double diff_in_sec;
	int h,m,s;
	float wpm = 0;

	diff_in_sec = difftime(end_t, start_t);
	wpm = diff_in_sec ? (words/diff_in_sec)*60 : 0;

	h = (int)(diff_in_sec / 3600);
	diff_in_sec -= h * 3600;
	m = (int)(diff_in_sec / 60);
	diff_in_sec -= m * 60;
	s = (int)diff_in_sec;

	/* TODO 5c: print timing stats using mvwprintw */
	mvwprintw(output, LINE, COLUMN, "%s", "Time elapsed till last printed character: ", h, ":", m, ":", s);

  	/* TODO 4: refresh window using wrefresh 
	 * You won't notice a difference though, until one of TODO 5 is implemented
	 * because there's no output on the window to display yet.
	 *
	 * TODO 5d: if you're curious, try commenting this out again
	 * once you've implemented one of TODO 5 & check the result */
	//wrefresh(output);
}

int main(int argc, char**argv){
	/* Initialize the screen */
	initscr();

	/* Make sure that Ctrl+C will exit the program */
	cbreak();

	/* Disable default key press echo */
	noecho();

	time_t start_t, end_t;
	srand(time(NULL));

	/* Create input window on the left with heading */
	mvwprintw(stdscr, WIN_START_Y - 2, WIN1_START_X, "%s", "Typing Window");
	WINDOW *input = newwin(WIN_HEIGHT, WIN_WIDTH, WIN_START_Y, WIN1_START_X);

	/* TODO 1a: Create output window on the right with heading. Use mvwprintw for heading, & newwin for new window */
	mvwprintw(stdscr, WIN_START_Y - 2, WIN1_START_X, "%s", "Typing Statistics");
	WINDOW *output = newwin(WIN_HEIGHT, WIN_WIDTH, WIN_START_Y, WIN1_START_X);

	refresh();

	/* Create box around input window */
	box(input,0,0);
	wrefresh(input);

	/* TODO 1b: Create box around output window */
	box(output,0,0);
	wrefresh(output);

	int inputChar = 0;
	int prevChar = -1;
	int words = 0;

	/* Following co-ordinates are relative to window */
	int input_y  = 1;
	int input_x  = 1;

	time(&start_t);

	while(1){
    	time(&end_t);

		/* TODO 3: fill in the right parameter for calling printStat */
		printStat(output, start_t, end_t, words); // output window is the first parameter

		/* Go to next line if width is exhausted */
		if (input_x == WIN_WIDTH - 1){
			input_x = 1;
			input_y++;
		}

		wmove(input, input_y, input_x);
		wrefresh(input);

		/* TODO 2a: Use wgetch to take single character input */
		inputChar = wgetch(input); // From intput window

		/* TODO 2b: getch() was temporarily added for making the window wait for an input.
		 * Comment this out after TODO 3a is completed.*/
		//getch();

		/* Handle alphabets */
		if(is_char(inputChar)) {
			char first_char = get_first_char_for_case(inputChar);
			alpha[inputChar - first_char]++;
			if(!is_char(prevChar) && !is_num(prevChar))
				words++;
		}
		/* Handle numbers */
		else if(is_num(inputChar)) {
			nums[inputChar-'0']++;
			if(!is_char(prevChar) && !is_num(prevChar))
				words++;
		}
		// handle enter
		else if(inputChar == '\n') {
			input_y++;
			input_x=1;
			prevChar = inputChar;
			continue;
		}
		// ignore backspace
		else if(inputChar == 127) {
			continue;
		}

		prevChar = inputChar;

		mvwprintw(input,input_y,input_x,"%c",inputChar);
		input_x++;
	}
	
	endwin();
	return 0;
}

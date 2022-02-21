
use ncurses::*;
use std::char::from_u32;
use std::time::Instant;

const TOT_ALPHA: usize = 26;
const TOT_NUMBERS: usize = 10;
const WIN_HEIGHT: i32 = 40;
const WIN_WIDTH: i32 = 60;
const WIN1_START_X: i32 = 10;
const WIN2_START_X: i32 = WIN1_START_X + WIN_WIDTH + 20;
const WIN_START_Y: i32 = 10;

fn is_char(a: i32) -> bool {
  if a >= 'a' as i32 && a <= 'z' as i32 {
    return true;
  }
  if a >= 'A' as i32 && a <= 'Z' as i32 {
    return true;
  }
  return false;
}

fn is_num(a: i32) -> bool {
  if a >= '0' as i32 && a <= '9' as i32 {
    return true;
  }
  return false
}

fn is_upper_case(a: i32) -> bool {
  if a >= 'A' as i32 && a <= 'Z' as i32 {
    return true;
  }
  return false;
}

fn print_stat(output: WINDOW, start: Instant, end: Instant, nwords: u64, alpha: &[i32], nums: &[i32]) {

  let mut curr_y = 2;

  /* print alphabet histogram */
  for i in 0..TOT_ALPHA {
    if alpha[i] > 0 {
      mvwprintw(
        output, curr_y, 10,
        &format!("{}: {}", from_u32(i as u32 + 'a' as u32).unwrap().to_string(), alpha[i]).to_string()
      );
      curr_y += 1;
    }
  }

  /* print number histogram */
  curr_y = 2;
  for i in 0..TOT_NUMBERS {
    if nums[i] > 0 {
      mvwprintw(
        output, curr_y, (WIN_WIDTH / 2) + 10,
        &format!("{}: {}",  from_u32(i as u32 + '0' as u32).unwrap().to_string(), nums[i]).to_string()
      );
      curr_y += 1;
    }
  }

  /* Print timing stats */
  let mut diff_in_sec = end.duration_since(start).as_secs();

  let wpm = if diff_in_sec > 0 { (nwords as f64 / diff_in_sec as f64) * (60 as f64) } else { 0.0 };
  let h = diff_in_sec / 3600;
  diff_in_sec -= h * 3600;
  let m = diff_in_sec / 60;
  diff_in_sec -= m * 60;
  let s = diff_in_sec;

  /* TODO 5c: print timing stats using mvwprintw */

  /* TODO 4: refresh window using wrefresh 
  * You won't notice a difference though, until one of TODO 5 is implemented
  * because there's no output on the window to display yet.
  *
  * TODO 5d: if you're curious, try commenting this out again
  * once you've implemented one of TODO 5 & check the result */
}


fn main() {

  // Initialize the screen
  initscr();

  // Make sure that Ctrl+C will exit the program
  cbreak();

  // Disable default key press echo
  noecho();

  // Create windows and their titles
  mvwprintw(stdscr(), WIN_START_Y - 2, WIN1_START_X, "Typing Window");
  let input = newwin(WIN_HEIGHT, WIN_WIDTH, WIN_START_Y, WIN1_START_X);
  
  /* TODO 1a: Create output window on the right with heading. Use mvwprintw for heading, & newwin for new window */

  refresh();

  /* Create box around input window */
  box_(input,0,0);
  wrefresh(input);

  /* TODO 1b: Create box around output window */

  /* Following co-ordinates are relative to window*/
  let mut input_x = 1;
  let mut input_y = 1;

  let mut input_char;
  let mut prev_char = 0;
  let mut nwords: u64 = 0;
  let mut alpha: [i32; 26] = [0; 26];
  let mut nums: [i32; 10] = [0; 10];

  let start = Instant::now();
  let mut end: Instant;
  loop {
    end = Instant::now();
    /* TODO 3: fill in the right parameter for calling printStat */
    // print_stat(____, start, end, nwords, &alpha, &nums);
    if input_x == WIN_WIDTH - 1 {
      input_x = 1;
      input_y += 1;
    }

    wmove(input, input_y, input_x);
    wrefresh(input);

    /* TODO 2a: Use wgetch to take single character input */
    // input_char = ____;

    /* TODO 2b: getch() was temporarily added for making the window wait for an input.
    * Comment this out after TODO 3a is completed.*/
    getch();

    /* Handle alphabets */
    if is_char(input_char) {
      if is_upper_case(input_char) {
        alpha[(input_char - 'A' as i32) as usize] += 1;
      }
      else {
        alpha[(input_char - 'a' as i32) as usize] += 1;
      }

      if !is_char(prev_char) && !is_num(prev_char) {
        nwords += 1;
      }
    }
    /* Handle numbers */
    else if is_num(input_char) {
      nums[(input_char - '0' as i32) as usize] += 1;
      if !is_char(prev_char) && !is_num(prev_char) {
        nwords += 1;
      }
    }
    // handle enter
    else if input_char == 10 {
      input_y += 1;
      input_x=1;
      prev_char = input_char;
      continue;
    }
    // ignore backspace
    else if input_char == 127 {
      continue;
    }

    prev_char = input_char;
    mvwprintw(input, input_y, input_x, &from_u32(input_char as u32).unwrap().to_string());
    input_x += 1;
  }

  endwin();
}

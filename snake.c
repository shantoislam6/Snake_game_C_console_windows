/*-------------------------------------
 * A Simple Terminal Based Snake Game |
 * Implementing Queue Data Structure  |
 * Date: 4-12-2022                    |
 * ------------------------------------
*/

#include <stdio.h>
#include <windows.h>
#include <conio.h>

// save as utf-8
#pragma execution_character_set("utf-8")

// Define Arrow Keys
#define UP 72
#define DOWN 80
#define LEFT 75
#define RIGHT 77
#define ROWs_SIZE 19;
#define COLs_SIZE 45;
// Static Speed
#define SPEED 100;

// Create Game State Instance
typedef struct
{
  int quit;
  int game_over;
  int speed;
  int score;
  int reset;
  int pause;
} Game_state;

// Create Snake Instance
typedef struct
{
  int dx;
  int dy;
  int len;
  int start_row;
  int newRow;
  int newCol;
  int** body;
} Snake;

// Create Food Instance
typedef struct
{
  int row;
  int col;
  int start_row;
} Food;

void Snake_Game();

int main(void)
{
  // set console code page to utf-8
  SetConsoleOutputCP(CP_UTF8);

  // Init sanke game
  Snake_Game();
  return 0;
}

// Create Instruction
void Instruction()
{
  // Show Instructions
  printf("\n\n\n\033[38;5;242m"
    " ┌ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┐\n"
    "             INSTRUCTION            \n"
    " └ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┘\n"
    "       Up: (↑Up Arrow) Or (w)\n"
    "     Left: (←Left Arrow) Or (a)\n"
    "    Right: (→Right Arrow) Or (d)\n"
    "    Down: (↓Down Arrow) Or (s)\n"
    "     Quit: (shift + q) Or (Q)\n"
    "  Restart: (shift + r) Or (R) \n"
    "  To Toggle Pause & Resume Press (p)\n\n"
    " ┌─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ┐\n"
    "   IF SNAKE HEAD TOUCHES IT TAILS  \n"
    "           THEN GAMEOVER           \n"
    " └ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─ ─┘\n");
  printf("  Press Any Key To Continue...");
  getch();

}

// Create Game Dispay
void Display(int ROWS, int COLS, Game_state* state) {

  // Reset the terminal
  printf("\033c");

  // Hide Cursor
  printf("\033[?25l");

  // Show score
  printf("\n\n\033[%iC\033[38;5;242mScore:%d\n", (COLS - 7) / 2, state->score);

  // Create Table·
  printf("\033[38;5;239m┌");
  for (int i = 0; i < COLS; i++)
    printf("─");
  printf("┐\n");
  for (int row = 1; row <= ROWS; row++)
  {
    printf("│");
    for (int col = 1; col <= COLS; col++)
    {
      printf("\033[38;5;236m·");
    }
    printf("\033[38;5;239m│\n");
  }
  printf("└");
  for (int i = 0; i < COLS; i++)
    printf("─");
  printf("┘");
}

void Draw(Snake* snake, Food* food,
  Game_state* state, int ROWS, int COLS) {

  //Draw Food
  printf("\033[%i;%iH%s⚬\033[m",
    food->row + food->start_row, food->col,
    state->game_over ? "\033[38;5;22m" : "\033[38;5;196m");

  // Draw Snake
  for (int i = 0; i < snake->len; i++)
    printf("\033[%i;%iH%s%s",
      snake->body[i][0] + snake->start_row,
      snake->body[i][1],
      state->game_over ? "\033[38;5;88m" : "\033[m",
      i == snake->len - 1 ? "o" : "⚬");

  // Create dynamic alignment for score
  char score_len_holder[13];
  int score_len = state->score == 0 ? 1 :
    sprintf(score_len_holder, "%d", state->score);
  int alignment = (COLS - (score_len + 6)) / 2;
  // Show Score
  printf("\033[%i;1H\033[2K\033[%iC\033[38;5;242mScore:%d\033[m",
    snake->start_row, alignment, state->score);

  // Show when game is pause
  if (state->pause)
    printf("\033[%i;%iH\033[38;5;240mPAUSED!",
      ((ROWS - 3) / 2) + 5, (COLS / 2) - 1);

  // Show When game is over
  if (state->game_over)
    printf("\033[%i;%iH\033[38;5;240m Game Over! "
      "\n\033[%iC Play Again? \n\033[%iC y / n",
      ((ROWS - 3) / 2) + 3, (COLS / 2) - 4,
      (COLS / 2) - 6, (COLS / 2) - 3);
}
void DrawReset(
  Snake* snake,
  Food* food,
  Game_state* state,
  int ROWS, int COLS) {
  // Reset Snake Position
  for (int i = 0; i < snake->len; i++)
    printf("\033[%i;%iH\033[38;5;236m·\033[m",
      snake->body[i][0] + snake->start_row,
      snake->body[i][1]);

  // Reset Paused Positon 
  if (!state->pause)
    printf("\033[%i;%iH\033[38;5;236m·······",
      ((ROWS - 3) / 2) + 5, (COLS / 2) - 1);
}


// Set Snake Direction
void snake_dir(Snake* snake, int dx, int dy)
{
  snake->dy = dy;
  snake->dx = dx;
}

// Set radome position for foot  
int check_food_pos(Food* food, Snake* snake) {
  // check if food pos in snake 
  for (int i = 0; i < snake->len; i++) {
    if (snake->body[i][0] == food->row
      && snake->body[i][1] == food->col) {
      return 1;
    }
  }
  return 0;
}
void Set_food_ran_position(Food* food, Snake* snake,
  const int ROWS, const int COLS)
{
  do
  {
    do
      food->row = (rand() % ROWS) + 2;
    while (food->row < 2 || food->row > ROWS + 1);
    do
      food->col = (rand() % COLS) + 2;
    while (food->col < 2 || food->col > COLS + 1);
  } while (check_food_pos(food, snake));
}

// Handle snake direction
void Input_Handler(
  Snake* snake,
  Game_state* state)
{
  // Handle game input
  if (kbhit())
  {
    int k = getch();
    
    if ((k == 'w' || k == 'W' || k == UP) && !snake->dy)
      snake_dir(snake, 0, -1);
    if ((k == 's' || k == 'S' || k == DOWN) && !snake->dy)
      snake_dir(snake, 0, 1);
    if ((k == 'a' || k == 'A' || k == LEFT) && !snake->dx)
      snake_dir(snake, -1, 0);
    if ((k == 'd' || k == 'D' || k == RIGHT) && !snake->dx)
      snake_dir(snake, 1, 0);
    if (k == 'p') {
      if (state->pause)
        state->pause = 0;
      else
        state->pause = 1;
    }
    if (k == 'Q' || k == '\3')
      state->quit = 1;
    if (k == 'R')
      state->reset = 1;
  }
}

// Create Game Logic
void Game_logic(
  Game_state* state,
  Snake* snake,
  Food* food,
  int ROWS, int COLS)
{
  // When state pause
  if (state->pause) return;

  int i;

  // Set new row and column for snake head
  snake->newRow = snake->body[snake->len - 1][0] + snake->dy;
  snake->newCol = snake->body[snake->len - 1][1] + snake->dx;

  // If snake head touch tail
  for (i = 1; i < snake->len; i++) {
    if (snake->body[i][0] == snake->newRow
      && snake->body[i][1] == snake->newCol) {
      state->game_over = 1;
      return;
    }
  }

  // Check If head touch food
  if (snake->body[snake->len - 1][0] == food->row
    && snake->body[snake->len - 1][1] == food->col) {
    Set_food_ran_position(food, snake, ROWS, COLS);
    //Increase snake tail and score
    snake->len++;
    state->score++;
  }

  else {
    // Otherwise dequeuing Snake Postions
    for (i = 0; i < snake->len - 1; i++)
    {
      snake->body[i][0] = snake->body[i + 1][0];
      snake->body[i][1] = snake->body[i + 1][1];
    };
  }

  // Enqueuing Snake Positions
  snake->body[snake->len - 1][0] = snake->newRow;
  snake->body[snake->len - 1][1] = snake->newCol;

  // If snake hit the edge of table
  if (snake->newRow > ROWS + 1) {
    snake->body[snake->len - 1][0] = 2;
    snake->body[snake->len - 1][1] = snake->newCol;
  }
  else if (snake->newRow < 2) {
    snake->body[snake->len - 1][0] = ROWS + 1;
    snake->body[snake->len - 1][1] = snake->newCol;
  }
  else if (snake->newCol > COLS + 1) {
    snake->body[snake->len - 1][0] = snake->newRow;
    snake->body[snake->len - 1][1] = 2;
  }
  else if (snake->newCol < 2) {
    snake->body[snake->len - 1][0] = snake->newRow;
    snake->body[snake->len - 1][1] = COLS + 1;
  }

}

// Create Snake Game
void Snake_Game()
{
  printf("\033c");
  Instruction();

  int ROWS = ROWs_SIZE;
  int COLS = COLs_SIZE;

  // Create state;
  Game_state state;
  // Create Snake, Food
  Snake snake;
  Food food;

  // Create snake body
  snake.body = malloc(sizeof(int*) * ((COLS * ROWS) + (COLS + ROWS)));
  for (int i = 0; i < COLS * ROWS; i++)
    snake.body[i] = malloc(sizeof(int) * 2);

Play:

  // Show Screen
  Display(ROWS, COLS, &state);

  //Set initial game state property
  state.score = 0;
  state.game_over = 0;
  state.reset = 0;
  state.quit = 0;
  state.speed = SPEED;
  state.pause = 0;

  // Set initial positon and direction for snake;
  snake.body[0][0] = 6;
  snake.body[0][1] = 4;
  snake.body[1][0] = 6;
  snake.body[1][1] = 5;
  snake.len = 2;

  snake.dx = 1;
  snake.dy = 0;
  snake.start_row = food.start_row = 3;

  // // Set Initial Random Positon for Food
  Set_food_ran_position(&food, &snake, ROWS, COLS);

  // Game Loop
  while (!state.quit && !state.game_over && !state.reset)
  {
    Input_Handler(&snake, &state);
    DrawReset(&snake, &food, &state, ROWS, COLS);
    Game_logic(&state, &snake, &food, ROWS, COLS);
    Draw(&snake, &food, &state, ROWS, COLS);
    Sleep(state.speed);
  }

  // If game state is reset mode
  if (state.reset) {
    state.reset = 0;
    goto Play;
  }

  // If snake gamer over and state doesn't quit
  if (!state.quit && state.game_over)
  {
    // Prompt
    char promt;
  RePromt:
    promt = getch();
    // replay
    if (promt == 'y' || promt == 'Y')
    {
      state.game_over = 0;
      state.score = 0;
      goto Play;
    }
    else if (promt == 'n' || promt == 'N'
      || promt == 'Q' || promt == '\3')
    {
      // Reset Terminal
      printf("\033c");
    }
    else {
      goto RePromt;
    }
  }

  // Deallocating snake body
  for (int i = 0; i < ROWS * COLS; i++)
    free(snake.body[i]);
  free(snake.body);

  // Otherwise Reset The Terminal and Exit the game;
  printf("\033c");

}

#include <Adafruit_GFX.h>   // Core graphics library
#include <RGBmatrixPanel.h> // Hardware-specific library

#define CLK 11 // clock
#define LAT A3
#define OE  9
#define A   A0
#define B   A1
#define C   A2
#define INIT_LEN (7) // initial snake length
#define PRIME1 (37)
#define TABLE_SIZE (512)
#define ROWS (31)
#define COLUMNS (15)
#define MEDIAN_HEIGHT (10)
#define MAX_SNAKES (2)
#define SLOWDOWN (30)

typedef struct snake *Snake;
typedef struct arena *Arena;
typedef struct hash *Hash;

struct snake {
  Hash head;
  Hash tail;
  byte grow;
  int vector; // x++ = 1, x-- = 1, y++ = 2, y-- = -2
  // vector gives the direction in which a snake is headed
  uint8_t r, g, b;
  int slow_count; // gives the amount of "time" a snake moves at half-speed
};

struct arena {
  Snake *snakes; // all snakes in the arena
  Hash occupied[ROWS + 2][COLUMNS + 2]; // all occupied pixels
  Hash food; // points to location at which food is stored
  byte foodtype; // 0 default, 1 for regular food, 2 for hyperjump, 3 for shrink opponent, 4 for slow opponent
};

struct hash {
  byte on; // specifies whether pixel is occupied or not
  signed char x;
  signed char y;
  Hash next; // linked list so we can easily draw the snake
  Hash prev;
};

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

int UD1 = 0; // up/down, left/right inputs
int LR1 = 0;

int UD2 = 0;
int LR2 = 0;

// input pins
int IUP1=A5;
int ILR1=A4;
int IUP2=A7;
int ILR2=A6;

int MID = 50; // sensitivity
int LRMID1 = 0; // centered coordinates
int UPMID1 = 0;
int LRMID2 = 0;
int UPMID2 = 0;
int debounce1 = 0; // for debouncing input
int debounce2 = 0;
byte tiebreaker = 0; // for determining who wins in a head-on collision

unsigned long timer = 0;
unsigned long timer1 = 0; // debounce timer for snake1
unsigned long timer2 = 0;

Arena initArena() {
  Arena arena = malloc(sizeof(*arena));
  arena->snakes = malloc(sizeof(*arena->snakes) * MAX_SNAKES);
  for (char i = 0; i < MAX_SNAKES; i++) {
    arena->snakes[i] = 0; // initialize snake pointers
  }
  arena->food = 0;
  arena->foodtype = 0;
  for (signed char i = 0; i < ROWS + 1; i++) {
    for (signed char j = 0; j < COLUMNS + 1; j++) {
      arena->occupied[i][j] = malloc(sizeof(*arena->occupied[i][j])); // malloc for all pixels (not optimal, but not too bad)
      arena->occupied[i][j]->on = 0;
      arena->occupied[i][j]->x = i;
      arena->occupied[i][j]->y = j;
      arena->occupied[i][j]->next = 0;
      arena->occupied[i][j]->prev = 0;
    }
  }
  return arena;
}

void freeArena(Arena arena) {
  for (signed char i = 0; i < ROWS + 1; i++) {
    for (signed char j = 0; j < COLUMNS + 1; j++) {
      free(arena->occupied[i][j]);
    }
  }
  free(arena->snakes);
  free(arena);
}

void freeSnake(Snake snake) {
  free(snake);
}

// function that turns off all pixels for the defeated snake
void destroySnake(Snake snake) {
  Hash temp = snake->head;
  while (temp != 0) {
    matrix.writePixel(temp->x, temp->y, matrix.Color333(0, 0, 0));
    delay(200);
    temp = temp->next;
  }
}

// function that performs all tasks related to the head of the snake
// 1) detects if there's a collision
// 2) handles eating
// 3) moves the head in the direction specified by snake->vector
Snake addToHead(Snake snake, signed char x, signed char y, Arena arena) {
  if (arena->occupied[x][y]->on == 1) {
    delay(1000);
    destroySnake(snake);
    snake->vector = 0; // mark for deletion
    matrix.fillScreen(matrix.Color333(0, 0, 0)); // blackout
    return snake;
  }
  arena->occupied[x][y]->on = 1;
  if(x == arena->food->x && y == arena->food->y) {
      Snake opponent;
      if (snake == arena->snakes[0]) {
        opponent = arena->snakes[1];
      } else {
        opponent = arena->snakes[0];
      }
      if (arena->foodtype == 2) { // check for HYPERJUMP
        arena->occupied[x][y]->on = 0;
        matrix.writePixel(x, y, matrix.Color333(0, 0, 0)); // delete old food
        do {
          x = (signed char) random(1, ROWS);
          y = (signed char) random(1, COLUMNS);
          // pad landing zone
        } while (arena->occupied[x][y]->on == 1 || arena->occupied[x + 1][y]->on == 1
                 || arena->occupied[x - 1][y]->on == 1 || arena->occupied[x][y + 1]->on == 1
                 || arena->occupied[x][y - 1]->on == 1 || arena->occupied[x + 1][y + 1]->on == 1
                 || arena->occupied[x - 1][y - 1]->on == 1 || arena->occupied[x + 1][y - 1]->on == 1
                 || arena->occupied[x - 1][y + 1]->on == 1); // find this snake a new place to land that's relatively safe
        arena->occupied[x][y]->on = 1;
      } else if (arena->foodtype == 3) { // check for OPPONENT SHRINK
        if (opponent->head->next->next != NULL) { // make sure it's at least 2 units long
          removeTail(opponent, arena);
        }
      } else if (arena->foodtype == 4) {
        opponent->slow_count = SLOWDOWN;
      }
    snake->grow = 1;
    setFood(arena);
  }
  matrix.writePixel(x, y, matrix.Color333(snake->r, snake->g, snake->b)); // turn on LED in matrix at new head
  Hash temp = snake->head;
  snake->head = arena->occupied[x][y]; // point to new head
  snake->head->next = temp; // point new to old
  temp->prev = snake->head; // point back up
  return snake;
}

// turn a pixel off
void expunge(Hash h) {
  h->on = 0;
  h->next = 0;
  h->prev = 0;
}

 // takes care of tail pointer
Snake removeTail(Snake snake, Arena arena) {
  matrix.writePixel(snake->tail->x, snake->tail->y, matrix.Color333(0, 0, 0)); // turn off LED in matrix at old tail
  Hash temp = snake->tail;
  snake->tail = snake->tail->prev;
  snake->tail->next = 0;
  expunge(temp);
  return snake;
}

// uses snake->vector to move snake
Snake moveSnake(Snake snake, Arena arena) {
  if (snake->vector == 0) {
    return snake;
  }
  signed char x = snake->head->x;
  signed char y = snake->head->y;
  if (snake->slow_count == 0 || snake->slow_count % 2 == 0) {
    if (snake->vector == 1) {
      x = x + 1;
      if (x > ROWS) {
        x = 0;
      }
    } else if (snake->vector == -1) {
      x = x - 1;
      if (x < 0) {
        x = ROWS;
      }
    } else if (snake->vector == 2) {
      y = y + 1;
      if (y > COLUMNS) {
        y = 0;
      }
    } else {
      y = y - 1;
      if (y < 0) {
        y = COLUMNS;
      }
    }
    snake = addToHead(snake, x, y, arena);
    if (snake->vector == 0) { // snake has been removed from LED, abort
      return snake;
    }
    if (snake->grow == 1) { // dont shrink the end this time
      snake->grow = 0;
    } else { // okay, shrink the end
      snake = removeTail(snake, arena);
    } if (snake->slow_count != 0) {
      snake->slow_count--;
    }
  } else {
    snake->slow_count--;
  }
  return snake;
}

// debugging tool
void printSnake(Snake snake) {
  Serial.print(snake->head->on);
  Serial.print(" ");
  Serial.print(snake->head->x);
  Serial.print(" ");
  Serial.print(snake->tail->on);
  Serial.print(" ");
  Serial.print(snake->tail->x);
}

// debugging tool
void printArena(Arena arena) {
  for (signed char i = 0; i < ROWS + 1; i++) {
    for (signed char j = 0; j < COLUMNS + 1; j++) {
      Serial.print(arena->occupied[i][j]->on);
      Serial.print(" ");
    }
    Serial.println("");
  }
  Serial.println("");
  Serial.println("");
}

// randomly places food in the arena, but not inside a snake
void setFood(Arena arena) {
  signed char x = (signed char) random(1, ROWS);
  signed char y = (signed char) random(1, COLUMNS);
  if (arena->occupied[x][y]->on == 1) { // if food is placed inside a snake, retry!
    setFood(arena);
  } else {
    arena->food = arena->occupied[x][y]; // point to food location
    signed char type = (signed char) random(1, 5);
    arena->foodtype = type;
    signed char r, g, b;
    switch(type) {
      case 1:
        r = 4, g = 4, b = 0; // yellow indicates regular food
        break;
      case 2:
        r = 0, g = 4, b = 2; // teal causes player to jump
        break;
      case 3:
        r = 4, g = 0, b = 1; // magenta shrinks enemy player
        break;
      case 4:
        r = 0; g = 0, b = 4; // deep blue slows enemy player down
        break;
    }
    matrix.writePixel(x, y, matrix.Color333(r, g, b));
  }
}

// initialization for a snake
Snake createSnake(signed char x, signed char y, int vector, Arena arena, int rgb) {
  Snake snake = malloc(sizeof(*snake));
  arena->snakes[rgb] = snake; // point to this snake
  snake->vector = vector;
  snake->grow = 0;
  if (rgb == 0) {
    snake->r = 0;
    snake->g = 3;
    snake->b = 0;
  } else if (rgb == 1) {
    snake->r = 3;
    snake->g = 0;
    snake->b = 0;
  } else {
    snake->r = 3;
    snake->g = 3;
    snake->b = 0;
  }
  snake->slow_count = 0;
  snake->head = arena->occupied[x][y];
  snake->head->next = arena->occupied[x - 1][y];
  snake->tail = snake->head->next;
  snake->tail->prev = snake->head;
  arena->occupied[x - 1][y]->on = 1;
  matrix.writePixel(x - 1, y, matrix.Color333(snake->r, snake->g, snake->b));
  for (signed char i = 0; i < INIT_LEN - 1; i++) {
    snake = addToHead(snake, x + i, y, arena);
  }
  return snake;
}

Arena arena = initArena();
Snake snake1 = createSnake(3, MEDIAN_HEIGHT - 1, 1, arena, 0);
Snake snake2 = createSnake(3, MEDIAN_HEIGHT, 1, arena, 1);

void setup(){
  Serial.begin(9600);
  matrix.begin();
  //calibrate center
  LRMID1 = analogRead(ILR1);
  UPMID1 = analogRead(IUP1);
  LRMID2 = analogRead(ILR2);
  UPMID2 = analogRead(IUP2);
  mainMenu();
  setFood(arena);
  randomSeed(analogRead(A8));
}

// fun introduction
void mainMenu() {
  matrix.setTextSize(1);
  for (int i = 10; i > 0; i--) {
    matrix.setTextColor(matrix.Color333(random(1,5), random(1,5), random(1,5)));
    matrix.setCursor(0, 0);
    matrix.print('S');
    matrix.setTextColor(matrix.Color333(random(1,5), random(1,5), random(1,5)));
    matrix.print('N');
    matrix.setTextColor(matrix.Color333(random(1,5), random(1,5), random(1,5)));
    matrix.print('A');
    matrix.setTextColor(matrix.Color333(random(1,5), random(1,5), random(1,5)));
    matrix.print('K');
    matrix.setTextColor(matrix.Color333(random(1,5), random(1,5), random(1,5)));
    matrix.print('E');
    delay(200);
  }
  matrix.fillRect(0, 0, 32, 8, matrix.Color333(0, 0, 0)); // blackout
  matrix.setCursor(9, 0);
  matrix.print('G');
  matrix.print('O');
  matrix.print('!');
  delay(800);
  matrix.fillRect(0, 0, 32, 8, matrix.Color333(0, 0, 0)); // blackout
}

void loop(){
  // x < 31, y < 15
  // read from the joysticks
  int UD1 = analogRead(IUP1);
  int LR1 = analogRead(ILR1);
  int UD2 = analogRead(IUP2);
  int LR2 = analogRead(ILR2);
  if(UD1 > UPMID1 + MID && snake1->vector != 2){ // can't turn immediately around
   if (!debounce1) {
      timer1 = millis();
      debounce1 = 1;
      snake1->vector = -2;
   }
  }
  
  if(UD1 < UPMID1 - MID && snake1->vector != -2){ // can't turn immediately around
   if (!debounce1) {
      timer1 = millis();
      debounce1 = 1;
      snake1->vector = 2;
   }
  }
  // LEFT-RIGHT
  if(LR1 > LRMID1 + MID && snake1->vector != -1){
   if (!debounce1) {
      timer1 = millis();
      debounce1 = 1;
      snake1->vector = 1;
   }
  }
  
  if(LR1 < LRMID1 - MID && snake1->vector != 1){
   if (!debounce1) {
      timer1 = millis();
      debounce1 = 1;
      snake1->vector = -1;
   }
  }

  if(UD2 > UPMID2 + MID && snake2->vector != -2){
   if (!debounce2) {
      timer2 = millis();
      debounce2 = 1;
      snake2->vector = 2;
   }
  }
  
  if(UD2 < UPMID2 - MID && snake2->vector != 2){
   if (!debounce2) {
      timer2 = millis();
      debounce2 = 1;
      snake2->vector = -2;
   }
  }
  // LEFT-RIGHT
  if(LR2 > LRMID2 + MID && snake2->vector != 1){
   if (!debounce2) {
      timer2 = millis();
      debounce2 = 1;
      snake2->vector = -1;
   }
  }
  
  if(LR2 < LRMID2 - MID && snake2->vector != -1){
   if (!debounce2) {
      timer2 = millis();
      debounce2 = 1;
      snake2->vector = 1;
   }
  }
  
  // updating screen
  if (tiebreaker) { // alternate winner in head-on collisions
    snake1 = moveSnake(snake1, arena);
    snake2 = moveSnake(snake2, arena);
  } else {
    snake2 = moveSnake(snake2, arena);
    snake1 = moveSnake(snake1, arena);
  }
  // check if need to restart the game
  if (snake1->vector == 0 || snake2->vector == 0) {
    freeSnake(snake1);
    freeSnake(snake2);
    freeArena(arena);
    matrix.fillScreen(matrix.Color333(0, 0, 0)); // blackout
    arena = initArena();
    snake1 = createSnake(3, MEDIAN_HEIGHT - 1, 1, arena, 0);
    snake2 = createSnake(3, MEDIAN_HEIGHT, 1, arena, 1);
    mainMenu();
    setFood(arena);
  }

  delay(100);
  tiebreaker = tiebreaker ^ 1;
  // printArena(arena);
/*
  printSnake(snake1);
  Serial.print("    ||    ");
  printSnake(snake2);
  Serial.println("");
  */
  
  if (timer - timer1 >= 25) {
    debounce1 = 0;
  }
  if (timer - timer2 >= 25) {
    debounce2 = 0;
  }
  timer = millis();
  // Serial.println(arena->snakes[1]->slow_count);
}


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// INIT
// Set the directions

typedef struct {
   int x;
   int y;
} vector;

vector dirs[] = {
   { .x = 0,  .y = -1 },
   { .x = -1,  .y = 0 },
   { .x = 0,  .y = 1 },
   { .x = 1,  .y = 0 },
   { .x = 1,  .y = 1 },
   { .x = -1, .y = -1 },
   { .x = 1,  .y = -1 },
   { .x = -1, .y = 1 }
};

#define NUMDIRS sizeof(dirs)/sizeof(vector)

// Set the wordlist

char *wordlist[] = {
    "DOG",
    "CAT",
    "GIRAFFE",
    "HEN",
    "MONKEY",
    "SNAKE",
    "BADGER",
    "LEMUR",
    "DONKEY",
    "ZEBRA",
    "TOUCAN",
    "CHICKEN",
    "RAT",
    "PIGLET",
    "OWL"
};

#define NUMWORDS sizeof(wordlist)/sizeof(char *)

// Set the grid

#define SIZE 15
char **grid;

// FUNCTIONS

void destroyGrid(void)
{
   for (int i=0; i<SIZE; i++) {
      free(grid[i]);
   }
   free(grid);
}

void setupGrid(void)
{
   grid = malloc(SIZE * sizeof(char *));
   for (int i=0; i<SIZE; i++) {
      grid[i] = malloc(SIZE * sizeof(char));
      for (int j=0; j<SIZE; j++) {
         grid[i][j] = '-';
      }
   }
}

void blankGrid(void)
{
   for (int i=0; i<SIZE; i++) {
      for (int j=0; j<SIZE; j++) {
         grid[i][j] = '-';
      }
   }
}

void printBorder(void)
{
   printf("+");
   for (int i=0; i<SIZE; i++) {
      printf("-+");
   }
   printf("\n");
}

void printGrid(void)
{
   printBorder();
   for (int i=0; i<SIZE; i++) {
      printf("|");
      for (int j=0; j<SIZE; j++) {
         printf("%c|", grid[i][j]);
      }
      printf("\n");
   }
   printBorder();
}

int inBounds(int x, int y, vector v, int len)
{
   int bx = x + (v.x * len);
   int by = y + (v.y * len);
   return (bx < SIZE && bx >= 0) && (by < SIZE && by >= 0);
}

int inClear(int x, int y, vector v, char *word)
{
   for (int a=0; a<strlen(word); a++) {
      if (grid[y][x] != '-' && grid[y][x] != word[a]) {
         return 0;
      }
      x += v.x;
      y += v.y;
   }
   return 1;
}

void populateGrid1(char *word, int count)
{
   srand((long)grid);

   int done = 0;
   int len = strlen(word);
   
   while (done < count) {
      int x = rand() % SIZE;
      int y = rand() % SIZE;
      int dir = rand() % NUMDIRS; 

      if (inBounds(x, y, dirs[dir], len) && inClear(x, y, dirs[dir], word)) {
         for (int a=0; a<len; a++) {
            grid[y][x] = word[a];
            x += dirs[dir].x;
            y += dirs[dir].y;
         }
         done++;
      }
   }
}

void populateGrid2(int count)
{
   srand((long)grid);
   
   int done = 0;
   
   while (done < count) {
      char *word = wordlist[rand() % NUMWORDS];
      int len = strlen(word);

      int x = rand() % SIZE;
      int y = rand() % SIZE;
      int dir = rand() % NUMDIRS; 

      if (inBounds(x, y, dirs[dir], len) && inClear(x, y, dirs[dir], word)) {
         for (int a=0; a<len; a++) {
            grid[y][x] = word[a];
            x += dirs[dir].x;
            y += dirs[dir].y;
         }
         done++;
      }
   }
}

void populateGrid3(char *watchword, int count)
{
   srand((long)grid);
   
   int done = 0;
   
   while (done < count) {
      char *word = wordlist[rand() % NUMWORDS];
      int len = strlen(word);

      int x = rand() % SIZE;
      int y = rand() % SIZE;
      int dir = rand() % NUMDIRS; 

      if (inBounds(x, y, dirs[dir], len) && inClear(x, y, dirs[dir], word)) {
         if (!strcmp(word, watchword)) {
            printf("Placed %s at (%d, %d) with dir (%d, %d)\n", word, x, y, dirs[dir].x, dirs[dir].y);
         }

         for (int a=0; a<len; a++) {
            grid[y][x] = word[a];
            x += dirs[dir].x;
            y += dirs[dir].y;
         }
         done++;
      }
   }
}

void fillBlanks(void)
{
   for (int i=0; i<SIZE; i++) {
      for (int j=0; j<SIZE; j++) {
         if (grid[i][j] == '-') {
            grid[i][j] = 65 + (rand() % 26); // Assign a random capital letter
         }
      }
   }
}

int startBTA(int x, int y, char *word, vector dir, int len, int idx)
{
   if (idx == len) {
      // We found it and we're done
      return 1;
   } else if (grid[y][x] == word[idx]) {
      // Right letter, but is there space for the rest of the word?
      if (inBounds(x, y, dir, len - idx)) {
         int newx = x + dir.x;
         int newy = y + dir.y;

         if (startBTA(newx, newy, word, dir, len, idx + 1)) {
            return 1;
         }
      }
   }
   return 0;
}

void findWord(char *word)
{
   int len = strlen(word);
   int count = 0;

   for (int y=0; y<SIZE; y++) {
      for (int x=0; x<SIZE; x++) {
         for (int d=0; d<NUMDIRS; d++) {
            if (startBTA(x, y, word, dirs[d], len, 0)) {
               printf("Found %s at (%d, %d) with dir (%d, %d)\n", word, x, y, dirs[d].x, dirs[d].y);
               count++;
            }
         }
      }
   }
   printf("Found %d of %s\n", count, word);
}

int main(void)
{
   char staticword[] = "DOG";
   char watchword[] = "TOUCAN";
   int numwords = 25;

   // Allocate the memory and blank the values
   setupGrid();

   // Show the blank grid
   printf("Blank grid:\n");
   printGrid();

   // Sprinkle staticword numwords times randomly around the grid - within bounds, clash only if letter is shared
   printf("Populated with single word:\n");
   populateGrid1(staticword, numwords);
   printGrid();

   // Clear it down
   blankGrid();

   // Sprinkle numwords random words randomly around the grid, reporting on watchword - within bounds, clash only if letter is shared
   printf("Populated with words from list:\n");
   populateGrid3(watchword, numwords);
   printGrid();

   // Fill her up
   printf("And filled:\n");
   fillBlanks();
   printGrid();

   // Find the occurrences of watchword
   printf("Finding %s:\n", watchword);
   findWord(watchword);

   // Free the memory
   destroyGrid();

   // And we're done
   return 0;
}

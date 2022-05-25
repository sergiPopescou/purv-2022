/*
* ETF Banja Luka (Purv - 2022)
* Aleksanda Beslic 1198/17
* Conway game of life 3x3
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAP_SIZE 3
#define INPUT_SIZE 10

char map[MAP_SIZE][MAP_SIZE] = {0};
int generation = 0;

void printMap()
{
     printf("Generation: %d\n", generation);
     for (size_t i = 0; i < MAP_SIZE; i++)
     {
          for (size_t j = 0; j < MAP_SIZE; j++)
          {
               printf("%c ", map[i][j]);
          }

          printf("\n");
     }

     printf("\n");
}

void parseInput(char** input)
{
     for (size_t i = 0; i < INPUT_SIZE-1; i++)
     {
          int row = i / MAP_SIZE;
          int colm = i % MAP_SIZE;
          map[row][colm] = input[i+1][0]; // skiping file name at 0
     }
}

void nextGen()
{
     char nexGenMap[MAP_SIZE][MAP_SIZE] = {0};

     for (int row = 0; row < MAP_SIZE; row++)
     {
          for (int colm = 0; colm < MAP_SIZE; colm++)
          {
               int aliveCells = 0;

               // Finding the number of Alive Neighbours
               for (int i = -1; i <= 1; i++)
               {
                    for (int j = -1; j <= 1; j++)
                    {
                         int x = row + i;
                         int y = colm + j;

                         if(x == row && y == colm)
                              continue;

                         if ( x >= 0 && x < MAP_SIZE && y >= 0 && y < MAP_SIZE)
                         {
                              if(map[x][y] == '1' || map[x][y] == '@')
                                   aliveCells++;

                   } 
                    }
                    
               }

               // Next Generation
               if(map[row][colm] == '1' && (aliveCells == 2 || aliveCells == 3))
               {
                    nexGenMap[row][colm] = '1';
               }
               else if (map[row][colm] == '@' && (aliveCells == 2 || aliveCells == 3))
               {
                    nexGenMap[row][colm] = '@';
               }
               else if (map[row][colm] == '0' && aliveCells == 3)
               {
                    nexGenMap[row][colm] = '1';
               }
               else
               {
                    nexGenMap[row][colm] = '0';
               }
               
               
          }
          
     }

     memcpy(map, nexGenMap, MAP_SIZE*MAP_SIZE);
     generation++;
}
int main(int argc, char** args)
{
     if(argc != INPUT_SIZE)
     {
          printf("Incorect input\n %d", argc);
          return EXIT_FAILURE;
     }
          

     parseInput(args);

     printMap();

     nextGen();

     printMap();

     return EXIT_SUCCESS;
}

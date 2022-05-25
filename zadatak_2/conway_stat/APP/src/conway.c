/*
* ETF Banja Luka (Purv - 2022)
* Aleksanda Beslic 1198/17
* Conway game of life 10x10 with statistics
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAP_SIZE 10
#define SLEEP_TIME 5 /* in seconds*/
#define BUFF_SIZE 80

#define ALIVE_CELL '*'
#define DEAD_CELL '.'
#define FS_NAME "/sys/kernel/stat_sysfs/stat_value"

char map[MAP_SIZE][MAP_SIZE] = {0};
int generation = 0;
int live_cells = 0;
int dead_cells = 0;
int born_cells = 0;

sem_t map_sem[MAP_SIZE][MAP_SIZE];

/*
* Prints map on screan
*/
void printMap()
{
     printf("Generation: %d\n", ++generation);
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

/*
*   Counts the number of live cells
* 
*/
void count_live_cells()
{
     live_cells = 0;

     for (size_t i = 0; i < MAP_SIZE; i++)
     {
          for (size_t j = 0; j < MAP_SIZE; j++)
          {
               if(map[i][j] == ALIVE_CELL)
                    live_cells++;
          }
          
     }
}

/*
* Prints the statistics in the file system (FS_NAME)
* 
*/
void print_stat()
{
     char buff[BUFF_SIZE];

     count_live_cells();

     sprintf(buff, "%d %d %d %d", live_cells, born_cells, dead_cells, generation-1);

     int file_desc = open(FS_NAME, O_RDWR);

	if(file_desc < 0)
	{
		perror("open");
		return;
	}
	
	// Pisanje u driver
	int ret_val = write(file_desc, buff, BUFF_SIZE);
	close(file_desc);
	
	if(ret_val < 0)
	{
		perror("write");
	}

    live_cells = 0; // for next gen.
}


/*
*  Next Generation Calculation
* 
*/
void nextGen(int row, int colm)
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
                    if(map[x][y] == ALIVE_CELL)
                    {
                         aliveCells++; // live neighbor cells
                    }     
               }
          }
     }

     // Next Generation
     // lives
     if(map[row][colm] == ALIVE_CELL && (aliveCells == 2 || aliveCells == 3))
     {
          map[row][colm] = ALIVE_CELL;
     }
     // comes to life
     else if (map[row][colm] == DEAD_CELL && aliveCells == 3)
     {
          map[row][colm] = ALIVE_CELL;
          born_cells++;
     }
     //dies
     else
     {
          dead_cells++;
          map[row][colm] = DEAD_CELL;
     }
     
}

void* conway(void* pParam)
{
     int location = ((int*)pParam);
     int row = location / MAP_SIZE;
     int colm = location % MAP_SIZE;

     while (1)
     {
          sem_wait(&map_sem[row][colm]);

          nextGen(row, colm);
          
          // Last element
          if (row == MAP_SIZE-1 && colm == MAP_SIZE-1)
          {
               if(system("clear") == -1){
                    printf("can't clear screan");
               }

               printMap();
               print_stat();
               sleep(SLEEP_TIME);
               sem_post(&map_sem[0][0]);
          }

          // Signaling to others
          if (colm + 1 < MAP_SIZE)
               sem_post(&map_sem[row][colm + 1]);

          if (row + 1 < MAP_SIZE && colm - 2 >= 0)
               sem_post(&map_sem[row + 1][colm - 2]);             
     }
     
     return 0;
}

/*
* Puts Alive or Dead cells on randow locations
*
*/
void random_stuff(){
     time_t t;
     srand((unsigned) time(&t));

     for (size_t i = 0; i < MAP_SIZE; i++)
     {
          for (size_t j = 0; j < MAP_SIZE; j++)
          {
               map[i][j] = rand() < RAND_MAX / 2 ? ALIVE_CELL : DEAD_CELL;
          }
     }
}

int main()
{
     if(system("clear") == -1){
          printf("can't clear screan");
     }
     random_stuff();
     printMap();
     sleep(SLEEP_TIME);

     // Initialize semaphors
     for (size_t i = 0; i < MAP_SIZE; i++)
     {
          for (size_t j = 0; j < MAP_SIZE; j++)
          {
               sem_init(&map_sem[i][j], 0, 0);
          }
     }
     
     // Crating threads
     pthread_t pNiti[MAP_SIZE * MAP_SIZE];

	for (size_t i = 0; i < MAP_SIZE * MAP_SIZE; i++)
	{
		pthread_create(&pNiti[i], NULL, conway, (void*)i);
	}
     
     // Posting the first thread
     sem_post(&map_sem[0][0]);

     // Joying all thread
     for (size_t i = 0; i < MAP_SIZE * MAP_SIZE; i++)
	{
		pthread_join(pNiti[i], NULL);
	}

     // Destroying all semafors
     for (size_t i = 0; i < MAP_SIZE; i++)
     {
          for (size_t j = 0; j < MAP_SIZE; j++)
          {
               sem_destroy(&map_sem[i][j]);
          }
     }

     return EXIT_SUCCESS;
}

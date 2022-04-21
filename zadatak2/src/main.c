#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <termios.h>
#include <pthread.h>

int COL_NUM = 10;
int ROW_NUM = 10;

int thread_priority = 0, stop = 1, i, j;
pthread_mutex_t lock;
pthread_cond_t* cond;
int	**board, **board_pom, pause_p;

typedef struct thread_status{
    int x, y, priority;
}TS;

void print_table(int** table){
    for(i = 0; i < ROW_NUM; i++, printf("\n"))
        for(j = 0; j < COL_NUM; j++)
            printf("%d ", table[i][j]);
}

int** fill_table(){
    int** table = (int**)malloc(ROW_NUM * sizeof(int*));
    for (i = 0; i < ROW_NUM; i++)
        table[i] = (int*)malloc(COL_NUM * sizeof(int));
    for(i = 0; i < ROW_NUM; i++)
        for(j = 0; j < COL_NUM; j++)
            //table[i][j] = 0;
            table[i][j] = rand()%2;
    /*table[2][4] = 1;
    table[3][4] = 1;
    table[2][3] = 1;
    table[3][3] = 1;
    table[4][5] = 1;
    table[5][5] = 1;
    table[4][6] = 1;
    table[5][6] = 1;*/

    /*table[2][4] = 1;
    table[3][4] = 1;
    table[3][2] = 1;
    table[4][4] = 1;
    table[4][3] = 1;*/
    return table;
}

int check_cell(int y, int x){
    int num_of_cells = 0;
    if(y-1 >= 0){
        num_of_cells += board[y-1][x];
        if(x-1 >= 0){
            num_of_cells += board[y-1][x-1];
        }
        if(x+1 < COL_NUM){
            num_of_cells += board[y-1][x+1];
        }
    }
    if(y+1 < ROW_NUM){
        num_of_cells += board[y+1][x];
        if(x+1 < COL_NUM){
            num_of_cells += board[y+1][x+1];
        }
        if(x-1 >= 0){
            num_of_cells += board[y+1][x-1];
        }
    }
    if(x-1 >= 0){
        num_of_cells += board[y][x-1];
    }
    if(x+1 < COL_NUM){
        num_of_cells += board[y][x+1];
    }
    return num_of_cells;
}

void* game(void* arg){
    TS stat = *(TS*)arg;
    while(1){
        pthread_mutex_lock(&lock);
        if(stat.priority != thread_priority){
            pthread_cond_wait(&cond[stat.priority], &lock);
        }
        int num_of_cells;
        num_of_cells = check_cell(stat.y, stat.x);
        if(board[stat.y][stat.x] == 1){
            if(num_of_cells < 2)
                board_pom[stat.y][stat.x] = 0;
            else if(num_of_cells < 4)
                board_pom[stat.y][stat.x] = 1;
            else
                board_pom[stat.y][stat.x] = 0;
        }
        else{
            if(num_of_cells == 3)
                board_pom[stat.y][stat.x] = 1;
            else
                board_pom[stat.y][stat.x] = 0;
        }
        thread_priority++;
        if(thread_priority == 100 && stop){
            for(i = 0; i < ROW_NUM; i++)
                for(j = 0; j < COL_NUM; j++)
                    board[i][j] = board_pom[i][j];
            print_table(board);
            usleep(pause_p*1000);
            system("clear");
            thread_priority = 0;
            pthread_cond_signal(&cond[thread_priority]);
        }
        else if(stop == 0){
			for(i = 0; i < 100; i++)
				pthread_cond_signal(&cond[i]);
		}
        else
            pthread_cond_signal(&cond[thread_priority]);
        pthread_mutex_unlock(&lock);
    }
}
int main (int argc, char *argv[]) {
	int br = 0, num_of_cond = COL_NUM*ROW_NUM;
	cond = (pthread_cond_t*)malloc(num_of_cond*sizeof(pthread_cond_t));
	pthread_mutex_init(&lock, NULL);
	pthread_t cells[COL_NUM*ROW_NUM];
	TS stat[COL_NUM*ROW_NUM];
	board_pom = (int**)malloc(ROW_NUM * sizeof(int*));
	srand(time(0));

	printf("Unesite vrijeme uspavljivanja niti [u milisekundama]: ");
	scanf("%d", &pause_p);

    for (i = 0; i < ROW_NUM; i++)
        board_pom[i] = (int*)malloc(COL_NUM * sizeof(int));

    for(i = 0; i < COL_NUM*ROW_NUM; i++){
        pthread_cond_init(&cond[i], NULL);
	}

	board = fill_table();
	print_table(board);
	usleep(pause_p*1000);
	system("clear");

	for(i = 0; i < ROW_NUM; i++)
        for(j = 0; j < COL_NUM; j++, br++){
            stat[br].x = j;
            stat[br].y = i;
            stat[br].priority = br;
            pthread_create(&cells[br], NULL, game, (void *)&stat[br]);
        }
        
    while(1){
		if(getchar() == 's'){
			stop = 0;
			pthread_mutex_destroy(&lock);
			for(i = 0; i < num_of_cond; i++){
				pthread_cond_destroy(&cond[i]);
			}
			free(cond);
			for (i = 0; i < ROW_NUM; i++)
				free(board[i]);
			free(board);
			for (i = 0; i < ROW_NUM; i++)
				free(board_pom[i]);
			free(board_pom);
			return 0;
		}
	}
}

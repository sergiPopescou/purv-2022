#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>

// Procfs helping variables
int num_generations=0;
int live_cells=0;
int born_cells=0;
int died_cells=0;
signed char statistic_cell[10][10];
// =========================//


/*
    potrebno naglasiti signed jer u ARM arhitekturi
    char je unsigned
*/
signed char new_cell[10][10];
signed char old_cell[10][10];


typedef struct position{
    int x;
    int y;
}POSITION;

sem_t print_semaphore;
sem_t cell_semaphore_finished[10][10];
pthread_mutex_t cell_mutex;
pthread_t  cell_thread[10][10];
pthread_t print_thread;

POSITION cell_position[10][10];
int run = 1;


void signal_from_cell(int x , int y){
    if(x<9) {
    //prvo smanjujem vrijednost semafora kako ne bi doslo do stanja da je vrijednost
    //veca od 1
        sem_trywait(&(cell_semaphore_finished[x+1][y]));
        sem_post(&cell_semaphore_finished[x+1][y]);
    }
    if(y<9){
    //prvo smanjujem vrijednost semafora kako ne bi doslo do stanja da je vrijednost
    //veca od 1
        sem_trywait(&(cell_semaphore_finished[x][y+1]));
        sem_post(&cell_semaphore_finished[x][y+1]);
    }
    if(x == 9 && y == 9){
        sem_trywait(&print_semaphore);
        sem_post(&print_semaphore);
    }
}

int calcLiveNeighbors(int row, int column, int rows, int columns) {
	int liveNeighbors = 0;

	int spaceAbove = row > 0;
	int spaceBelow = rows-1 > row;
	int spaceLeft = column > 0;
	int spaceRight = columns-1 > column;

	if (spaceAbove && old_cell[row-1][column] == 'X') liveNeighbors++;
	if (spaceBelow && old_cell[row+1][column] == 'X') liveNeighbors++;
	if (spaceLeft && old_cell[row][column-1] == 'X') liveNeighbors++;
	if (spaceRight && old_cell[row][column+1] == 'X') liveNeighbors++;

	if (spaceAbove && spaceRight && old_cell[row-1][column+1] == 'X') liveNeighbors++;
	if (spaceAbove && spaceLeft && old_cell[row-1][column-1] == 'X') liveNeighbors++;
	if (spaceBelow && spaceRight && old_cell[row+1][column+1] == 'X') liveNeighbors++;
	if (spaceBelow && spaceLeft && old_cell[row+1][column-1] == 'X') liveNeighbors++;

	return liveNeighbors;
}
void* evaluate_cell(void* args){
    POSITION *p =(POSITION*)args;
    while(run){

        sem_wait(&cell_semaphore_finished[p->x][p->y]);
        //-----mutex nije potreban jer ne mijenjam stanje new_cell niti old_cell
        //pthread_mutex_lock(&cell_mutex);
        
        int neighbors;
        neighbors = calcLiveNeighbors(p->x, p->y, 10, 10);
	    if (old_cell[p->x][p->y] == 'X') {
	    	if (neighbors < 2) {
	    		new_cell[p->x][p->y] = 'O';
	    	} else if (neighbors < 4) {
	    		new_cell[p->x][p->y] = 'X';
	    	} else {
	    		new_cell[p->x][p->y] = 'O';
	    	}
	    } else {
			if (neighbors == 3) {
    			new_cell[p->x][p->y] = 'X';
    	    } else {
    		    new_cell[p->x][p->y] = 'O';
    	}
	    }
        signal_from_cell(p->x,p->y);
       // pthread_mutex_unlock(&cell_mutex);
    }
    
    return 0;
}

void * print_cells(){
    while(run){
        sem_wait(&print_semaphore);
        system("clear");
        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                printf("%c",new_cell[i][j]);

                // Pomocu ove matrice lako odredjujemo broj rodjenih i umrlih celija
                statistic_cell[i][j]=new_cell[i][j]-old_cell[i][j];
                // ================================================//
                if(new_cell[i][j]=='X')
                    live_cells++;
                old_cell[i][j]=new_cell[i][j];
            }
            printf("\n");
        }


        num_generations++;

        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                // ako je u matrici vrijednost veca od 0 znaci da je nova celija rodjena jer 'X'-'O'>0
                if(statistic_cell[i][j]>0)
                    born_cells++;
                // Ako je u matrici vrijednost manja od 0 znaci da je nova celija umrla jer 'O'-'X'<0
                else if(statistic_cell[i][j]<0)
                    died_cells++;
            }
        }
        
        /* 
            Dio koda za upis u procfs
            1.Upisuje broj generacija
            2.Broj rodjenih celija
            3.Broj umrlih celija
            4.Broj zivih celija
        */
        char text[50];
        sprintf(text,"Generacija: %d\nZive: %d\nRodjenje: %d\nUmrle: %d\n",num_generations,live_cells,born_cells,died_cells);
        
        //=========================//
        int fd=open("/proc/GoL_stat",O_RDWR);
        if(fd < 0) {
            printf("Cannot open device file...\n");
            return 0;
        }
        write(fd,text,strlen(text));
        close(fd);
        // Vracanje na pocetne vrijednosti
        for(int i=0;i<10;i++){
            for(int j=0;j<10;j++){
                statistic_cell[i][j]=0;
            }
        }
        born_cells=0;
        died_cells=0;
        live_cells=0;
        //==========================//

        sleep(2);
        sem_post(&(cell_semaphore_finished[0][0]));
    }
}

int main(){
    //-----popunjavanje POSITION-----
    //-----inicijalizacija semafora----

    system("clear");
    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            cell_position[i][j].x=i;
            cell_position[i][j].y=j;
            
            old_cell[i][j]='O';
            
            if(i==0 && j==0)
                sem_init(&(cell_semaphore_finished[i][j]),0,1);
            else
                sem_init(&(cell_semaphore_finished[i][j]),0,0);
        }
    }
    //-----patern 1--------

    // old_cell[3][4]='X';
    // old_cell[3][3]='X';
    // old_cell[4][3]='X';
    // old_cell[4][4]='X';
    // old_cell[5][5]='X';
    // old_cell[5][6]='X';
    // old_cell[6][5]='X';
    // old_cell[6][6]='X';


    //----oscilator--------
    old_cell[3][3]='X';
    old_cell[3][4]='X';
    old_cell[3][5]='X';
    

    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            printf("%c",old_cell[i][j]);
        }
        printf("\n");
    }
        printf("\n\n\n");
    //-----inicijalizacija mutex-a-------
    pthread_mutex_init(&cell_mutex,NULL);

    for(int i=0;i<10;i++){
        for(int j=0;j<10;j++){
            pthread_create(&(cell_thread[i][j]),NULL,evaluate_cell,(void*)&(cell_position[i][j]));
        }
    }

    pthread_create(&print_thread,NULL,print_cells,NULL);
    getchar();
    run= 0;
    return 0;
}

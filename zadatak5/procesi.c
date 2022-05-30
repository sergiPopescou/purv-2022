#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <signal.h>
#include <stdint.h>
#include <unistd.h>
#include <sched.h>

#define wait_next_activation pause

// periodicni zadatak na 40 milisekundi
void task1(void)
{
  int i,j;
 
  for (i=0; i<4; i++) {
    for (j=0; j<1000; j++) ;
    printf("1");
    fflush(stdout);
  }
}

// periodicni zadatak na 80 milisekundi
void task2(void)
{
  int i,j;

  for (i=0; i<6; i++) {
    for (j=0; j<10000; j++) ;
    printf("2");
    fflush(stdout);
  }
}

// periodicni zadatak na 120 milisekundi
void task3(void)
{
  int i,j;

  for (i=0; i<6; i++) {
    for (j=0; j<100000; j++) ;
    printf("3");
    fflush(stdout);
  }
}

static void sighand(int s)
{}

int start_periodic_timer(uint64_t offs, int period)
{
    struct itimerval t;

    t.it_value.tv_sec = offs / 1000000;
    t.it_value.tv_usec = offs % 1000000;
    t.it_interval.tv_sec = period / 1000000;
    t.it_interval.tv_usec = period % 1000000;

    signal(SIGALRM, sighand);

    return setitimer(ITIMER_REAL, &t, NULL);
}

int main(int argc, char *argv[])
{
    pid_t child1;
    child1 = fork();
    if (child1 < 0) {
        perror("Fork");
        return -1;
    }
    if (child1 == 0) {
		
		int pmin = sched_get_priority_min(SCHED_FIFO);
		struct sched_param param; 
		param.sched_priority = pmin + 30; 
		sched_setscheduler(0, SCHED_FIFO, &param);
		
		int res;
		res = start_periodic_timer(2000000, 40000);
		if (res < 0) {
			perror("Start Periodic Timer");
			return -1;
		}

		while(1) {
			wait_next_activation();
			task1();
		}
		return 0;
	}
	
	pid_t child2;
    child2 = fork();
    if (child2 < 0) {
        perror("Fork");
        return -1;
    }
    if (child2 == 0) {
		
		int pmin = sched_get_priority_min(SCHED_FIFO);
		struct sched_param param; 
		param.sched_priority = pmin + 20; 
		sched_setscheduler(0, SCHED_FIFO, &param);
		
		int res;
		res = start_periodic_timer(2000000, 80000);
		if (res < 0) {
			perror("Start Periodic Timer");
			return -1;
		}

		while(1) {
			wait_next_activation();
			task2();
		}
		return 0;
	}
	
	pid_t child3;
    child3 = fork();
    if (child3 < 0) {
        perror("Fork");
        return -1;
    }
    if (child3 == 0) {
		
		int pmin = sched_get_priority_min(SCHED_FIFO);
		struct sched_param param; 
		param.sched_priority = pmin + 10; 
		sched_setscheduler(0, SCHED_FIFO, &param);
		
		int res;
		res = start_periodic_timer(2000000, 120000);
		if (res < 0) {
			perror("Start Periodic Timer");
			return -1;
		}

		while(1) {
			wait_next_activation();
			task3();
		}
		return 0;
	}
	
	int pmin = sched_get_priority_min(SCHED_FIFO);
	struct sched_param param; 
	param.sched_priority = pmin; 
	sched_setscheduler(0, SCHED_FIFO, &param);
	
	wait();
	wait();
	wait();
	return 0;
}

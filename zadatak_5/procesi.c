#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <sched.h>

#define NSEC_PER_SEC 1000000000ULL
#define TASK_1_PERIOD 60000
#define TASK_2_PERIOD 80000
#define TASK_3_PERIOD 120000

static inline void timespec_add_us(struct timespec *t, uint64_t d)
{
    d *= 1000;
    d += t->tv_nsec;
    while (d >= NSEC_PER_SEC) {
        d -= NSEC_PER_SEC;
		t->tv_sec += 1;
    }
    t->tv_nsec = d;
}

static void wait_next_activation(struct timespec *r, int period)
{
    clock_nanosleep(CLOCK_REALTIME, TIMER_ABSTIME, r, NULL);
    timespec_add_us(r, period);
}

int start_periodic_timer(uint64_t offs, struct timespec *r)
{
    clock_gettime(CLOCK_REALTIME, r);
    timespec_add_us(r, offs);

    return 0;
}

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

static void periodic(int period, void (*task)(void))
{
  struct timespec r;
  
  int res = start_periodic_timer(1000000, &r);
  
  if (res < 0) {
    perror("Start Periodic Timer");
    exit(EXIT_FAILURE);
  }

  while (1)
  {
    wait_next_activation(&r, period);
    task();
  }
  
}

int main(void)
{
  pid_t child_1, child_2;

  child_1 = fork();

  if (child_1 < 0) 
  {
    perror("Fork");
    return -1;
  }
  
  if (child_1 == 0)
  {
    int pmin = sched_get_priority_min(SCHED_FIFO);
	struct sched_param param; 
	param.sched_priority = pmin + 10; 
	sched_setscheduler(0, SCHED_FIFO, &param);

    periodic(TASK_1_PERIOD, task1);
  }
  else
  {
    child_2 = fork();

    if (child_2 == 0)
    {
      int pmin = sched_get_priority_min(SCHED_FIFO);
      struct sched_param param; 
      param.sched_priority = pmin + 5; 
      sched_setscheduler(0, SCHED_FIFO, &param);

      periodic(TASK_2_PERIOD, task2);
    }
    else
    {
      int pmin = sched_get_priority_min(SCHED_FIFO);
      struct sched_param param; 
      param.sched_priority = pmin; 
      sched_setscheduler(0, SCHED_FIFO, &param);

      periodic(TASK_3_PERIOD, task3);
    }
    
  }
  
  return EXIT_SUCCESS;
}
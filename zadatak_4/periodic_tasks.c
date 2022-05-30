#include <sys/time.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <pthread.h>

#define NSEC_PER_SEC 1000000000ULL
#define TASK_1_PERIOD 60000
#define TASK_2_PERIOD 80000

static int res;

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

void task1(void)
{
  int i,j;
 
  for (i=0; i<4; i++) {
    for (j=0; j<10; j++) ;
    printf("1");
    fflush(stdout);
  }
}

void task2(void)
{
  int i,j;

  for (i=0; i<6; i++) {
    for (j=0; j<10; j++) ;
    printf("2");
    fflush(stdout);
  }
}

void* task1_thread(void* arg)
{
  static struct timespec r1;

  res = start_periodic_timer(3000000, &r1);
  if (res < 0) {
    perror("Start Periodic Timer");
    return &res;
  }

  while (1)
  {
    wait_next_activation(&r1, TASK_1_PERIOD);
    task1();
  }

}

void* taks2_thread(void* arg)
{
  static struct timespec r2;
  

  res = start_periodic_timer(2000000, &r2);
  if (res < 0) {
    perror("Start Periodic Timer");
    return &res;
  }

  while (1)
  {
    wait_next_activation(&r2,TASK_2_PERIOD);
    task2();
  }
  
}

int main()
{

  pthread_t t_task_1, t_task_2;

  pthread_create(&t_task_1, NULL, task1_thread, NULL);
  pthread_create(&t_task_2, NULL, taks2_thread, NULL);

  pthread_join(t_task_1, NULL);
  pthread_join(t_task_2, NULL);

  return EXIT_SUCCESS;
}
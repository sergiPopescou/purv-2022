#include <sys/types.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include <sched.h>
#include <signal.h>
#include <time.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
static sigset_t sigset;



// periodicni zadatak na 40 milisekundi
void task1(void)
{
  int i, j;

  for (i = 0; i < 4; i++)
  {
    for (j = 0; j < 1000; j++);
    printf("1");
    fflush(stdout);
  }
}

// periodicni zadatak na 80 milisekundi
void task2(void)
{
  int i, j;

  for (i = 0; i < 6; i++)
  {
    for (j = 0; j < 10000; j++);
    printf("2");
    fflush(stdout);
  }
}

// periodicni zadatak na 120 milisekundi
void task3(void)
{
  int i, j;

  for (i = 0; i < 6; i++)
  {
    for (j = 0; j < 100000; j++);
    printf("3");
    fflush(stdout);
  }
}

static void wait_next_activation(void)
{
  int dummy;

  sigwait(&sigset, &dummy);
}

int start_periodic_timer(uint64_t offs, int period)
{
  struct itimerspec t;
  struct sigevent sigev;
  timer_t timer;
  const int signal = SIGALRM;
  int res;

  t.it_value.tv_sec = offs / 1000000;
  t.it_value.tv_nsec = (offs % 1000000) * 1000;
  t.it_interval.tv_sec = period / 1000000;
  t.it_interval.tv_nsec = (period % 1000000) * 1000;

  // ovaj dio ostaje isti
  sigemptyset(&sigset);
  sigaddset(&sigset, signal);
  sigprocmask(SIG_BLOCK, &sigset, NULL);

  // alociranje i popunjavanje sigevent strukture
  memset(&sigev, 0, sizeof(struct sigevent));
  sigev.sigev_notify = SIGEV_SIGNAL; // kada vrijeme istekne
                                     // okinuce se signal, jer se ostatak
                                     // koda oslanja na signale
  sigev.sigev_signo = signal;        // koji signal? pa SIGALRM, naravno

  // kreiranje tajmera
  res = timer_create(CLOCK_MONOTONIC, &sigev, &timer);
  if (res < 0)
  {
    perror("Timer Create");

    exit(-1);
  }
  // pokretanje tajmera
  return timer_settime(timer, 0 /*TIMER_ABSTIME*/, &t, NULL);
}

void print_child_status (int status) 
{
	if (WIFEXITED (status)) {
		printf("Child exited with status %d\n", WEXITSTATUS (status));
	} else if (WIFSTOPPED (status)) {
		printf("Child stopped by signal %d (%d)\n", WSTOPSIG (status),
	    strsignal(WSTOPSIG (status)));
	} else if (WIFSIGNALED (status)) {
		printf("Child killed by signal %d (%d)\n", WTERMSIG (status),
	    strsignal(WTERMSIG (status)));
	} else {
		printf("Unknown child status\n");
	}
}

int main(int argc, char *argv[])
{
  int status1,status2,status3;
  pid_t child1,child2,child3;
  printf("Parent pid %d\n",getpid());
  child1 = fork();
  if (child1 < 0) 
  {
      perror("Fork");
      return -1;
  }
    
  if (child1 == 0)
  {
    int pmin = sched_get_priority_min(SCHED_FIFO);
    struct sched_param param; 
    param.sched_priority = pmin+2; 
    sched_setscheduler(0, SCHED_FIFO, &param);
    
    int res;
    res = start_periodic_timer(1000000, 40000);
    if (res < 0)
    {
      perror("Start Periodic Timer");

      return -1;
    }

    while (1)
    {
      wait_next_activation();
      task1();
    }

    return 1;
  }



  child2 = fork();
  if (child2 < 0) 
  {
      perror("Fork");
      return -1;
  }
    
  if (child2 == 0)
  {
    int pmin = sched_get_priority_min(SCHED_FIFO);
    struct sched_param param; 
    param.sched_priority = pmin + 1; 
    sched_setscheduler(0, SCHED_FIFO, &param);
    
    int res;
    res = start_periodic_timer(1000000, 80000);
    if (res < 0)
    {
      perror("Start Periodic Timer");

      return -1;
    }

    while (1)
    {
      wait_next_activation();
      task2();
    }

    return 2;
  }



  child3=fork();
  if (child3 < 0) 
  {
      perror("Fork");
      return -1;
  }
    
  if(child3==0)
  {
    int pmin = sched_get_priority_min(SCHED_FIFO);
    struct sched_param param; 
    param.sched_priority = pmin;  
    sched_setscheduler(0, SCHED_FIFO, &param);

    int res;
    res = start_periodic_timer(1000000, 120000);
    if (res < 0)
    {
      perror("Start Periodic Timer");

      return -1;
    }

    while (1)
    {
      wait_next_activation();
      task3();
    }
    
    return 3;
  }
  
  
  waitpid(child1,&status1,0);
  waitpid(child2,&status2,0);
  waitpid(child3,&status3,0);

  return 0;
}
